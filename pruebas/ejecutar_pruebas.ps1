$ErrorActionPreference = "Stop"

$repoRoot = Split-Path -Parent $PSScriptRoot
$compiler = Get-Command g++ -ErrorAction SilentlyContinue
if ($null -eq $compiler) {
    throw "No se encontro g++; instale MinGW o agregue g++ al PATH."
}

$workRoot = Join-Path $env:TEMP ("taller-pruebas-" + [Guid]::NewGuid().ToString("N"))
$seedRoot = Join-Path $workRoot "semilla"
$caseRoot = Join-Path $workRoot "casos"
$appPath = Join-Path $workRoot "taller.exe"
$generatorPath = Join-Path $workRoot "generar_datos_demo.exe"

function Invoke-CheckedProcess {
    param(
        [string]$FileName,
        [string[]]$Arguments,
        [string]$WorkingDirectory,
        [string]$InputText = ""
    )

    $startInfo = New-Object System.Diagnostics.ProcessStartInfo
    $startInfo.FileName = $FileName
    $startInfo.Arguments = ($Arguments -join " ")
    $startInfo.WorkingDirectory = $WorkingDirectory
    $startInfo.UseShellExecute = $false
    $startInfo.RedirectStandardInput = $true
    $startInfo.RedirectStandardOutput = $true
    $startInfo.RedirectStandardError = $true
    $process = New-Object System.Diagnostics.Process
    $process.StartInfo = $startInfo
    if (-not $process.Start()) {
        throw "No se pudo iniciar $FileName."
    }

    $stdoutTask = $process.StandardOutput.ReadToEndAsync()
    $stderrTask = $process.StandardError.ReadToEndAsync()
    $process.StandardInput.Write($InputText)
    $process.StandardInput.Close()
    $process.WaitForExit()
    $stdout = $stdoutTask.Result
    $stderr = $stderrTask.Result
    if ($process.ExitCode -ne 0) {
        throw "$FileName termino con codigo $($process.ExitCode).`n$stdout`n$stderr"
    }
    return $stdout
}

function Assert-Case {
    param([string]$Name)

    $directory = Join-Path $caseRoot $Name
    New-Item -ItemType Directory -Force -Path $directory | Out-Null
    Copy-Item (Join-Path $seedRoot "Datos") $directory -Recurse
    $inputText = [System.IO.File]::ReadAllText(
        (Join-Path $PSScriptRoot ("casos\" + $Name + ".in")))
    $expected = [System.IO.File]::ReadAllLines(
        (Join-Path $PSScriptRoot ("casos\" + $Name + ".out")))
    $output = Invoke-CheckedProcess -FileName $appPath -Arguments @() `
        -WorkingDirectory $directory -InputText $inputText
    foreach ($fragment in $expected) {
        if (-not $output.Contains($fragment)) {
            throw "Caso '$Name': falta la salida esperada '$fragment'.`n$output"
        }
    }
    if ($Name -eq "primer_arranque" -and
        $output.Contains("4. Ordenes de servicio")) {
        throw "Caso '$Name': se mostro Ordenes antes de registrar sus dependencias."
    }

    if ($Name -eq "crear_backup") {
        $backup = Get-ChildItem $directory -Directory |
            Where-Object { $_.Name -like "backup_*" } |
            Select-Object -First 1
        if ($null -eq $backup) {
            throw "Caso '$Name': no se creo la carpeta de respaldo."
        }
        foreach ($fileName in @(
            "taller.bin", "vehiculos.bin", "mecanicos.bin",
            "repuestos.bin", "ordenes.bin", "detalles.bin"
        )) {
            $source = [System.IO.File]::ReadAllBytes(
                (Join-Path $directory ("Datos\" + $fileName)))
            $copy = [System.IO.File]::ReadAllBytes(
                (Join-Path $backup.FullName $fileName))
            if ([Convert]::ToBase64String($source) -ne
                [Convert]::ToBase64String($copy)) {
                throw "Caso '$Name': el respaldo de $fileName no coincide byte por byte."
            }
        }
    }
    Write-Output "PASS $Name"
}

try {
    New-Item -ItemType Directory -Force -Path (Join-Path $seedRoot "Datos") | Out-Null
    New-Item -ItemType Directory -Force -Path $caseRoot | Out-Null

    & $compiler.Source -std=c++11 -Wall -Wextra -pedantic `
        -o $appPath (Join-Path $repoRoot "main.cpp")
    if ($LASTEXITCODE -ne 0) {
        throw "No se pudo compilar main.cpp."
    }
    & $compiler.Source -std=c++11 -Wall -Wextra -pedantic `
        -o $generatorPath (Join-Path $PSScriptRoot "generar_datos_demo.cpp")
    if ($LASTEXITCODE -ne 0) {
        throw "No se pudo compilar generar_datos_demo.cpp."
    }
    Invoke-CheckedProcess -FileName $generatorPath -Arguments @() `
        -WorkingDirectory $seedRoot | Out-Null

    $counts = @{
        "vehiculos.bin" = 8
        "mecanicos.bin" = 4
        "repuestos.bin" = 15
        "ordenes.bin" = 10
        "detalles.bin" = 35
    }
    foreach ($entry in $counts.GetEnumerator()) {
        $bytes = [System.IO.File]::ReadAllBytes(
            (Join-Path $seedRoot ("Datos\" + $entry.Key)))
        $count = [BitConverter]::ToInt32($bytes, 0)
        if ($count -ne $entry.Value) {
            throw "La semilla tiene $count registros en $($entry.Key); se esperaban $($entry.Value)."
        }
    }
    $mechanicHeader = [System.IO.File]::ReadAllBytes(
        (Join-Path $seedRoot "Datos\mecanicos.bin"))
    if ([BitConverter]::ToInt32($mechanicHeader, 8) -ne 3) {
        throw "La semilla debe contener tres mecanicos activos y uno dado de baja."
    }

    $orderBytes = [System.IO.File]::ReadAllBytes(
        (Join-Path $seedRoot "Datos\ordenes.bin"))
    $stateCounts = @{ "Abierta" = 0; "Cerrada" = 0; "Anulada" = 0 }
    for ($i = 0; $i -lt 10; ++$i) {
        $offset = 16 + ($i * 280) + 34
        $stateText = [System.Text.Encoding]::ASCII.GetString($orderBytes, $offset, 12)
        $state = $stateText.TrimEnd([char]0)
        if (-not $stateCounts.ContainsKey($state)) {
            throw "Estado inesperado '$state' en la orden $($i + 1)."
        }
        $stateCounts[$state] = $stateCounts[$state] + 1
    }
    if ($stateCounts["Abierta"] -ne 2 -or
        $stateCounts["Cerrada"] -ne 6 -or
        $stateCounts["Anulada"] -ne 2) {
        throw "La semilla no contiene la distribucion esperada de estados."
    }

    $detailBytes = [System.IO.File]::ReadAllBytes(
        (Join-Path $seedRoot "Datos\detalles.bin"))
    $lineCounts = @{}
    for ($i = 0; $i -lt 35; ++$i) {
        $offset = 16 + ($i * 160)
        if ($detailBytes[$offset + 140] -eq 0) {
            $idOrder = [BitConverter]::ToInt32($detailBytes, $offset + 4)
            if (-not $lineCounts.ContainsKey($idOrder)) {
                $lineCounts[$idOrder] = 0
            }
            $lineCounts[$idOrder] = $lineCounts[$idOrder] + 1
        }
    }
    if ($lineCounts[1] -lt 15 -or $lineCounts[2] -lt 4 -or
        $lineCounts[3] -lt 4 -or $lineCounts[4] -lt 4) {
        throw "La semilla no incluye las ordenes con cuatro y quince lineas requeridas."
    }

    foreach ($name in @(
        "buscar_vehiculo",
        "listar_mecanicos",
        "consultar_orden_15_lineas",
        "listar_estados_orden",
        "menu_ordenes_habilitado",
        "verificar_integridad",
        "reporte_repuestos",
        "crear_backup"
    )) {
        Assert-Case $name
    }

    $freshDirectory = Join-Path $workRoot "primer_arranque"
    New-Item -ItemType Directory -Force -Path (Join-Path $freshDirectory "Datos") | Out-Null
    $inputText = [System.IO.File]::ReadAllText(
        (Join-Path $PSScriptRoot "casos\primer_arranque.in"))
    $output = Invoke-CheckedProcess -FileName $appPath -Arguments @() `
        -WorkingDirectory $freshDirectory -InputText $inputText
    foreach ($fragment in [System.IO.File]::ReadAllLines(
        (Join-Path $PSScriptRoot "casos\primer_arranque.out"))) {
        if (-not $output.Contains($fragment)) {
            throw "El primer arranque no produjo la salida esperada '$fragment'.`n$output"
        }
    }
    $emptyFileNames = @(
        "vehiculos.bin", "mecanicos.bin", "repuestos.bin",
        "ordenes.bin", "detalles.bin"
    )
    foreach ($fileName in $emptyFileNames) {
        $bytes = [System.IO.File]::ReadAllBytes(
            (Join-Path $freshDirectory ("Datos\" + $fileName)))
        if ($bytes.Length -ne 16 -or
            [BitConverter]::ToInt32($bytes, 0) -ne 0 -or
            [BitConverter]::ToInt32($bytes, 4) -ne 1 -or
            [BitConverter]::ToInt32($bytes, 8) -ne 0 -or
            [BitConverter]::ToInt32($bytes, 12) -ne 1) {
            throw "El archivo inicial $fileName no quedo con un encabezado vacio valido."
        }
    }
    if ((Get-Item (Join-Path $freshDirectory "Datos\taller.bin")).Length -ne 360) {
        throw "taller.bin no tiene el tamaño esperado despues del primer arranque."
    }

    $inputText = [System.IO.File]::ReadAllText(
        (Join-Path $PSScriptRoot "casos\verificar_integridad.in"))
    $output = Invoke-CheckedProcess -FileName $appPath -Arguments @() `
        -WorkingDirectory $freshDirectory -InputText $inputText
    if (-not $output.Contains("Estado: SISTEMA INTEGRO")) {
        throw "Los archivos nuevos no pasaron la verificacion de integridad.`n$output"
    }
    Write-Output "PASS primer_arranque_archivos_vacios"

    $persistDirectory = Join-Path $caseRoot "persistencia_stock"
    New-Item -ItemType Directory -Force -Path $persistDirectory | Out-Null
    Copy-Item (Join-Path $seedRoot "Datos") $persistDirectory -Recurse
    $inputText = [System.IO.File]::ReadAllText(
        (Join-Path $PSScriptRoot "casos\registrar_entrada_stock.in"))
    $output = Invoke-CheckedProcess -FileName $appPath -Arguments @() `
        -WorkingDirectory $persistDirectory -InputText $inputText
    foreach ($fragment in [System.IO.File]::ReadAllLines(
        (Join-Path $PSScriptRoot "casos\registrar_entrada_stock.out"))) {
        if (-not $output.Contains($fragment)) {
            throw "La entrada de stock no produjo la salida esperada '$fragment'.`n$output"
        }
    }

    $inputText = [System.IO.File]::ReadAllText(
        (Join-Path $PSScriptRoot "casos\verificar_stock_persistido.in"))
    $output = Invoke-CheckedProcess -FileName $appPath -Arguments @() `
        -WorkingDirectory $persistDirectory -InputText $inputText
    foreach ($fragment in [System.IO.File]::ReadAllLines(
        (Join-Path $PSScriptRoot "casos\verificar_stock_persistido.out"))) {
        if (-not $output.Contains($fragment)) {
            throw "El stock no persistio tras reiniciar: falta '$fragment'.`n$output"
        }
    }
    Write-Output "PASS persistencia_stock_tras_reinicio"
    Write-Output "Todas las pruebas pasaron."
}
finally {
    if (Test-Path $workRoot) {
        Remove-Item -LiteralPath $workRoot -Recurse -Force
    }
}
