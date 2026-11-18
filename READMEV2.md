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
