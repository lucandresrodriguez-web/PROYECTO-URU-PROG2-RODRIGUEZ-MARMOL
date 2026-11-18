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

### 1. Tamaños en bytes en las estructuras (`sizeof`):
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

### 2. Formula de desplazamiento para el acceso aleatorio
Para evitar recorridos secuenciales ineficientes, el sistema utiliza punteros de posicionamiento en disco (`seekg` y `seekp`) fundamentados en el siguiente principio matemático:

$$\text{Posición Física (Bytes)} = \text{sizeof}(\text{ArchivoHeader}) + (\text{ID} - 1) \times \text{sizeof}(\text{EstructuraTarget})$$

#### Desglose de la lógica de posicionamiento:
1. **Salto de Cabecera (`sizeof(ArchivoHeader)`):** Todo archivo binario inicia con un bloque fijo de 16 bytes reservado para su header de control. El puntero debe saltar esta sección obligatoriamente para no sobrescribir ni interpretar mal los metadatos.
2. **Índice Relativo (`ID - 1`):** Dado que los identificadores únicos (`id`) en el sistema comienzan en `1` (y no en `0`), se resta una unidad para sincronizar el ID con el índice base del arreglo físico en el archivo.
3. **Escala de Bloque (`× sizeof(...)`):** Se multiplica el índice relativo por el tamaño exacto en bytes que ocupa la estructura en disco, garantizando que el cursor caiga exactamente en el inicio del registro buscado, permitiendo lecturas o escrituras atómicas de un solo registro a la vez.

## Relación entre ID e Índice Físico y Condiciones de Invalidez:

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

## Diseño del detalle de la orden:

Siguiendo los requerimiento tecnicos pedidos para el proyecto, el sistema implementa un modelo de almacenamiento persistente de **tamaño fijo** para gestionar las lineas de componentes (repuestos) asociadas a cada orden de servicio. Este diseño reemplaza los arreglos dinámicos en memoria volátil por una tabla relacional en disco, permitiendo almacenar múltiples detalles por cada orden de manera estructurada y eficiente.

### 1. Estructura y Distribucion de Bytes (`DetalleArchivo`):
Cada linea de repuesto vinculada a una orden se almacena en el archivo binario `datos/detalles.bin` utilizando la estructura `DetalleArchivo`, la cual posee un tamaño exacto y constante de **160 bytes** en disco:

| Campo del Registro | Tipo de Dato | Tamaño | Descripcion del Atributo |
| :--- | :--- | :--- | :--- |
| **`idDetalle`** | `int` | 4 bytes | Identificador unico autoincremental del detalle. |
| **`idOrden`** | `int` | 4 bytes | Clave foranea que vincula la linea con la `OrdenServicio` principal. |
| **`idRepuesto`** | `int` | 4 bytes | Clave foránea que referencia al componente en el inventario (`repuestos.bin`). |
| **`codigoRepuesto`** | `char[20]` | 20 bytes | Código alfanumérico histórico del repuesto. |
| **`nombreRepuesto`** | `char[100]` | 100 bytes | Nombre descriptivo del repuesto (respaldo histórico ante cambios de inventario). |
| **`cantidad`** | `int` | 4 bytes | Unidades físicas del componente consumidas en la orden. |
| **`precioUnitario`** | `float` | 4 bytes | Costo unitario histórico del repuesto al momento de su asignación. |
| **`eliminado`** | `bool` | 1 byte | Indicador de borrado lógico (`true` si el repuesto fue retirado de la orden). |
| **`fechaCreacion`** | `time_t` | 8/4 bytes | Marca de tiempo UNIX de la creación del detalle. |
| **`fechaUltimaModificacion`** | `time_t` | 8/4 bytes | Marca de tiempo UNIX de la última modificación o retiro. |

### 2. Modelo Relacional y Gestion de Multiples Lineas por Orden:
A diferencia de entidades como Vehiculos o Mecanicos (donde existe una correspondecia estricta de un registro por ID fisico), `detalles.bin` funciona como una **tabla relacional dependiente de uno a muchos (1:N)**:
* Una misma orden de servicio (`idOrden`) puede tener asociados múltiples registros independientes en `detalles.bin`, cada uno representando un repuesto distinto.
* **Lectura y Filtrado por Orden:** Dado que los registros de detalles se escriben secuencialmente conforme se agregan a cualquier orden, las funciones de consulta (como `buscarDetallesDeOrden`) leen el archivo omitiendo el encabezado (`sizeof(ArchivoHeader)`) y recorren los bloques validando que el `idOrden` coincida y que el indicador `eliminado` sea falso (`false`).
* **Control de Stock y Restitución:** Al agregar un repuesto, el sistema descuenta las unidades del inventario principal y crea un nuevo registro en `detalles.bin`. Si se retira un repuesto o se anula la orden, el sistema aplica un borrado lógico del detalle (`eliminado = true`) y restituye de forma automática las cantidades al stock del repuesto correspondiente.

## Firmas diseñadas para Persistencia y Logica de los modulos:

El sistema separa estrictamente sus responsabilidades arquitectonicas mediante un diseño modular. La capa de persistencia interactua directamente con los archivos binarios utilizando codigos de estados estatico (`enum class`), mientras que la capa logica gestiona las reglas de negocio en memoria y valida las restricciones operativas antes de consolidar los cambios.

### A. Modulo de Vehiculos:
* Capa de persistencia (Acceso de bloques y actualizacion):
    * `bool eliminarVehiculo(SistemaTaller*s, int id);`
        * Exito: Retorna `true` al marca logicamente el vehiculo como eliminado (`eliminado = true`), actualizar su byte de control en `vehiculos.bin` mediante `seekp()` y decrementar los registros activos en el `ArchivoHeader`.
        * Fallo y causa: Retorna `false` si el vehiculo no existe, ya se encuentra previamente eliminado o posee ordenes de servicio asociadas en curso.

* Capa Logica (Validacion y Manipulacion):
    * `Vehiculo* agregarVehiculo(SistemaTaller* s, Vehiculo datos);`
        * Exito: Retorna un puntero directo al nuevo registro insertado en el arreglo dinamico tras validar las reglas de la placa (6-7 caracteres), unicidad y rangos de año.
        * Fallo y causa: Retorna `nullptr` si la placa esta duplicada, la cedula del propietario es invalida o los datos no superan los filtros de validacion.

### B. Modulo de Mecanicos:
* Capa de persistencia (Gestion de Estados en Disco):
    * `bool eliminarMecanico(SistemaTaller* s, int id);`
        * Exito: Retorna `true` al aplicar el borrados logico y sincronizar el cambio en el archivo `mecanicos.bin` y su respectivo header.
        * Fallo y Causa: Retorna `false` si el mecanico no existe o si `contarOrdenesAbiertasMecanico()` detecta que tiene ordenes abiertas asignadas (bloqueando su baja).

* Capa Logica (Reglas Profesionales):
    * `Mecanico* agregarMecanico(SistemaTaller*s, Mecancio datos);`
        * Exito : Retorna un puntero al mecanico recien agregado tras validar su cedula y que su especialidad pertenezca al catalogo permitido **(MOTOR, FRENOS, ELECTRICIDAD, SUSPENSION, GENERAL)**.
        * Fallo y Causa: Retorna `false` si el mecanico no existe o si `contarOrdenesAbiertasMecanico()` detecta que tiene ordenes abiertas asignadas (bloqueando su baja).

### C. Modulo de Repuestos (Inventario):
* Capa de persistencia (Control de Componentes):
    * `bool eliminarRepuesto(SistemaTaller* s, int id);`
        * Exito: Retorna `true` si el repuesto se marca como inativo en disco y actualiza el header de `repuestos.bin`.
        * Fallo y Causa: Retorna `false` si el repuesto no existe, si ya fue eliminado, o si la funcion relacional `existeDetalleDeRepuesto()` detecta que esta vinculado a lineas de detalle historicas.

* Capa Logica (Operaciones de Stock y Costos):
    * `bool menuRegistraEntradaMercancia(SistemaTaller* s, int idRepuesto, int CantidadAdicional);`
        * Exito: Retorna `true` al incrementar de forma segura las unidades disponibles en el stock del repuesto en memoria.
        * Fallo y Causa: Retorna `false` si el ID del repuesto no existe, si la cantidad adicional es menor o igual a cero, o si ocurre un desbordamiento aritmetico de enteros (`overflow`).

### D. Modulo de Ordenes de Servicio y Detalles (`detalles.bin`):
* Capa de persistencia (Manejo de Errores con `ResultadosArchivoDetalles`):
    * `ResultadoArchivoDetalles crearDetalleArchivo(DetalleArchivo& detalle);`
        * Exito: Retorna `ResultadoArchivoDetalle::Correcto` al anexar el registro de tamaño fijo de 160 bytes al final de `detalle.bin`, incrementando el conteo del header y actualizando el proximo ID.
        * Fallo y Causa: Retorna codigos especificos de error como `ArchivosNoDisponible` (si el archivo no existe), `EncabezadoInvalido` (si los metadatos estan corruptos) o `RegistroInconsistente` (si el tamaño fisico en disco no coincide con el esperado).

* Capa Logica (Transacciones Operativas):
    * `OrdenServicio* abrirOrden(SistemaTaller* s, int idVehiculo, int idMecanico, const char* descripcionFalla, int KilometrajeIngreso, const char* fechaActual);`
        * Exito: Retorna un puntero a la nueva orden abierta al verificar la existencia del vehículo y mecánico, validar el incremento del kilometraje y comprobar que el mecánico no supere el límite máximo de 3 órdenes abiertas simultáneas.
        * Fallo y Causa: Retorna `nullptr` si el vehículo o mecánico no existen, si el kilometraje es menor al registrado, si el vehículo ya posee una orden abierta, o si el mecánico alcanzó su carga máxima de trabajo.

## Preguntas de Analisis:

### 1) Escriban con write una OrdenServicio del Proyecto 1 tal cual (con su DetalleOrden* ), cierren el programa, vuelvan a ejecutarlo y lean ese registro. ¿Qué observaron al intentar acceder a sus líneas? ¿Por qué ocurre?

Al cerrar el programa, volver a ejecutarlo e intentar leer el registro de la orden junto con sus repuestos usados, ocurren algunas fallos criticos:

* Fallo de segmentacion (Segmentation Fault): Al intentar recorrer el arreglo a traves del puntero recuperado, el programa suele colapsar inmediatamente.
* Punteros Invalidos (Dangling Pointers): Si el programa no craseha de inmediato, al inspeccionar el puntero `repuestosUsados` se observa que apunta a una direccion de memoria RAM aleatoria, basura o protegida que no contiene los datos reales de los repuestos.
* Pérdida total de los detalles: Los datos de las líneas de repuestos nunca se grabaron realmente en el archivo binario, por lo que el contador `numRepuestosUsados` puede tener un número cualquiera o desfasado, y los elementos son inaccesibles.

Esta clase de fenomenos ocurre debido a la diferencia fundamental entre el almacenamiento estatico y el alamacenamiento dinamico en memoria (Heap) en C++:

* Serialización superficial de punteros: La estructura `OrdenServicio` contiene un puntero (`DetalleOrden* repuestosUsados`). Cuando se ejecuta `write()` para volcar la estructura al archivo binario, C++ únicamente copia los bytes que componen los miembros internos de la estructura en la pila (stack), lo cual incluye el valor numérico de la dirección de memoria donde apuntaba el puntero en ese instante (por ejemplo, `0x000001f...`), pero no el contenido del bloque de memoria al que apunta.
* Volatilidad de las direcciones de memoria: Las direcciones de memoria RAM son dinámicas y efímeras. Al cerrar el programa, el sistema operativo libera esos espacios. Al volver a ejecutarlo, el proceso se carga en un espacio de direcciones diferente (o el sistema operativo aplica restricciones de seguridad como ASLR). Por lo tanto, el número guardado en el archivo binario se convierte en una dirección totalmente inválida para la nueva ejecución.
* La solución arquitectónica (Fase 2): Para corregir esto en sistemas con persistencia binaria, se deben evitar los punteros dinámicos internos dentro de las estructuras guardadas en disco. En su lugar, se implementan archivos independientes de tamaño fijo (como `detalles.bin`) que se relacionan mediante claves foráneas lógicas (por ejemplo, guardando el `idOrden` en cada detalle), permitiendo almacenar y recuperar registros independientes sin perder la integridad relacional.


