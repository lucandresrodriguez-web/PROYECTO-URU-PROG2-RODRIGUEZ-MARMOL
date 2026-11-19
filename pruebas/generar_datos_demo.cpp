#define main taller_app_main
#include "../main.cpp"
#undef main

bool revisar(ResultadoArchivoDetalles resultado, const char* accion) {
    if (resultado == ResultadoArchivoDetalles::Correcto) return true;
    cerr << "No se pudo " << accion << " (" << static_cast<int>(resultado) << ")." << endl;
    return false;
}

void copiarTexto(char* destino, size_t capacidad, const char* texto) {
    strncpy(destino, texto, capacidad - 1);
    destino[capacidad - 1] = '\0';
}

bool crearOrden(
    SistemaTaller& sistema,
    int idVehiculo,
    int idMecanico,
    const int* repuestos,
    size_t cantidadRepuestos,
    const char* estado,
    int& idOrden) {
    char fecha[] = "2026-03-15";
    ResultadoArchivoDetalles resultado = abrirOrden(
        &sistema, idVehiculo, idMecanico, "Mantenimiento preventivo",
        15000 + idVehiculo * 500, fecha, idOrden);
    if (!revisar(resultado, "abrir una orden")) return false;

    for (size_t i = 0; i < cantidadRepuestos; ++i) {
        if (!agregarRepuestoAOrden(&sistema, idOrden, repuestos[i], 1)) {
            cerr << "No se pudo agregar el repuesto " << repuestos[i]
                 << " a la orden " << idOrden << "." << endl;
            return false;
        }
    }

    if (strcmp(estado, "Cerrada") == 0) {
        if (!cerrarOrden(&sistema, idOrden, 250.0f + idOrden, fecha)) {
            cerr << "No se pudo cerrar la orden " << idOrden << "." << endl;
            return false;
        }
    } else if (strcmp(estado, "Anulada") == 0 && !anularOrden(&sistema, idOrden)) {
        cerr << "No se pudo anular la orden " << idOrden << "." << endl;
        return false;
    }
    return true;
}

int main() {
    if (!revisar(cargarDatos(), "inicializar archivos de datos")) return 1;

    ArchivoHeader header;
    if (!revisar(leerHeaderVehiculos(header), "leer vehiculos.bin") ||
        header.cantidadRegistros != 0 ||
        !revisar(leerHeaderMecanicos(header), "leer mecanicos.bin") ||
        header.cantidadRegistros != 0 ||
        !revisar(leerHeaderRepuestos(header), "leer repuestos.bin") ||
        header.cantidadRegistros != 0 ||
        !revisar(leerHeaderOrdenes(header), "leer ordenes.bin") ||
        header.cantidadRegistros != 0 ||
        !revisar(leerHeaderDetalles(header), "leer detalles.bin") ||
        header.cantidadRegistros != 0) {
        cerr << "El generador requiere archivos vacios para no duplicar datos." << endl;
        return 1;
    }

    Taller taller = {};
    copiarTexto(taller.nombre, sizeof(taller.nombre), "Taller Demo");
    copiarTexto(taller.rif, sizeof(taller.rif), "J-12345678-9");
    copiarTexto(taller.direccion, sizeof(taller.direccion), "Maracaibo, Venezuela");
    copiarTexto(taller.telefono, sizeof(taller.telefono), "0414-555-0100");
    taller.fechaCreacion = time(nullptr);
    taller.fechaUltimaModificacion = taller.fechaCreacion;
    if (!revisar(guardarTaller(taller), "guardar taller.bin")) return 1;

    SistemaTaller sistema = {};
    inicializarSistema(sistema, taller);

    for (int i = 0; i < 8; ++i) {
        Vehiculo vehiculo = {};
        char placa[10];
        char nombre[32];
        char cedula[20];
        sprintf(placa, "DEMO%03d", i + 1);
        sprintf(nombre, "Propietario Demo %02d", i + 1);
        sprintf(cedula, "V-%08d", 30000000 + i);
        copiarTexto(vehiculo.placa, sizeof(vehiculo.placa), placa);
        copiarTexto(vehiculo.marca, sizeof(vehiculo.marca), "Toyota");
        sprintf(vehiculo.modelo, "Modelo %02d", i + 1);
        vehiculo.anio = 2018 + i;
        vehiculo.kilometraje = 15000 + (i + 1) * 500;
        copiarTexto(vehiculo.nombrePropietario, sizeof(vehiculo.nombrePropietario), nombre);
        copiarTexto(vehiculo.cedulaPropietario, sizeof(vehiculo.cedulaPropietario), cedula);
        copiarTexto(vehiculo.telefonoPropietario, sizeof(vehiculo.telefonoPropietario), "0414-555-0100");
        if (!revisar(agregarVehiculo(vehiculo), "registrar un vehiculo")) return 1;
    }

    const char* especialidades[] = {"MOTOR", "FRENOS", "ELECTRICIDAD", "SUSPENSION"};
    for (int i = 0; i < 4; ++i) {
        Mecanico mecanico = {};
        char nombre[32];
        char cedula[20];
        sprintf(nombre, "Mecanico Demo %02d", i + 1);
        sprintf(cedula, "V-%08d", 40000000 + i);
        copiarTexto(mecanico.nombre, sizeof(mecanico.nombre), nombre);
        copiarTexto(mecanico.cedula, sizeof(mecanico.cedula), cedula);
        copiarTexto(mecanico.especialidad, sizeof(mecanico.especialidad), especialidades[i]);
        if (!revisar(agregarMecanico(mecanico), "registrar un mecanico")) return 1;
    }

    for (int i = 0; i < 15; ++i) {
        Repuesto repuesto = {};
        char codigo[20];
        char nombre[32];
        sprintf(codigo, "REP-%03d", i + 1);
        sprintf(nombre, "Repuesto Demo %02d", i + 1);
        copiarTexto(repuesto.codigo, sizeof(repuesto.codigo), codigo);
        copiarTexto(repuesto.nombre, sizeof(repuesto.nombre), nombre);
        copiarTexto(repuesto.marca, sizeof(repuesto.marca), "Marca Demo");
        repuesto.precio = 20.0f + static_cast<float>(i);
        repuesto.stock = 100;
        repuesto.stockMinimo = 5;
        if (!revisar(agregarRepuesto(repuesto), "registrar un repuesto")) return 1;
    }

    const int linea15[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    const int linea4a[] = {1, 2, 3, 4};
    const int linea4b[] = {5, 6, 7, 8};
    const int linea4c[] = {9, 10, 11, 12};
    const int linea2a[] = {1, 5};
    const int linea2b[] = {2, 6};
    const int linea1[] = {13};
    int idOrden = 0;
    if (!crearOrden(sistema, 1, 1, linea15, 15, "Cerrada", idOrden) ||
        !crearOrden(sistema, 2, 2, linea4a, 4, "Cerrada", idOrden) ||
        !crearOrden(sistema, 3, 3, linea4b, 4, "Cerrada", idOrden) ||
        !crearOrden(sistema, 4, 1, linea4c, 4, "Cerrada", idOrden) ||
        !crearOrden(sistema, 5, 2, linea2a, 2, "Cerrada", idOrden) ||
        !crearOrden(sistema, 6, 3, linea2b, 2, "Cerrada", idOrden) ||
        !crearOrden(sistema, 7, 1, linea1, 1, "Abierta", idOrden) ||
        !crearOrden(sistema, 8, 2, linea1, 1, "Abierta", idOrden) ||
        !crearOrden(sistema, 1, 3, linea1, 1, "Anulada", idOrden) ||
        !crearOrden(sistema, 2, 1, linea1, 1, "Anulada", idOrden)) {
        return 1;
    }

    if (!revisar(eliminarMecanico(&sistema, 4), "dar de baja un mecanico")) return 1;
    cout << "Datos demo creados: 8 vehiculos, 4 mecanicos (1 dado de baja), "
         << "15 repuestos y 10 ordenes (6 cerradas, 2 abiertas, 2 anuladas)." << endl;
    cout << "La orden 1 contiene 15 lineas de repuestos." << endl;
    return 0;
}
