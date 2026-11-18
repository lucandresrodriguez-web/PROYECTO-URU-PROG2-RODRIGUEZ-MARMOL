# Proyecto 2: Sistema general de taller mecanico con persistencia y acceso aleatorio

## Integrantes y desarrolladores:

- Lucas Rodriguez
- Isidro Marmol

## Descripcion general de la segunda fase del proyecto:
"Presentamos una actualizacion para el sistema desarrollado en el Proyecto 1 original, teniendo su enfoque central en la transicion general de una estructura basada en la memoria volatil (RAM) a un modelo de almacenamiento persistente en disco a partir del uso de archivos binarios (.bin), aplicando operaciones de acceso aleatorio y calculo de desplazamiento (seekg y seekp)".

Mediante estas actualizaciones se implementaron detalles como:
- La carga de todo el sistema en memoria a procesar unicamente un registro a la vez cuando se requiere.
- Los datos ya no se pierden al cerrar el programa, y ahora cada uno de estos persisten de forma permanente entre ejecuciones.
- Las funciones de la capa logica ya no retornan punteros a estructuras internas del sistema, sino que cada lectura produce una copia independiente.
- Se elimina el recorrido secuencial de los arreglos, sustituyendolo por un calculo directo de la posicion en bytes a partir del ID del registro.
- Se reemplaza el arreglo dinamico en memoria (DetalleOrden*) por un estilo de persistencia de con tamaño fijo que permite almacenar multiples lineas por orden en disco.

Tambien se incorporaron dos nuevos modulos en el menu del sistema general:
1. Modulo de Mantenimiento: Usado para garantizar la integridad de los datos almacenados en disco y prevenir la perdida de informacion del taller. Funciona a partir de dos herramientas princiales: 
- Por un lado la verificacion de integridad que recorre los archivos binarios comparando los metadatos en sus encabezados con los registros reales para detectar algun tipo de inconsistencia (referencias rotas, registros inactivos o stock negativo).
- Y tambien una funcion de respaldo (backup) que crea automaticamente una carpeta identificada con la fecha y hora actual para copiar byte a byte todos los archivos (.bin) del sistema.

2. Modulo de Reportes: Permite extraer informacion analitica y contable leyendo directamente desde los archivos en disco sin cargar todo el sistema en la memoria RAM. Contiene un funcionamiento que abarca tres consultas clave:
- Historial detallado y facturacion cerrada de un vehiculo a partir de su placa.
- Ranking con los diez repuestos mas consumidos en ordenes cerradas.
- Balance de rendimiento por mecanico que totaliza sus ordenes cerradas, ingresos por manos de obra y costos de repuestos.

## Instrucciones de compilacion y ejecucion:

Requisitos:
- Compilador de C++ (g++, MinGW o MSVC)
- Consola o terminal
- Sistema operativo Windows, Linux o macOS

Pasos para la ejecutacion:
- Descarga de los archivos y carpetas por medio del ZIP en repositorio de github (usando el boton <>Code).
- Ingreso al archivo y codigo "main.cpp" en Visual Studio Code con lenguaje C++.
- Abrir terminal por medio de "GitBash" ingresando el comando "git add ." en la carpeta de Fase 1.
- Posteriormente ingresar el comando: g++ "main.cpp" -o taller.
- Se crear un archivo .exe llamado "taller" y por ultimo ingresar el comando especificio y ya estara dentro del programa:

Windows:
./taller.exe

Linux/macOS:
./taller

## Estructura, Tamaños ('sizeof') y Acceso aleatorio:

Para lograr garantizar el acceso directo, eficiente y de tiempo constante ($O(1)$) a los registros en disco sin necesidad de cargar todos los archivos en la memoria RAM, el sistema implementa un modelo de almacenamiento de **tamaño fijo**. Esto permite calcular matemáticamente la posición exacta (en bytes) de cualquier registro a partir de su ID.

### Tamaños en bytes en las estructuras (`sizeof`):
Cada entidad y su respectivo encabezado de control poseen un tamaño fijo en disco, determinado por la alineacion de memoria de sus tipos de datos:

| Estructura / Entidad | Descripcion del Rol en disco | Tamaño Exacto (`sizeof`) |
| :--- | :--- | :--- |
| **`ArchivoHeader`** | Metadatos de control (conteos, IDs proximos y version) | **16 bytes** |
| **`Taller`** | Informacion general y configuracion global del taller | **344 bytes** |
| **`Vehiculo`** | Datos operativos del automotor y su propietario | **288 bytes** |
| **`Mecanico`** | Perfil, especialidad y stado profesional del mecanico | **156 bytes**|
| **`Repuesto`** |Inventario, costos y umbrales del stock de componentes | **192 bytes** |
| **`DetalleArchivo`** | Lineas de componentes asociados a una orden de servicio| **160 bytes** |
| **`OrdenServicio`** | Cabecera operativa de servicios, fallas y mano de obra | **268 bytes** |

### Formula de desplazamiento para el acceso aleatorio
Para evitar recorridos secuenciales ineficientes, el sistema utiliza punteros de posicionamiento en disco (`seekg` y `seekp`) fundamentados en el siguiente principio matemático:

$$\text{Posición Física (Bytes)} = \text{sizeof}(\text{ArchivoHeader}) + (\text{ID} - 1) \times \text{sizeof}(\text{EstructuraTarget})$$

#### Desglose de la lógica de posicionamiento:
1. **Salto de Cabecera (`sizeof(ArchivoHeader)`):** Todo archivo binario inicia con un bloque fijo de 16 bytes reservado para su header de control. El puntero debe saltar esta sección obligatoriamente para no sobrescribir ni interpretar mal los metadatos.
2. **Índice Relativo (`ID - 1`):** Dado que los identificadores únicos (`id`) en el sistema comienzan en `1` (y no en `0`), se resta una unidad para sincronizar el ID con el índice base del arreglo físico en el archivo.
3. **Escala de Bloque (`× sizeof(...)`):** Se multiplica el índice relativo por el tamaño exacto en bytes que ocupa la estructura en disco, garantizando que el cursor caiga exactamente en el inicio del registro buscado, permitiendo lecturas o escrituras atómicas de un solo registro a la vez.

## Relación entre ID e Índice Físico y Condiciones de Invalidez

El diseño del modelo de persistencia del sistema se fundamenta en una correspondencia matemática directa, biunívoca y determinista entre la identificación lógica de un registro (su clave primaria `id`) y su ubicación espacial exacta (en bytes) dentro del archivo binario correspondiente.

### 1. Deducción de la Relación (Indexación Directa en $O(1)$)
Dado que todas las entidades del taller (`Vehículo`, `Mecánico`, `Repuesto`, `OrdenServicio` y `DetalleArchivo`) se almacenan de forma contigua y con un **tamaño estrictamente fijo en bytes** (`sizeof`), el sistema no requiere realizar búsquedas secuenciales o recorridos lineales de complejidad $O(N)$. 

En su lugar, el desplazamiento físico necesario para posicionar los punteros de lectura y escritura (`seekg` y `seekp`) se calcula de manera inmediata mediante la siguiente fórmula:

$$\text{Posición Física (Bytes)} = \text{sizeof}(\text{ArchivoHeader}) + (\text{ID} - 1) \times \text{sizeof}(\text{EstructuraTarget})$$

Desglosando sus componentes lógicos:
* **Salto del Encabezado (`sizeof(ArchivoHeader)`):** Todo archivo binario inicia con un bloque fijo de metadatos (16 bytes) que el puntero debe omitir obligatoriamente para no corromper la lectura del header.
* **Índice Relativo (`ID - 1`):** Puesto que los identificadores únicos (`id`) en el sistema son autoincrementales y comienzan en `1`, se resta una unidad para sincronizar el ID con el índice base cero ($0$) del arreglo físico en disco.
* **Escala de Bloque (`× sizeof(...)`):** Al multiplicar el índice relativo por el tamaño exacto en bytes de la estructura, se obtiene el desplazamiento absoluto exacto donde comienza el registro en el archivo binario.

### 2. ¿Qué cambios en el arquitectura del sistema harían que esta relación dejara de ser válida?
La validez y precisión matemática de esta fórmula dependen de restricciones estructurales sumamente estrictas. Cualquier modificación que rompa la linealidad, el tamaño constante o el orden correlativo de los registros invalidaría por completo el sistema de acceso aleatorio. Los factores críticos que destruirían la relación son:

* **A. Implementación de Borrado Físico y Reutilización de Espacios:**
  Si en lugar de emplear el esquema actual de **borrado lógico** (marcar el registro con el campo booleano `eliminado = true` manteniendo intacta su posición física), el sistema borrara físicamente los registros del archivo y desplazara los bloques subsiguientes para "rellenar el espacio vacante", los IDs almacenados dejarían de coincidir con su posición matemática ($\text{ID} - 1$), corrompiendo todas las lecturas posteriores.
* **B. Introducción de Longitudes o Estructuras de Tamaño Variable:**
  Si se modificaran las estructuras para admitir cadenas de texto de longitud dinámica (por ejemplo, punteros a memoria dinámica dentro de los structs, serialización basada en JSON o longitudes de campos ajustables), el tamaño de cada bloque (`sizeof`) dejaría de ser constante, haciendo matemáticamente imposible calcular el desplazamiento mediante una multiplicación estática.
* **C. Procesos de Compactación o "Garbage Collection" Global:**
  Si se programara una rutina de mantenimiento orientada a depurar el archivo eliminando físicamente los registros inactivos y compactando el espacio restante —reasignando nuevos IDs correlativos a los elementos supervivientes—, todos los punteros previos, claves foráneas y referencias cruzadas almacenadas en otras entidades (como los detalles de órdenes o historiales) quedarían totalmente desincronizados y rotos.
* **D. Inserciones No Secuenciales (Indexación por Tablas Hash o Listas Libres):**
  Si el sistema permitiera reutilizar huecos de registros eliminados mediante una lista libre enlazada en disco o una tabla hash para insertar nuevos registros en posiciones arbitrarias, la correspondencia lineal $\text{ID} - 1$ dejaría de cumplirse, ya que el orden físico en el archivo ya no reflejaría el orden secuencial de los identificadores únicos.

## Diseño del detalle de la orden
