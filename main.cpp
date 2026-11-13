#include <iostream>
#include <cstring>
#include <iomanip>
#include <cstdlib>
#include <cstdio>
#include <limits>
#include <ctime>
#include <cctype>
#include <cmath>
#include <fstream>

using namespace std;

//Validaciones
enum class ResultadoEntrada {
    Correcto,
    Cancelar,
    Atras,
    Reintentar
};

bool entradaConSaltoPendiente = false;

class Validaciones {
public:
    static bool esComando(const char* texto, const char* comando) {
        if (texto == nullptr || comando == nullptr) return false;
        while (*texto != '\0' && *comando != '\0') {
            if (toupper(static_cast<unsigned char>(*texto)) !=
                toupper(static_cast<unsigned char>(*comando))) return false;
            ++texto;
            ++comando;
        }
        return *texto == '\0' && *comando == '\0';
    }

    static ResultadoEntrada leerTexto(const char* mensaje, char* valor, size_t capacidad) {
        char entrada[512];
        while (true) {
            cout << mensaje;
            if (!cin.getline(entrada, sizeof(entrada))) {
                if (cin.eof()) return ResultadoEntrada::Cancelar;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                entradaConSaltoPendiente = false;
                cout << "[!] El texto excede el limite permitido." << endl;
                continue;
            }
            entradaConSaltoPendiente = false;
            if (esComando(entrada, "CANCELAR")) return ResultadoEntrada::Cancelar;
            if (esComando(entrada, "ATRAS")) return ResultadoEntrada::Atras;
            if (strlen(entrada) >= capacidad) {
                cout << "[!] El texto excede el limite permitido." << endl;
                continue;
            }
            if (!textoNoVacio(entrada)) {
                cout << "[!] El campo no puede quedar vacio." << endl;
                continue;
            }
            strcpy(valor, entrada);
            return ResultadoEntrada::Correcto;
        }
    }

    static ResultadoEntrada leerEnteroRango(const char* mensaje, int minimo, int maximo, int& valor) {
        char entrada[64];
        while (true) {
            cout << mensaje;
            if (!cin.getline(entrada, sizeof(entrada))) {
                if (cin.eof()) return ResultadoEntrada::Cancelar;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                entradaConSaltoPendiente = false;
                cout << "[!] El valor excede el limite permitido." << endl;
                continue;
            }
            entradaConSaltoPendiente = false;
            if (esComando(entrada, "CANCELAR")) return ResultadoEntrada::Cancelar;
            if (esComando(entrada, "ATRAS")) return ResultadoEntrada::Atras;
            char* fin = nullptr;
            long numero = strtol(entrada, &fin, 10);
            while (fin != nullptr && isspace(static_cast<unsigned char>(*fin))) ++fin;
            if (fin != entrada && fin != nullptr && *fin == '\0' &&
                numero >= minimo && numero <= maximo) {
                valor = static_cast<int>(numero);
                return ResultadoEntrada::Correcto;
            }
            cout << "[!] Ingrese un numero entero entre " << minimo << " y " << maximo << "." << endl;
        }
    }

    static ResultadoEntrada leerDecimalRango(const char* mensaje, double minimo, double maximo, double& valor) {
        char entrada[64];
        while (true) {
            cout << mensaje;
            if (!cin.getline(entrada, sizeof(entrada))) {
                if (cin.eof()) return ResultadoEntrada::Cancelar;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                entradaConSaltoPendiente = false;
                cout << "[!] El valor excede el limite permitido." << endl;
                continue;
            }
            entradaConSaltoPendiente = false;
            if (esComando(entrada, "CANCELAR")) return ResultadoEntrada::Cancelar;
            if (esComando(entrada, "ATRAS")) return ResultadoEntrada::Atras;
            char* fin = nullptr;
            double numero = strtod(entrada, &fin);
            while (fin != nullptr && isspace(static_cast<unsigned char>(*fin))) ++fin;
            if (fin != entrada && fin != nullptr && *fin == '\0' &&
                isfinite(numero) && numero >= minimo && numero <= maximo) {
                valor = numero;
                return ResultadoEntrada::Correcto;
            }
            cout << "[!] Ingrese un numero entre " << minimo << " y " << maximo << "." << endl;
        }
    }

    static bool procesarResultado(ResultadoEntrada resultado, int& campo) {
        if (resultado == ResultadoEntrada::Cancelar) return false;
        if (resultado == ResultadoEntrada::Atras) {
            if (campo == 0) return false;
            --campo;
        } else if (resultado == ResultadoEntrada::Correcto) {
            ++campo;
        }
        return true;
    }

    static bool textoNoVacio(const char* texto) {
        if (texto == nullptr) return false;
        for (size_t i = 0; texto[i] != '\0'; ++i) {
            if (iscntrl(static_cast<unsigned char>(texto[i]))) return false;
            if (!isspace(static_cast<unsigned char>(texto[i]))) return true;
        }
        return false;
    }

    static bool placaValida(const char* placa) {
        if (placa == nullptr) return false;
        size_t largo = strlen(placa);
        if (largo < 6 || largo > 7) return false;
        for (size_t i = 0; i < largo; ++i) {
            if (!isalnum(static_cast<unsigned char>(placa[i]))) return false;
        }
        return true;
    }

    static bool telefonoValido(const char* telefono) {
        if (telefono == nullptr) return false;
        int digitos = 0;
        for (size_t i = 0; telefono[i] != '\0'; ++i) {
            unsigned char caracter = static_cast<unsigned char>(telefono[i]);
            if (isdigit(caracter)) {
                ++digitos;
            } else if (telefono[i] != '+' && telefono[i] != '-' &&
                       telefono[i] != '(' && telefono[i] != ')' &&
                       !isspace(caracter)) {
                return false;
            }
        }
        return digitos >= 7 && digitos <= 15;
    }

    static bool cedulaValida(const char* cedula) {
        if (cedula == nullptr) return false;
        const char* digitos = cedula;
        char prefijo = static_cast<char>(toupper(static_cast<unsigned char>(cedula[0])));
        if ((prefijo == 'V' || prefijo == 'E') && cedula[1] == '-') digitos += 2;
        if (*digitos == '\0') return false;
        for (const char* p = digitos; *p != '\0'; ++p) {
            if (!isdigit(static_cast<unsigned char>(*p))) return false;
        }
        return true;
    }

    static bool fechaValida(const char* fecha) {
        if (fecha == nullptr || strlen(fecha) != 10 ||
            fecha[4] != '-' || fecha[7] != '-') return false;
        for (int i = 0; i < 10; ++i) {
            if (i != 4 && i != 7 && !isdigit(static_cast<unsigned char>(fecha[i]))) return false;
        }
        int anio = atoi(fecha);
        int mes = atoi(fecha + 5);
        int dia = atoi(fecha + 8);
        if (mes < 1 || mes > 12) return false;
        const int diasMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        int maxDia = diasMes[mes - 1];
        bool bisiesto = (anio % 4 == 0 && anio % 100 != 0) || anio % 400 == 0;
        if (mes == 2 && bisiesto) maxDia = 29;
        return dia >= 1 && dia <= maxDia;
    }

    static bool contieneSinMayusculas(const char* texto, const char* fragmento) {
        if (texto == nullptr || fragmento == nullptr) return false;
        size_t largoTexto = strlen(texto);
        size_t largoFragmento = strlen(fragmento);
        if (largoFragmento == 0) return true;
        if (largoFragmento > largoTexto) return false;
        for (size_t inicio = 0; inicio + largoFragmento <= largoTexto; ++inicio) {
            size_t j = 0;
            while (j < largoFragmento &&
                   toupper(static_cast<unsigned char>(texto[inicio + j])) ==
                   toupper(static_cast<unsigned char>(fragmento[j]))) ++j;
            if (j == largoFragmento) return true;
        }
        return false;
    }

    static bool igualSinMayusculas(const char* a, const char* b) {
        return esComando(a, b);
    }

    static void convertirAMayusculas(char* texto) {
        if (texto == nullptr) return;
        for (size_t i = 0; texto[i] != '\0'; ++i) {
            texto[i] = static_cast<char>(toupper(static_cast<unsigned char>(texto[i])));
        }
    }
};

void limpiarTerminal() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausarParaContinuar() {
    cout << "\nPresione Enter para volver al menu...";
    if (entradaConSaltoPendiente) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        entradaConSaltoPendiente = false;
    }
    cin.get();
}

//ESTRUCTURAS DE DATOS
struct ArchivoHeader {
    int cantidadRegistros;
    int proximoID;
    int registrosActivos;
    int version;
};

struct Taller {
    char nombre[100]; 
    char rif[20];
    char direccion[200];
    char telefono[20];
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;
};

struct Vehiculo {
    int id;
    char placa[10] = {};
    char marca [50];
    char modelo[50];
    int anio;
    int kilometraje;
    char nombrePropietario[100];
    char cedulaPropietario[20];
    char telefonoPropietario[20];
    char fechaRegistro[11];

    bool eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;
};

struct Mecanico {
    int id;
    char nombre[100];
    char cedula[20];
    char especialidad[20];
    char fechaIngreso[11];

    bool eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;
};

struct Repuesto {
    int id;
    char codigo[20];
    char nombre[100];
    char marca[50];
    float precio;
    int stock;
    int stockMinimo;
    char fechaRegistro[11];

    bool eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;
};

struct DetalleOrden {
    int idRepuesto;
    char codigoRepuesto[20];
    char nombreRepuesto[100];
    int cantidad;
    float precioUnitario;
};

struct OrdenServicio {
    int id;
    int idVehiculo;
    int idMecanico;
    char fechaApertura[11];
    char fechaCierre[11];
    char estado[12];
    char descripcionFalla[200];
    int kilometrajeIngreso;
    float manoObra;
    
    DetalleOrden lineasDetalle[15];
    int cantidadLineasUsadas;

    bool eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;
};

struct SistemaTaller {
    Taller taller;

    Vehiculo*      vehiculos; int numVehiculos; int capacidadVehiculos;
    Mecanico*      mecanicos; int numMecanicos; int capacidadMecanicos;
    Repuesto*      repuestos; int numRepuestos; int capacidadRepuestos;
    OrdenServicio* ordenes;   int numOrdenes;   int capacidadOrdenes;

    int siguienteIdVehiculos;
    int siguienteIdMecanicos;
    int siguienteIdRepuestos;
    int siguienteIdOrdenes;
};

// PERSISTENCIA ARCHIVOS BIN 
const char* ARCHIVO_VEHICULOS = "vehiculos.bin";
const char* ARCHIVO_MECANICOS = "mecanicos.bin";
const char* ARCHIVO_REPUESTOS = "repuestos.bin";
const char* ARCHIVO_ORDENES   = "ordenes.bin";
const int   VERSION_ARCHIVO   = 1;

void guardarDatos(SistemaTaller& s) {
    {
        std::ofstream archivo(ARCHIVO_VEHICULOS, ios::out | ios::binary);
        if (archivo.is_open()) {
            ArchivoHeader header = { s.numVehiculos, s.siguienteIdVehiculos, s.numVehiculos, VERSION_ARCHIVO };
            archivo.write(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
            if (s.numVehiculos > 0) {
                archivo.write(reinterpret_cast<char*>(s.vehiculos), sizeof(Vehiculo) * s.numVehiculos);
            }
            archivo.close();
        }
    }

    {
        std::ofstream archivo(ARCHIVO_MECANICOS, ios::out | ios::binary);
        if (archivo.is_open()) {
            ArchivoHeader header = { s.numMecanicos, s.siguienteIdMecanicos, s.numMecanicos, VERSION_ARCHIVO };
            archivo.write(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
            if (s.numMecanicos > 0) {
                archivo.write(reinterpret_cast<char*>(s.mecanicos), sizeof(Mecanico) * s.numMecanicos);
            }
            archivo.close();
        }
    }

    {
        std::ofstream archivo(ARCHIVO_REPUESTOS, ios::out | ios::binary);
        if (archivo.is_open()) {
            ArchivoHeader header = { s.numRepuestos, s.siguienteIdRepuestos, s.numRepuestos, VERSION_ARCHIVO };
            archivo.write(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
            if (s.numRepuestos > 0) {
                archivo.write(reinterpret_cast<char*>(s.repuestos), sizeof(Repuesto) * s.numRepuestos);
            }
            archivo.close();
        }
    }

    {
        std::ofstream archivo(ARCHIVO_ORDENES, ios::out | ios::binary);
        if (archivo.is_open()) {
            ArchivoHeader header = { s.numOrdenes, s.siguienteIdOrdenes, s.numOrdenes, VERSION_ARCHIVO };
            archivo.write(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
            if (s.numOrdenes > 0) {
                archivo.write(reinterpret_cast<char*>(s.ordenes), sizeof(OrdenServicio) * s.numOrdenes);
            }
            archivo.close();
        }
    }
}

void cargarDatos(SistemaTaller& s) {
    {
        std::ifstream archivo(ARCHIVO_VEHICULOS, ios::in | ios::binary);
        if (archivo.is_open()) {
            ArchivoHeader header;
            archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
            if (!archivo.fail()) {
                s.siguienteIdVehiculos = header.proximoID;
                if (header.cantidadRegistros > 0) {
                    if (header.cantidadRegistros > s.capacidadVehiculos) {
                        delete[] s.vehiculos;
                        s.capacidadVehiculos = header.cantidadRegistros * 2;
                        s.vehiculos = new Vehiculo[s.capacidadVehiculos];
                    }
                    archivo.read(reinterpret_cast<char*>(s.vehiculos), sizeof(Vehiculo) * header.cantidadRegistros);
                    s.numVehiculos = header.cantidadRegistros;
                }
            }
            archivo.close();
        }
    }

    {
        std::ifstream archivo(ARCHIVO_MECANICOS, ios::in | ios::binary);
        if (archivo.is_open()) {
            ArchivoHeader header;
            archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
            if (!archivo.fail()) {
                s.siguienteIdMecanicos = header.proximoID;
                if (header.cantidadRegistros > 0) {
                    if (header.cantidadRegistros > s.capacidadMecanicos) {
                        delete[] s.mecanicos;
                        s.capacidadMecanicos = header.cantidadRegistros * 2;
                        s.mecanicos = new Mecanico[s.capacidadMecanicos];
                    }
                    archivo.read(reinterpret_cast<char*>(s.mecanicos), sizeof(Mecanico) * header.cantidadRegistros);
                    s.numMecanicos = header.cantidadRegistros;
                }
            }
            archivo.close();
        }
    }

    {
        std::ifstream archivo(ARCHIVO_REPUESTOS, ios::in | ios::binary);
        if (archivo.is_open()) {
            ArchivoHeader header;
            archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
            if (!archivo.fail()) {
                s.siguienteIdRepuestos = header.proximoID;
                if (header.cantidadRegistros > 0) {
                    if (header.cantidadRegistros > s.capacidadRepuestos) {
                        delete[] s.repuestos;
                        s.capacidadRepuestos = header.cantidadRegistros * 2;
                        s.repuestos = new Repuesto[s.capacidadRepuestos];
                    }
                    archivo.read(reinterpret_cast<char*>(s.repuestos), sizeof(Repuesto) * header.cantidadRegistros);
                    s.numRepuestos = header.cantidadRegistros;
                }
            }
            archivo.close();
        }
    }

    {
        std::ifstream archivo(ARCHIVO_ORDENES, ios::in | ios::binary);
        if (archivo.is_open()) {
            ArchivoHeader header;
            archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
            if (!archivo.fail()) {
                s.siguienteIdOrdenes = header.proximoID;
                if (header.cantidadRegistros > 0) {
                    if (header.cantidadRegistros > s.capacidadOrdenes) {
                        delete[] s.ordenes;
                        s.capacidadOrdenes = header.cantidadRegistros * 2;
                        s.ordenes = new OrdenServicio[s.capacidadOrdenes];
                    }
                    archivo.read(reinterpret_cast<char*>(s.ordenes), sizeof(OrdenServicio) * header.cantidadRegistros);
                    s.numOrdenes = header.cantidadRegistros;
                }
            }
            archivo.close();
        }
    }
}

// Prototipos de funciones
Vehiculo* buscarVehiculoPorID(SistemaTaller* s, int id);
Vehiculo* buscarVehiculoPorPlaca(SistemaTaller* s, const char* placa);
Vehiculo** listarVehiculos(SistemaTaller* s, int* cantidad);
void mostrarListaVehiculos(Vehiculo** vehiculos, int cantidad);
void mostrarMecanicoUnico(Mecanico* m);
void mostrarListaMecanico(SistemaTaller* s);
Repuesto* buscarRepuestoPorID(SistemaTaller* s, int id);
Repuesto* buscarRepuestoPorCodigo(SistemaTaller* s, const char* codigo);
Repuesto* buscarRepuestoPorNombreParcial(SistemaTaller* s, const char* subcadena);
void mostrarRepuesto(Repuesto* repuesto);
void mostrarListaRepuestos(Repuesto** repuestos, int cantidad);
Mecanico* buscarMecanicoporCedula(SistemaTaller* s, const char* cedula);
Mecanico* buscarMecanicoporID(SistemaTaller* s, int id);
int contarOrdenesAbiertasMecanico(SistemaTaller* s, int idMecanico);
OrdenServicio* buscarOrdenPorID(SistemaTaller* s, int id);
float calcularTotalOrden(const OrdenServicio* orden);
void mostrarDetalleOrden(SistemaTaller* s, OrdenServicio* orden);
void mostrarListaOrdenes(SistemaTaller* s, OrdenServicio** ordenes, int cantidad);

bool fechaValida(const char* fecha);
void obtenerFechaActual(char* fecha, size_t capacidad);
bool cedulaValida(const char* cedula);
bool confirmarAccion(const char* mensaje);
bool esCancelar(const char* texto);
bool contieneSinMayusculas(const char* texto, const char* fragmento);

bool leerTexto(const char* mensaje, char* valor, size_t capacidad) {
    return Validaciones::leerTexto(mensaje, valor, capacidad) == ResultadoEntrada::Correcto;
}

bool leerEnteroRango(const char* mensaje, int minimo, int maximo, int& valor) {
    return Validaciones::leerEnteroRango(mensaje, minimo, maximo, valor) == ResultadoEntrada::Correcto;
}

bool leerDecimalRango(const char* mensaje, double minimo, double maximo, double& valor) {
    return Validaciones::leerDecimalRango(mensaje, minimo, maximo, valor) == ResultadoEntrada::Correcto;
}

void mostrarOpcionVolver() {
    cout << "Escriba CANCELAR para volver al menu o ATRAS para regresar al campo anterior." << endl;
}

//SISTEMA GENERAL 
void inicializarSistema(SistemaTaller &s, Taller infoTaller) {
    s.taller = infoTaller;

    s.capacidadVehiculos = 4;
    s.capacidadRepuestos = 4;
    s.capacidadMecanicos = 4;
    s.capacidadOrdenes = 4;

    s.vehiculos = new Vehiculo[s.capacidadVehiculos];
    s.repuestos = new Repuesto[s.capacidadRepuestos];
    s.mecanicos = new Mecanico[s.capacidadMecanicos];
    s.ordenes = new OrdenServicio[s.capacidadOrdenes];

    s.numVehiculos = 0;
    s.numRepuestos = 0;
    s.numMecanicos = 0;
    s.numOrdenes = 0;

    s.siguienteIdMecanicos = 1;
    s.siguienteIdOrdenes = 1;
    s.siguienteIdRepuestos = 1;
    s.siguienteIdVehiculos = 1;
}

void liberarSistema(SistemaTaller &s) {
    delete[] s.vehiculos;
    delete[] s.repuestos;
    delete[] s.mecanicos;
    delete[] s.ordenes; 
}

bool cedulaValida(const char* cedula) {
    return Validaciones::cedulaValida(cedula);
}

bool fechaValida(const char* fecha) {
    return Validaciones::fechaValida(fecha);
}

void obtenerFechaActual(char* fecha, size_t capacidad) {
    time_t ahora = time(nullptr);
    tm* fechaLocal = localtime(&ahora);
    if (fechaLocal == nullptr || capacidad < 11) {
        if (capacidad > 0) fecha[0] = '\0';
        return;
    }
    strftime(fecha, capacidad, "%Y-%m-%d", fechaLocal);
}

bool confirmarAccion(const char* mensaje) {
    char respuesta;
    while (true) {
        cout << mensaje << " (S/N): ";
        if (!(cin >> respuesta)) {
            if (cin.eof()) return false;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            entradaConSaltoPendiente = false;
            continue;
        }
        entradaConSaltoPendiente = true;
        respuesta = static_cast<char>(toupper(static_cast<unsigned char>(respuesta)));
        if (respuesta == 'S') return true;
        if (respuesta == 'N') return false;
        cout << "[!] Responda S o N." << endl;
    }
}

bool esCancelar(const char* texto) {
    return Validaciones::esComando(texto, "CANCELAR");
}

bool contieneSinMayusculas(const char* texto, const char* fragmento) {
    return Validaciones::contieneSinMayusculas(texto, fragmento);
}

bool cadenaTerminada(const char* texto, size_t capacidad) {
    return texto != nullptr && memchr(texto, '\0', capacidad) != nullptr;
}

bool textoNoVacio(const char* texto) {
    return Validaciones::textoNoVacio(texto);
}

bool placaValida(const char* placa) {
    return Validaciones::placaValida(placa);
}

bool telefonoValido(const char* telefono) {
    return Validaciones::telefonoValido(telefono);
}

bool igualSinMayusculas(const char* a, const char* b) {
    return Validaciones::igualSinMayusculas(a, b);
}

//SISTEMA DE VEHICULOS 
void redimensionarVehiculos(SistemaTaller* s) {
    s->capacidadVehiculos *= 2; 
    Vehiculo* nuevosVehiculos = new Vehiculo[s->capacidadVehiculos];

    for (int i = 0; i < s->numVehiculos; i++) {
        nuevosVehiculos[i] = s->vehiculos[i];
    }

    delete[] s->vehiculos;
    s->vehiculos = nuevosVehiculos;
}

bool datosVehiculoValidos(SistemaTaller* s, const Vehiculo& datos, int idIgnorado) {
    if (!cadenaTerminada(datos.placa, sizeof(datos.placa)) ||
        !cadenaTerminada(datos.marca, sizeof(datos.marca)) ||
        !cadenaTerminada(datos.modelo, sizeof(datos.modelo)) ||
        !cadenaTerminada(datos.nombrePropietario, sizeof(datos.nombrePropietario)) ||
        !cadenaTerminada(datos.cedulaPropietario, sizeof(datos.cedulaPropietario)) ||
        !cadenaTerminada(datos.telefonoPropietario, sizeof(datos.telefonoPropietario)) ||
        !placaValida(datos.placa) || !textoNoVacio(datos.marca) ||
        !textoNoVacio(datos.modelo) || datos.anio < 1950 || datos.anio > 2027 ||
        datos.kilometraje < 0 || !textoNoVacio(datos.nombrePropietario) ||
        !cedulaValida(datos.cedulaPropietario) ||
        !telefonoValido(datos.telefonoPropietario)) return false;

    for (int i = 0; i < s->numVehiculos; ++i) {
        if (s->vehiculos[i].id != idIgnorado &&
            igualSinMayusculas(s->vehiculos[i].placa, datos.placa)) return false;
    }
    return true;
}

bool capturarDatosVehiculo(SistemaTaller* s, Vehiculo& datos, int idIgnorado,
                           int kilometrajeMinimo) {
    int campo = 0;
    while (campo < 8) {
        ResultadoEntrada resultado;
        switch (campo) {
            case 0:
                resultado = Validaciones::leerTexto(
                    "Placa (6-7 caracteres alfanumericos): ", datos.placa, sizeof(datos.placa));
                if (resultado == ResultadoEntrada::Correcto) {
                    if (!placaValida(datos.placa)) {
                        cout << "[!] La placa debe tener 6 o 7 caracteres alfanumericos." << endl;
                        resultado = ResultadoEntrada::Reintentar;
                    } else {
                        Vehiculo* existente = buscarVehiculoPorPlaca(s, datos.placa);
                        if (existente != nullptr && existente->id != idIgnorado) {
                            cout << "[!] Esa placa ya esta registrada." << endl;
                            resultado = ResultadoEntrada::Reintentar;
                        } else {
                            Validaciones::convertirAMayusculas(datos.placa);
                        }
                    }
                }
                break;
            case 1:
                resultado = Validaciones::leerTexto("Marca: ", datos.marca, sizeof(datos.marca));
                break;
            case 2:
                resultado = Validaciones::leerTexto("Modelo: ", datos.modelo, sizeof(datos.modelo));
                break;
            case 3:
                resultado = Validaciones::leerEnteroRango("Anio (1950-2027): ", 1950, 2027, datos.anio);
                break;
            case 4:
                resultado = Validaciones::leerEnteroRango(
                    "Kilometraje (no menor al actual): ", kilometrajeMinimo,
                    numeric_limits<int>::max(), datos.kilometraje);
                break;
            case 5:
                resultado = Validaciones::leerTexto(
                    "Nombre del propietario: ", datos.nombrePropietario, sizeof(datos.nombrePropietario));
                break;
            case 6:
                resultado = Validaciones::leerTexto(
                    "Cedula del propietario (digitos, V- o E- opcional): ",
                    datos.cedulaPropietario, sizeof(datos.cedulaPropietario));
                if (resultado == ResultadoEntrada::Correcto && !cedulaValida(datos.cedulaPropietario)) {
                    cout << "[!] Use solo digitos, opcionalmente precedidos por V- o E-." << endl;
                    resultado = ResultadoEntrada::Reintentar;
                }
                if (resultado == ResultadoEntrada::Correcto &&
                    (datos.cedulaPropietario[0] == 'v' || datos.cedulaPropietario[0] == 'e')) {
                    datos.cedulaPropietario[0] =
                        static_cast<char>(toupper(static_cast<unsigned char>(datos.cedulaPropietario[0])));
                }
                break;
            default:
                resultado = Validaciones::leerTexto(
                    "Telefono del propietario (7-15 digitos): ",
                    datos.telefonoPropietario, sizeof(datos.telefonoPropietario));
                if (resultado == ResultadoEntrada::Correcto && !telefonoValido(datos.telefonoPropietario)) {
                    cout << "[!] Ingrese un telefono con 7-15 digitos; puede usar +, espacios, guiones o parentesis." << endl;
                    resultado = ResultadoEntrada::Reintentar;
                }
                break;
        }
        if (!Validaciones::procesarResultado(resultado, campo)) return false;
    }
    return true;
}

Vehiculo* agregarVehiculo(SistemaTaller* s, Vehiculo datos) {
    if (!datosVehiculoValidos(s, datos, -1)) return nullptr;
    for (size_t i = 0; datos.placa[i] != '\0'; ++i)
        datos.placa[i] = static_cast<char>(toupper(static_cast<unsigned char>(datos.placa[i])));
    if (datos.cedulaPropietario[0] == 'v' || datos.cedulaPropietario[0] == 'e')
        datos.cedulaPropietario[0] = static_cast<char>(toupper(static_cast<unsigned char>(datos.cedulaPropietario[0])));
    if (datos.fechaRegistro[0] == '\0') obtenerFechaActual(datos.fechaRegistro, sizeof(datos.fechaRegistro));
    if (s->numVehiculos >= s->capacidadVehiculos) {
        redimensionarVehiculos(s);
    }

    datos.id = s->siguienteIdVehiculos++;
    s->vehiculos[s->numVehiculos] = datos;
    s->numVehiculos++;

    return &(s->vehiculos[s->numVehiculos - 1]);
}

Vehiculo* buscarVehiculoPorID(SistemaTaller* s, int id) {
    for (int i = 0; i < s->numVehiculos; i++) {
        if (s->vehiculos[i].id == id) {
            return &(s->vehiculos[i]);
        }
    }
    return nullptr;
}

Vehiculo* buscarVehiculoPorPlaca(SistemaTaller* s, const char* placa){
    for (int i = 0; i < s->numVehiculos; i++) {
        if (igualSinMayusculas(s->vehiculos[i].placa, placa)) {
            return &(s->vehiculos[i]);
        }
    }
    return nullptr;
}

Vehiculo** buscarVehiculosPorPropietario(SistemaTaller* s, const char* subcadena, int* cantidad) {
    int encontrados = 0;
    for (int i = 0; i < s->numVehiculos; i++) {
        if (contieneSinMayusculas(s->vehiculos[i].nombrePropietario, subcadena)) {
            encontrados++;
        }
    }
    *cantidad = encontrados;
    if (encontrados == 0) return nullptr;

    Vehiculo** lista = new Vehiculo*[encontrados];
    int j = 0;
    for (int i = 0; i < s->numVehiculos; i++) {
        if (contieneSinMayusculas(s->vehiculos[i].nombrePropietario, subcadena)) {
            lista[j++] = &(s->vehiculos[i]);
        }
    }
    return lista;
}

Vehiculo** listarVehiculos(SistemaTaller* s, int* cantidad) {
    *cantidad = s->numVehiculos;
    if (s->numVehiculos == 0) return nullptr;

    Vehiculo** lista = new Vehiculo*[s->numVehiculos];
    for (int i = 0; i < s->numVehiculos; i++) {
        lista[i] = &(s->vehiculos[i]);
    }
    return lista;
}

bool actualizarVehiculo(SistemaTaller* s, int id, Vehiculo datos) {
    Vehiculo* v = buscarVehiculoPorID(s, id);
    if (v == nullptr || !datosVehiculoValidos(s, datos, id) ||
        datos.kilometraje < v->kilometraje) return false;

    for (size_t i = 0; datos.placa[i] != '\0'; ++i)
        datos.placa[i] = static_cast<char>(toupper(static_cast<unsigned char>(datos.placa[i])));
    strcpy(v->placa, datos.placa);
    strcpy(v->marca, datos.marca);
    strcpy(v->modelo, datos.modelo);
    v->anio = datos.anio;
    v->kilometraje = datos.kilometraje;
    strcpy(v->nombrePropietario, datos.nombrePropietario);
    strcpy(v->cedulaPropietario, datos.cedulaPropietario);
    if (v->cedulaPropietario[0] == 'v' || v->cedulaPropietario[0] == 'e')
        v->cedulaPropietario[0] = static_cast<char>(toupper(static_cast<unsigned char>(v->cedulaPropietario[0])));
    strcpy(v->telefonoPropietario, datos.telefonoPropietario);
    return true;
}

bool eliminarVehiculo(SistemaTaller* s, int id) {
    int pos = -1;
    for (int i = 0; i < s->numVehiculos; i++) {
        if (s->vehiculos[i].id == id) {
            pos = i;
            break;
        }
    }

    if (pos == -1) return false;
    for (int i = 0; i < s->numOrdenes; ++i) {
        if (s->ordenes[i].idVehiculo == id) return false;
    }

    for (int i = pos; i < s->numVehiculos - 1; i++) {
        s->vehiculos[i] = s->vehiculos[i + 1];
    }
    
    s->numVehiculos--;
    return true;
}

void mostrarVehiculo(Vehiculo* vehiculo) {
    if (vehiculo == nullptr) {
        cout << "[!] Vehiculo no encontrado." << endl;
        return;
    }

    cout << "\n--- DETALLE DEL VEHICULO ---" << endl;
    cout << "ID: " << vehiculo->id << endl;
    cout << "Placa: " << vehiculo->placa << endl;
    cout << "Marca: " << vehiculo->marca << endl;
    cout << "Modelo: " << vehiculo->modelo << endl;
    cout << "Anio: " << vehiculo->anio << endl;
    cout << "Kilometraje: " << vehiculo->kilometraje << endl;
    cout << "Fecha de registro: " << vehiculo->fechaRegistro << endl;
    cout << "Propietario: " << vehiculo->nombrePropietario << endl;
    cout << "Cedula del propietario: " << vehiculo->cedulaPropietario << endl;
    cout << "Telefono del propietario: " << vehiculo->telefonoPropietario << endl;
}

void menuRegistrarVehiculo(SistemaTaller* s) {
    limpiarTerminal();
    Vehiculo nuevo = {};
    cout << "\n--- REGISTRAR VEHICULO ---" << endl;
    mostrarOpcionVolver();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!capturarDatosVehiculo(s, nuevo, -1, 0)) return;
    if (!confirmarAccion("Guardar vehiculo")) return;
    Vehiculo* creado = agregarVehiculo(s, nuevo);
    if (creado == nullptr) {
        cout << "[!] No se pudo registrar el vehiculo; revise sus datos." << endl;
        return;
    }
    cout << "[OK] Vehiculo registrado con ID " << creado->id << "." << endl;
}

void menuBuscarVehiculo(SistemaTaller* s) {
    limpiarTerminal();
    int opcion = 0;
    cout << "\n--- BUSCAR VEHICULO ---\n1. Por ID\n2. Por placa\n3. Por propietario\n";
    mostrarOpcionVolver();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    int campo = 0;
    while (campo < 2) {
        ResultadoEntrada resultado;
        if (campo == 0) {
            resultado = Validaciones::leerEnteroRango("Opcion (1-3): ", 1, 3, opcion);
        } else {
            if (opcion == 1) {
                int id = 0;
                resultado = Validaciones::leerEnteroRango(
                    "ID: ", 1, numeric_limits<int>::max(), id);
                if (resultado == ResultadoEntrada::Correcto) {
                    mostrarVehiculo(buscarVehiculoPorID(s, id));
                }
            } else if (opcion == 2) {
                char placa[10] = {};
                resultado = Validaciones::leerTexto("Placa: ", placa, sizeof(placa));
                if (resultado == ResultadoEntrada::Correcto) {
                    mostrarVehiculo(buscarVehiculoPorPlaca(s, placa));
                }
            } else {
                char nombre[100] = {};
                resultado = Validaciones::leerTexto("Nombre o fragmento: ", nombre, sizeof(nombre));
                if (resultado == ResultadoEntrada::Correcto) {
                    int cantidad = 0;
                    Vehiculo** lista = buscarVehiculosPorPropietario(s, nombre, &cantidad);
                    mostrarListaVehiculos(lista, cantidad);
                    delete[] lista;
                }
            }
        }
        if (!Validaciones::procesarResultado(resultado, campo)) return;
    }
}

void menuActualizarVehiculo(SistemaTaller* s) {
    limpiarTerminal();
    int id = 0;
    cout << "\n--- ACTUALIZAR VEHICULO ---" << endl;
    mostrarOpcionVolver();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    ResultadoEntrada resultado = Validaciones::leerEnteroRango(
        "Ingrese el ID del vehiculo a actualizar: ", 1, numeric_limits<int>::max(), id);
    if (resultado != ResultadoEntrada::Correcto) return;

    Vehiculo* vExistente = buscarVehiculoPorID(s, id);
    if (vExistente == nullptr) {
        cout << "[!] No se encontro un vehiculo con el ID " << id << endl;
        return;
    }
    Vehiculo datosNuevos = {};
    if (!capturarDatosVehiculo(s, datosNuevos, id, vExistente->kilometraje)) return;

    if (!confirmarAccion("Guardar cambios del vehiculo")) return;
    if (actualizarVehiculo(s, id, datosNuevos)) {
        cout << "[OK] Vehiculo actualizado exitosamente." << endl;
    } else {
        cout << "[!] Error al actualizar." << endl;
    }
}

void menuEliminarVehiculo(SistemaTaller* s) {
    limpiarTerminal();
    int id = 0;
    cout << "\n--- ELIMINAR VEHICULO ---\n";
    mostrarOpcionVolver();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (Validaciones::leerEnteroRango("ID: ", 1, numeric_limits<int>::max(), id) !=
        ResultadoEntrada::Correcto) return;
    if (!confirmarAccion("Eliminar vehiculo")) return;
    cout << (eliminarVehiculo(s, id) ? "[OK] Vehiculo eliminado." :
             "[!] No existe o tiene ordenes asociadas.") << endl;
}

void menuListarVehiculos(SistemaTaller* s) {
    limpiarTerminal();
    int cantidad = 0;
    Vehiculo** lista = listarVehiculos(s, &cantidad);
    mostrarListaVehiculos(lista, cantidad);
    delete[] lista;
    pausarParaContinuar();
}

void mostrarListaVehiculos(Vehiculo** vehiculos, int cantidad) {
    if (vehiculos == nullptr || cantidad == 0) {
        cout << "\n[!] No hay vehiculos registrados para mostrar." << endl;
        return;
    }

    cout << "\n=== LISTA DE VEHICULOS (" << cantidad << ") ===" << endl;
    for (int i = 0; i < cantidad; i++) {
        cout << "ID: " << vehiculos[i]->id
             << " | Placa: " << vehiculos[i]->placa
             << " | Marca: " << vehiculos[i]->marca
             << " | Modelo: " << vehiculos[i]->modelo
             << " | Propietario: " << vehiculos[i]->nombrePropietario << endl;
    }
}

// SISTEMA DE MECANICOS 
void redimensionarMecanicos(SistemaTaller* s) {
    s->capacidadMecanicos *= 2; 
    Mecanico* nuevosMecanicos = new Mecanico[s->capacidadMecanicos];

    for (int i = 0; i < s->numMecanicos; i++) {
        nuevosMecanicos[i] = s->mecanicos[i];
    }

    delete[] s->mecanicos;
    s->mecanicos = nuevosMecanicos;
}

Mecanico* agregarMecanico(SistemaTaller* s, Mecanico datos) {
    Validaciones::convertirAMayusculas(datos.cedula);
    Validaciones::convertirAMayusculas(datos.especialidad);
    const char* especialidades[] = {"MOTOR", "FRENOS", "ELECTRICIDAD", "SUSPENSION", "GENERAL"};
    bool especialidadValida = false;
    for (size_t i = 0; i < sizeof(especialidades) / sizeof(especialidades[0]); ++i) {
        if (strcmp(datos.especialidad, especialidades[i]) == 0) especialidadValida = true;
    }
    if (!cedulaValida(datos.cedula) || buscarMecanicoporCedula(s, datos.cedula) != nullptr ||
        !especialidadValida) return nullptr;
    if (datos.fechaIngreso[0] == '\0') obtenerFechaActual(datos.fechaIngreso, sizeof(datos.fechaIngreso));
    if (s->numMecanicos >= s->capacidadMecanicos) {
        redimensionarMecanicos(s);
    }

    datos.id = s->siguienteIdMecanicos++;
    s->mecanicos[s->numMecanicos] = datos;
    s->numMecanicos++;

    return &(s->mecanicos[s->numMecanicos - 1]);
}

Mecanico* buscarMecanicoporID(SistemaTaller* s, int id) {
    for (int i = 0; i < s->numMecanicos; i++) {
        if (s->mecanicos[i].id == id) {
            return &(s->mecanicos[i]);
        }
    }
    return nullptr;
}

Mecanico* buscarMecanicoporCedula(SistemaTaller* s, const char* cedula) {
    for (int i = 0; i < s->numMecanicos; i++) {
        if (igualSinMayusculas(s->mecanicos[i].cedula, cedula)) {
            return &(s->mecanicos[i]);
        }
    }
    return nullptr;
}

Mecanico* listarMecanico(SistemaTaller* s, int* cantidad) {
    *cantidad = s->numMecanicos;
    if (s->numMecanicos == 0) return nullptr;
    return s->mecanicos;
}

int contarOrdenesAbiertasMecanico(SistemaTaller *s, int idMecanico) {
    int cont = 0;
    for (int i = 0; i < s->numOrdenes; i++) {
        if (s->ordenes[i].idMecanico == idMecanico) {
            if (igualSinMayusculas(s->ordenes[i].estado, "Abierta")) {
                cont++;
            }
        }
    }
    return cont;
}

bool capturarDatosMecanico(SistemaTaller* s, Mecanico& datos) {
    const char* especialidades[] = {"MOTOR", "FRENOS", "ELECTRICIDAD", "SUSPENSION", "GENERAL"};
    int campo = 0;
    while (campo < 3) {
        ResultadoEntrada resultado;
        if (campo == 0) {
            resultado = Validaciones::leerTexto("Nombre completo: ", datos.nombre, sizeof(datos.nombre));
        } else if (campo == 1) {
            resultado = Validaciones::leerTexto("Cedula: ", datos.cedula, sizeof(datos.cedula));
            if (resultado == ResultadoEntrada::Correcto && !cedulaValida(datos.cedula)) {
                cout << "[!] Cedula invalida; use digitos, opcionalmente precedidos por V- o E-." << endl;
                resultado = ResultadoEntrada::Reintentar;
            }
            if (resultado == ResultadoEntrada::Correcto &&
                buscarMecanicoporCedula(s, datos.cedula) != nullptr) {
                cout << "[!] Esa cedula ya esta registrada." << endl;
                resultado = ResultadoEntrada::Reintentar;
            }
            if (resultado == ResultadoEntrada::Correcto) {
                Validaciones::convertirAMayusculas(datos.cedula);
            }
        } else {
            resultado = Validaciones::leerTexto(
                "Especialidad (MOTOR, FRENOS, ELECTRICIDAD, SUSPENSION, GENERAL): ",
                datos.especialidad, sizeof(datos.especialidad));
            if (resultado == ResultadoEntrada::Correcto) {
                Validaciones::convertirAMayusculas(datos.especialidad);
                bool valida = false;
                for (const char* especialidad : especialidades) {
                    if (strcmp(datos.especialidad, especialidad) == 0) valida = true;
                }
                if (!valida) {
                    cout << "[!] Seleccione una de las especialidades indicadas." << endl;
                    resultado = ResultadoEntrada::Reintentar;
                }
            }
        }
        if (!Validaciones::procesarResultado(resultado, campo)) return false;
    }
    return true;
}

void menuRegistrarMecanico(SistemaTaller* s) {
    limpiarTerminal();
    Mecanico nuevoM = {};
    cout << "\n--- REGISTRAR NUEVO MECANICO ---" << endl;
    mostrarOpcionVolver();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!capturarDatosMecanico(s, nuevoM)) return;
    if (!confirmarAccion("Guardar mecanico")) return;
    nuevoM.fechaIngreso[0] = '\0';
    if (agregarMecanico(s, nuevoM) == nullptr) {
        cout << "[!] Cedula duplicada o datos invalidos." << endl;
        return;
    }
    cout << "\n[OK] Mecanico registrado exitosamente." << endl;
}

void menuListarMecanico(SistemaTaller* s) {
    limpiarTerminal();
    mostrarListaMecanico(s);
    pausarParaContinuar();
}

void mostrarMecanicoUnico(Mecanico *m) {
    if (m == nullptr) {
        cout << "[!] Mecanico no encontrado." << endl;
        return;
    }
    cout << "-------------------------------------------" << endl;
    cout << "ID: " << m->id << endl;
    cout << "Nombre: " << m->nombre << endl;
    cout << "Cedula: " << m->cedula << endl;
    cout << "Especialidad: " << m->especialidad << endl;
    cout << "Fecha Ingreso: " << m->fechaIngreso << endl;
    cout << "--------------------------------------------" << endl;
}

void mostrarMecanico(SistemaTaller* s) {
    int id = 0;
    if (!leerEnteroRango("Ingrese el ID del mecanico a mostrar: ",
                         numeric_limits<int>::min(), numeric_limits<int>::max(), id)) return;
    Mecanico* m = buscarMecanicoporID(s, id);
    mostrarMecanicoUnico(m);
}

void mostrarListaMecanico(SistemaTaller* s) {
    int cantidad = 0;
    Mecanico* lista = listarMecanico(s, &cantidad);

    if (lista == nullptr || cantidad == 0) {
        cout << "\n[!] No hay mecanicos registrados para mostrar." << endl;
        return;
    }

    cout << "\n=== LISTA DE MECANCIOS (" << cantidad << ") ===" << endl;
    for (int i = 0; i < cantidad; i++) {
        mostrarMecanicoUnico(&lista[i]);
        cout << "Ordenes abiertas: " << contarOrdenesAbiertasMecanico(s, lista[i].id) << endl;
    }
}

//SISTEMA DE REPUESTOS
void redimensionarRepuestos(SistemaTaller* s) {
    s->capacidadRepuestos *= 2;
    Repuesto* nuevosRepuestos = new Repuesto[s->capacidadRepuestos];

    for (int i = 0; i < s->numRepuestos; i++) {
        nuevosRepuestos[i] = s->repuestos[i];
    }

    delete[] s->repuestos;
    s->repuestos = nuevosRepuestos;
}

Repuesto* agregarRepuesto(SistemaTaller* s, Repuesto datos) {
    Validaciones::convertirAMayusculas(datos.codigo);
    if (datos.precio <= 0.0f || datos.stock < 0 || datos.stockMinimo < 0 ||
        buscarRepuestoPorCodigo(s, datos.codigo) != nullptr) return nullptr;
    if (datos.fechaRegistro[0] == '\0') obtenerFechaActual(datos.fechaRegistro, sizeof(datos.fechaRegistro));
    if (s->numRepuestos >= s->capacidadRepuestos) {
        redimensionarRepuestos(s);
    }

    datos.id = s->siguienteIdRepuestos++;
    s->repuestos[s->numRepuestos] = datos;
    s->numRepuestos++;

    return &(s->repuestos[s->numRepuestos - 1]);
}

Repuesto* buscarRepuestoPorID(SistemaTaller* s, int id) {
    for (int i = 0; i < s->numRepuestos; i++) {
        if (s->repuestos[i].id == id) {
            return &(s->repuestos[i]);
        }
    }
    return nullptr;
}

Repuesto* buscarRepuestoPorCodigo(SistemaTaller* s, const char* codigo) {
    for (int i = 0; i < s->numRepuestos; i++) {
        if (igualSinMayusculas(s->repuestos[i].codigo, codigo)) {
            return &(s->repuestos[i]);
        }
    }
    return nullptr;
}

Repuesto* buscarRepuestoPorNombreParcial(SistemaTaller* s, const char* subcadena) {
    for (int i = 0; i < s->numRepuestos; i++) {
        if (contieneSinMayusculas(s->repuestos[i].nombre, subcadena)) {
            return &(s->repuestos[i]);
        }
    }
    return nullptr;
}

Repuesto* listarRepuesto(SistemaTaller* s, int* cantidad) {
     *cantidad = s->numRepuestos;
    if (s->numRepuestos == 0) return nullptr;
    return s->repuestos;
}

bool menuRegistrarEntradaMercancia(SistemaTaller* s, int idRepuesto, int CantidadAdicional) {
    Repuesto* r = buscarRepuestoPorID(s, idRepuesto);
    if (r == nullptr || CantidadAdicional <= 0 || r->stock > numeric_limits<int>::max() - CantidadAdicional) return false;
    r->stock += CantidadAdicional;
    return true;
}

bool cambiarPrecioRepuesto(SistemaTaller* s, int idRepuesto, float nuevoPrecio) {
    Repuesto* r = buscarRepuestoPorID(s, idRepuesto);
    if (r == nullptr || nuevoPrecio <= 0.0f) return false;
    r->precio = nuevoPrecio;
    return true;
}

bool eliminarRepuesto(SistemaTaller* s, int id) {
    int pos = -1;
    for (int i = 0; i < s->numRepuestos; i++) {
        if (s->repuestos[i].id == id) {
            pos = i;
            break;
        }
    }

    if (pos == -1) return false;
    for (int i = 0; i < s->numOrdenes; ++i) {
        for (int j = 0; j < s->ordenes[i].cantidadLineasUsadas; ++j) {
            if (s->ordenes[i].lineasDetalle[j].idRepuesto == id) return false;
        }
    }

    for (int i = pos; i < s->numRepuestos - 1; i++) {
        s->repuestos[i] = s->repuestos[i + 1];
    }
    
    s->numRepuestos--;
    return true;
}

bool capturarDatosRepuesto(SistemaTaller* s, Repuesto& datos) {
    int campo = 0;
    double precio = 0.0;
    while (campo < 6) {
        ResultadoEntrada resultado;
        switch (campo) {
            case 0:
                resultado = Validaciones::leerTexto("Codigo: ", datos.codigo, sizeof(datos.codigo));
                if (resultado == ResultadoEntrada::Correcto &&
                    buscarRepuestoPorCodigo(s, datos.codigo) != nullptr) {
                    cout << "[!] Ese codigo ya esta registrado." << endl;
                    resultado = ResultadoEntrada::Reintentar;
                }
                if (resultado == ResultadoEntrada::Correcto) {
                    Validaciones::convertirAMayusculas(datos.codigo);
                }
                break;
            case 1:
                resultado = Validaciones::leerTexto("Nombre: ", datos.nombre, sizeof(datos.nombre));
                break;
            case 2:
                resultado = Validaciones::leerTexto("Marca: ", datos.marca, sizeof(datos.marca));
                break;
            case 3:
                resultado = Validaciones::leerDecimalRango(
                    "Precio unitario (> 0): ", numeric_limits<double>::min(),
                    numeric_limits<float>::max(), precio);
                if (resultado == ResultadoEntrada::Correcto) datos.precio = static_cast<float>(precio);
                break;
            case 4:
                resultado = Validaciones::leerEnteroRango(
                    "Stock inicial (0 o mayor): ", 0, numeric_limits<int>::max(), datos.stock);
                break;
            default:
                resultado = Validaciones::leerEnteroRango(
                    "Stock minimo (0 o mayor): ", 0, numeric_limits<int>::max(), datos.stockMinimo);
                break;
        }
        if (!Validaciones::procesarResultado(resultado, campo)) return false;
    }
    return true;
}

void menuRegistrarRepuesto(SistemaTaller* s) {
    limpiarTerminal();
    Repuesto nuevoR = {};
    cout << "\n--- REGISTRAR NUEVO REPUESTO ---" << endl;
    mostrarOpcionVolver();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!capturarDatosRepuesto(s, nuevoR)) return;
    nuevoR.fechaRegistro[0] = '\0';

    if (!confirmarAccion("Guardar repuesto")) return;
    if (agregarRepuesto(s, nuevoR) == nullptr) {
        cout << "[!] Codigo duplicado o datos invalidos." << endl;
        return;
    }
    cout << "\n[OK] Repuesto registrado exitosamente" << endl;
}

void menuBuscarRepuesto(SistemaTaller* s) {
    limpiarTerminal();
    int opc = 0;
    cout << "\n--- BUSCAR REPUESTO ---" << endl;
    cout << "1. Buscar por ID" << endl;
    cout << "2. Buscar por Codigo" << endl;
    cout << "3. Buscar por Nombre (parcial)" << endl;
    mostrarOpcionVolver();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    int campo = 0;
    while (campo < 2) {
        ResultadoEntrada resultado;
        if (campo == 0) {
            resultado = Validaciones::leerEnteroRango("Seleccione una opcion (1-3): ", 1, 3, opc);
        } else if (opc == 1) {
            int id = 0;
            resultado = Validaciones::leerEnteroRango(
                "Ingrese ID del repuesto: ", 1, numeric_limits<int>::max(), id);
            if (resultado == ResultadoEntrada::Correcto) mostrarRepuesto(buscarRepuestoPorID(s, id));
        } else if (opc == 2) {
            char codigo[20] = {};
            resultado = Validaciones::leerTexto("Ingrese Codigo: ", codigo, sizeof(codigo));
            if (resultado == ResultadoEntrada::Correcto) {
                mostrarRepuesto(buscarRepuestoPorCodigo(s, codigo));
            }
        } else {
            char sub[100] = {};
            resultado = Validaciones::leerTexto(
                "Ingrese el nombre o parte del mismo: ", sub, sizeof(sub));
            if (resultado == ResultadoEntrada::Correcto) {
                mostrarRepuesto(buscarRepuestoPorNombreParcial(s, sub));
            }
        }
        if (!Validaciones::procesarResultado(resultado, campo)) return;
    }
}

void menuEntradaMercancia(SistemaTaller* s) {
    limpiarTerminal();
    int id = 0, cant = 0;
    cout << "\n--- REGISTRAR ENTRADA DE MERCANCIA ---" << endl;
    mostrarOpcionVolver();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    int campo = 0;
    while (campo < 2) {
        ResultadoEntrada resultado = campo == 0
            ? Validaciones::leerEnteroRango("Ingrese ID del repuesto: ", 1, numeric_limits<int>::max(), id)
            : Validaciones::leerEnteroRango(
                "Ingrese la cantidad adicional a sumar al stock: ", 1, numeric_limits<int>::max(), cant);
        if (!Validaciones::procesarResultado(resultado, campo)) return;
    }

    if (!confirmarAccion("Registrar entrada de mercancia")) return;
    if (menuRegistrarEntradaMercancia(s, id, cant)) {
        cout << "[OK] Entrada de mercancia registrada con exito." << endl;
    } else {
        cout << "[!] No se encontro el repuesto con ese ID." << endl;
    }
}

void menuCambiarPrecio(SistemaTaller* s) {
    limpiarTerminal();
    int id = 0;
    double precio = 0.0;
    cout << "\n--- CAMBIAR PRECIO DE REPUESTO ---" << endl;
    mostrarOpcionVolver();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    int campo = 0;
    while (campo < 2) {
        ResultadoEntrada resultado = campo == 0
            ? Validaciones::leerEnteroRango("Ingrese ID del repuesto: ", 1, numeric_limits<int>::max(), id)
            : Validaciones::leerDecimalRango(
                "Ingrese el nuevo precio (> 0): ", numeric_limits<double>::min(),
                numeric_limits<float>::max(), precio);
        if (!Validaciones::procesarResultado(resultado, campo)) return;
    }
    float nuevoP = static_cast<float>(precio);

    if (!confirmarAccion("Cambiar precio")) return;
    if (cambiarPrecioRepuesto(s, id, nuevoP)) {
        cout << "[OK] Precio actualizado con exito." << endl;
    } else {
        cout << "[!] No se encontro el repuesto con ese ID." << endl;
    }
}

void menuEliminarRepuesto(SistemaTaller* s) {
    limpiarTerminal();
    int id = 0;
    cout << "\n--- ELIMINAR REPUESTO ---" << endl;
    mostrarOpcionVolver();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (Validaciones::leerEnteroRango(
            "Ingrese el ID del repuesto a eliminar: ", 1, numeric_limits<int>::max(), id) !=
        ResultadoEntrada::Correcto) return;

    if (!confirmarAccion("Eliminar repuesto")) return;
    if (eliminarRepuesto(s, id)) {
        cout << "[OK] Repuesto eliminado exitosamente." << endl;
    } else {
        cout << "[!] No existe o esta asociado a una orden." << endl;
    }
}

void menuListarRepuestos(SistemaTaller* s) {
    limpiarTerminal();
    int cantidad = 0;
    Repuesto* listaSimple = listarRepuesto(s, &cantidad);
    
    if (listaSimple == nullptr || cantidad == 0) {
        cout << "\n[!] No hay repuestos registrados para mostrar." << endl;
        pausarParaContinuar();
        return;
    }

    Repuesto** listaDoble = new Repuesto*[cantidad];
    for (int i = 0; i < cantidad; i++) {
        listaDoble[i] = &listaSimple[i];
    }

    mostrarListaRepuestos(listaDoble, cantidad);
    delete[] listaDoble;
    pausarParaContinuar();
}

void mostrarRepuesto(Repuesto* repuesto) {
    if (repuesto == nullptr) {
        cout << "[!] Repuesto no encontrado." << endl;
        return;
    }
    cout << "-------------------------------------------" << endl;
    cout << "ID: " << repuesto->id << endl;
    cout << "Nombre: " << repuesto->nombre << endl;
    cout << "Codigo: " << repuesto->codigo << endl;
    cout << "Marca: " << repuesto->marca << endl;
    cout << "Precio $: " << repuesto->precio << endl;
    cout << "Stock disponible: " << repuesto->stock;

     if (repuesto->stock <= repuesto->stockMinimo) {
        cout << " [BAJO STOCK]";
    }
      cout << endl;
      
    cout << "Stock minimo: " << repuesto->stockMinimo << endl;
    cout << "--------------------------------------------" << endl;
}

void mostrarListaRepuestos(Repuesto** repuestos, int cantidad) {
    if (repuestos == nullptr || cantidad == 0) {
        cout << "\n[!] No hay repuestos registrados para mostrar." << endl;
        return;
    }

    cout << "\n=== LISTA DE REPUESTOS (" << cantidad << ") ===" << endl;
    for (int i = 0; i < cantidad; i++) {
        mostrarRepuesto(repuestos[i]);
    }
}

//SISTEMA DE ORDEN DE SERVICIO
void redimensionarOrdenes(SistemaTaller* s) {
    s->capacidadOrdenes *= 2;
    OrdenServicio* nuevasOrdenes = new OrdenServicio[s->capacidadOrdenes];

    for (int i = 0; i < s->numOrdenes; i++) {
        nuevasOrdenes[i] = s->ordenes[i];
    }

    delete[] s->ordenes;
    s->ordenes = nuevasOrdenes;
}

OrdenServicio* abrirOrden(SistemaTaller* s, int idVehiculo, int idMecanico, const char* descripcionFalla, int KilometrajeIngreso, const char* fechaActual) {
    Vehiculo* vehiculo = buscarVehiculoPorID(s, idVehiculo);
    if (vehiculo == nullptr || buscarMecanicoporID(s, idMecanico) == nullptr ||
        descripcionFalla == nullptr || descripcionFalla[0] == '\0' ||
        fechaActual == nullptr || !fechaValida(fechaActual) ||
        KilometrajeIngreso < vehiculo->kilometraje ||
        contarOrdenesAbiertasMecanico(s, idMecanico) >= 3) return nullptr;
    for (int i = 0; i < s->numOrdenes; ++i) {
        if (s->ordenes[i].idVehiculo == idVehiculo &&
            strcmp(s->ordenes[i].estado, "Abierta") == 0) return nullptr;
    }
    if (s->numOrdenes >= s->capacidadOrdenes) {
        redimensionarOrdenes(s);
    }

    OrdenServicio nuevaO ={};
    nuevaO.id = s->siguienteIdOrdenes++;
    nuevaO.idVehiculo = idVehiculo;
    nuevaO.idMecanico = idMecanico;
    strcpy(nuevaO.descripcionFalla, descripcionFalla);
    nuevaO.kilometrajeIngreso = KilometrajeIngreso;
    strcpy(nuevaO.fechaApertura, fechaActual);
    strcpy(nuevaO.fechaCierre, "Pendiente");
    strcpy(nuevaO.estado, "Abierta");
    nuevaO.manoObra = 0.0f;
    nuevaO.cantidadLineasUsadas = 0;

    s->ordenes[s->numOrdenes] = nuevaO;
    s->numOrdenes++;
    vehiculo->kilometraje = KilometrajeIngreso;

    return &(s->ordenes[s->numOrdenes - 1]);
}

bool agregarRepuestoAOrden(SistemaTaller* s, int idOrden, int idRepuesto, int cantidad) {
    OrdenServicio* o = buscarOrdenPorID(s, idOrden);
    if (o == nullptr || strcmp(o->estado, "Abierta") != 0 || cantidad <= 0) return false;

    Repuesto* r = buscarRepuestoPorID(s, idRepuesto);
    if (r == nullptr || r->stock < cantidad) return false;

    for (int i = 0; i < o->cantidadLineasUsadas; ++i) {
        if (o->lineasDetalle[i].idRepuesto == idRepuesto) {
            if (o->lineasDetalle[i].cantidad > numeric_limits<int>::max() - cantidad) return false;
            o->lineasDetalle[i].cantidad += cantidad;
            r->stock -= cantidad;
            return true;
        }
    }

    if (o->cantidadLineasUsadas >= 15) return false;

    r->stock -= cantidad;

    int idx = o->cantidadLineasUsadas;
    o->lineasDetalle[idx].idRepuesto = r->id;
    strcpy(o->lineasDetalle[idx].codigoRepuesto, r->codigo);
    strcpy(o->lineasDetalle[idx].nombreRepuesto, r->nombre);
    o->lineasDetalle[idx].precioUnitario = r->precio;
    o->lineasDetalle[idx].cantidad = cantidad;
    o->cantidadLineasUsadas++;

    calcularTotalOrden(o);
    return true;
}

bool quitarRepuestoDeOrden(SistemaTaller* s, int idOrden, int idRepuesto) {
    OrdenServicio* o = buscarOrdenPorID(s, idOrden);
    if (o == nullptr || strcmp(o->estado, "Abierta") != 0) return false;

    int pos = -1;
    for (int i = 0; i < o->cantidadLineasUsadas; i++) {
        if (o->lineasDetalle[i].idRepuesto == idRepuesto) {
            pos = i;
            break;
        }
    }

    if (pos == -1) return false;

    Repuesto* r = buscarRepuestoPorID(s, idRepuesto);
    if (r != nullptr) {
        r->stock += o->lineasDetalle[pos].cantidad;
    }

    for (int i = pos; i < o->cantidadLineasUsadas - 1; i++) {
        o->lineasDetalle[i] = o->lineasDetalle[i + 1];
    }
    o->cantidadLineasUsadas--;

    calcularTotalOrden(o);
    return true;
}

bool cerrarOrden(SistemaTaller* s, int idOrden, float manoObra, const char* fechaActual) {
    OrdenServicio* o = buscarOrdenPorID(s, idOrden);
    if (o == nullptr || strcmp(o->estado, "Abierta") != 0 || manoObra < 0.0f ||
        fechaActual == nullptr || !fechaValida(fechaActual) ||
        (manoObra == 0.0f && o->cantidadLineasUsadas == 0)) return false;

    o->manoObra = manoObra;
    strcpy(o->fechaCierre, fechaActual);
    strcpy(o->estado, "Cerrada");
    calcularTotalOrden(o);
    return true;
}

bool anularOrden(SistemaTaller* s, int idOrden) {
    OrdenServicio* o = buscarOrdenPorID(s, idOrden);
    if (o == nullptr || strcmp(o->estado, "Abierta") != 0) return false;

    for (int i = 0; i < o->cantidadLineasUsadas; i++) {
        Repuesto* r = buscarRepuestoPorID(s, o->lineasDetalle[i].idRepuesto);
        if (r != nullptr) {
            r->stock += o->lineasDetalle[i].cantidad;
        }
    }

    strcpy(o->estado, "Anulada");
    return true;
}

float calcularTotalOrden(const OrdenServicio* orden) {
    if (orden == nullptr) return 0.0f;

    float costoRepuestos = 0.0f;
    for (int i = 0; i < orden->cantidadLineasUsadas; i++) {
        costoRepuestos += (orden->lineasDetalle[i].precioUnitario * orden->lineasDetalle[i].cantidad);
    }

    return orden->manoObra + costoRepuestos;
}

OrdenServicio* buscarOrdenPorID(SistemaTaller* s, int id) {
    for (int i = 0; i < s->numOrdenes; i++) {
        if (s->ordenes[i].id == id) {
            return &(s->ordenes[i]);
        }
    }
    return nullptr;
}

OrdenServicio** listaOrdenesPorEstado(SistemaTaller* s, const char* estado, int* cantidad) {
    int encontrados = 0;
    for (int i = 0; i < s->numOrdenes; i++) {
        if (igualSinMayusculas(s->ordenes[i].estado, estado)) {
            encontrados++;
        }
    }
    *cantidad = encontrados;
    if (encontrados == 0) return nullptr;

    OrdenServicio** lista = new OrdenServicio*[encontrados];
    int j = 0;
    for (int i = 0; i < s->numOrdenes; i++) {
        if (igualSinMayusculas(s->ordenes[i].estado, estado)) {
            lista[j++] = &(s->ordenes[i]);
        }
    }
    return lista;
}

OrdenServicio** listaOrdenesPorVehiculo(SistemaTaller* s, int vehiculo, int* cantidad) {
    int encontrados = 0;
    for (int i = 0; i < s->numOrdenes; i++) {
        if (s->ordenes[i].idVehiculo == vehiculo) {
            encontrados++;
        }
    }
    *cantidad = encontrados;
    if (encontrados == 0) return nullptr;

    OrdenServicio** lista = new OrdenServicio*[encontrados];
    int j = 0;
    for (int i = 0; i < s->numOrdenes; i++) {
        if (s->ordenes[i].idVehiculo == vehiculo) {
            lista[j++] = &(s->ordenes[i]);
        }
    }
    return lista;
}

OrdenServicio** listaTodasLasOrdenes(SistemaTaller* s, int* cantidad) {
    *cantidad = s->numOrdenes;
    if (s->numOrdenes == 0) return nullptr;

    OrdenServicio** lista = new OrdenServicio*[s->numOrdenes];
    for (int i = 0; i < s->numOrdenes; i++) {
        lista[i] = &(s->ordenes[i]);
    }
    return lista;
}

void menuAbrirOrden(SistemaTaller* s) {
    limpiarTerminal();
    int idV = 0, idM = 0, km = 0;
    char falla[200] = {}, fecha[11] = {};

    cout << "\n--- ABRIR NUEVA ORDEN DE SERVICIO ---" << endl;
    mostrarOpcionVolver();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    int campo = 0;
    while (campo < 4) {
        ResultadoEntrada resultado;
        if (campo == 0) {
            resultado = Validaciones::leerEnteroRango(
                "Ingrese ID del Vehiculo: ", 1, numeric_limits<int>::max(), idV);
            if (resultado == ResultadoEntrada::Correcto && buscarVehiculoPorID(s, idV) == nullptr) {
                cout << "[!] El vehiculo no existe." << endl;
                resultado = ResultadoEntrada::Reintentar;
            }
        } else if (campo == 1) {
            resultado = Validaciones::leerEnteroRango(
                "Ingrese ID del Mecanico: ", 1, numeric_limits<int>::max(), idM);
            if (resultado == ResultadoEntrada::Correcto && buscarMecanicoporID(s, idM) == nullptr) {
                cout << "[!] El mecanico no existe." << endl;
                resultado = ResultadoEntrada::Reintentar;
            }
        } else if (campo == 2) {
            resultado = Validaciones::leerTexto("Descripcion de la falla: ", falla, sizeof(falla));
        } else {
            Vehiculo* vehiculo = buscarVehiculoPorID(s, idV);
            int kilometrajeMinimo = vehiculo == nullptr ? 0 : vehiculo->kilometraje;
            resultado = Validaciones::leerEnteroRango(
                "Kilometraje de ingreso: ", kilometrajeMinimo, numeric_limits<int>::max(), km);
        }
        if (!Validaciones::procesarResultado(resultado, campo)) return;
    }
    obtenerFechaActual(fecha, sizeof(fecha));
    if (!confirmarAccion("Abrir orden")) return;
    if (abrirOrden(s, idV, idM, falla, km, fecha) == nullptr) {
        cout << "[!] No se pudo abrir: revise kilometraje y limites de ordenes abiertas." << endl;
        return;
    }
    cout << "\n[OK] Orden abierta exitosamente. Fecha: " << fecha << endl;
}

void menuAgregarRepuestoAOrden(SistemaTaller* s) {
    limpiarTerminal();
    int idO = 0, idR = 0, cant = 0;
    cout << "\n--- AGREGAR REPUESTO A ORDEN ---" << endl;
    mostrarOpcionVolver();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    int campo = 0;
    while (campo < 3) {
        ResultadoEntrada resultado;
        if (campo == 0) {
            resultado = Validaciones::leerEnteroRango(
                "Ingrese ID de la Orden: ", 1, numeric_limits<int>::max(), idO);
        } else if (campo == 1) {
            resultado = Validaciones::leerEnteroRango(
                "Ingrese ID del Repuesto: ", 1, numeric_limits<int>::max(), idR);
        } else {
            resultado = Validaciones::leerEnteroRango(
                "Cantidad: ", 1, numeric_limits<int>::max(), cant);
        }
        if (!Validaciones::procesarResultado(resultado, campo)) return;
    }

    if (!confirmarAccion("Agregar repuesto a la orden")) return;
    if (agregarRepuestoAOrden(s, idO, idR, cant)) {
        cout << "[OK] Repuesto agregado a la orden con exito." << endl;
    } else {
        cout << "[!] Error al agregar repuesto (verifique que la orden este abierta, el repuesto exista y haya stock)." << endl;
    }
}

void menuQuitarRepuestoDeOrden(SistemaTaller* s) {
    limpiarTerminal();
    int idO = 0, idR = 0;
    cout << "\n--- QUITAR REPUESTO DE ORDEN ---" << endl;
    mostrarOpcionVolver();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    int campo = 0;
    while (campo < 2) {
        ResultadoEntrada resultado = campo == 0
            ? Validaciones::leerEnteroRango(
                "Ingrese ID de la Orden: ", 1, numeric_limits<int>::max(), idO)
            : Validaciones::leerEnteroRango(
                "Ingrese ID del Repuesto a retirar: ", 1, numeric_limits<int>::max(), idR);
        if (!Validaciones::procesarResultado(resultado, campo)) return;
    }

    if (!confirmarAccion("Quitar repuesto de la orden")) return;
    if (quitarRepuestoDeOrden(s, idO, idR)) {
        cout << "[OK] Repuesto retirado de la orden y devuelto al stock." << endl;
    } else {
        cout << "[!] Error al quitar el repuesto." << endl;
    }
}

void menuCerrarOrden(SistemaTaller* s) {
    limpiarTerminal();
    int idO = 0;
    double manoObraIngresada = 0.0;
    float manoObra = 0.0f;
    char fecha[11] = {};
    cout << "\n--- CERRAR ORDEN DE SERVICIO ---" << endl;
    mostrarOpcionVolver();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    int campo = 0;
    while (campo < 2) {
        ResultadoEntrada resultado = campo == 0
            ? Validaciones::leerEnteroRango(
                "Ingrese ID de la Orden a cerrar: ", 1, numeric_limits<int>::max(), idO)
            : Validaciones::leerDecimalRango(
                "Costo de Mano de Obra: ", 0.0, numeric_limits<float>::max(), manoObraIngresada);
        if (!Validaciones::procesarResultado(resultado, campo)) return;
    }
    manoObra = static_cast<float>(manoObraIngresada);
    obtenerFechaActual(fecha, sizeof(fecha));

    if (!confirmarAccion("Cerrar orden")) return;
    if (cerrarOrden(s, idO, manoObra, fecha)) {
        cout << "[OK] Orden cerrada exitosamente. Total calculado." << endl;
    } else {
        cout << "[!] Error al cerrar la orden (verifique que este abierta)." << endl;
    }
}

void menuAnularOrden(SistemaTaller* s) {
    limpiarTerminal();
    int idO = 0;
    cout << "\n--- ANULAR ORDEN DE SERVICIO ---" << endl;
    mostrarOpcionVolver();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (Validaciones::leerEnteroRango(
            "Ingrese ID de la Orden a anular: ", 1, numeric_limits<int>::max(), idO) !=
        ResultadoEntrada::Correcto) return;

    if (!confirmarAccion("Anular orden")) return;
    if (anularOrden(s, idO)) {
        cout << "[OK] Orden anulada. Repuestos devueltos al inventario." << endl;
    } else {
        cout << "[!] Error al anular la orden." << endl;
    }
}

void menuVerDetalleOrden(SistemaTaller* s) {
    limpiarTerminal();
    int idO = 0;
    cout << "\n--- CONSULTAR DETALLE DE ORDEN ---" << endl;
    mostrarOpcionVolver();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (Validaciones::leerEnteroRango(
            "Ingrese ID de la Orden: ", 1, numeric_limits<int>::max(), idO) !=
        ResultadoEntrada::Correcto) return;
    OrdenServicio* o = buscarOrdenPorID(s, idO);
    mostrarDetalleOrden(s, o);
}

//SISTEMA DE LISTADO GENERAL
void menuListarOrdenes(SistemaTaller* s) {
    limpiarTerminal();
    int opc = 0, cantidad = 0;
    cout << "\n--- LISTAR ORDENES ---" << endl;
    cout << "1. Listar todas" << endl;
    cout << "2. Listar por Estado (Abierta / Cerrada / Anulada)" << endl;
    cout << "3. Listar por Vehiculo (por ID)" << endl;
    mostrarOpcionVolver();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    int campo = 0;
    while (campo < 2) {
        ResultadoEntrada resultado;
        if (campo == 0) {
            resultado = Validaciones::leerEnteroRango(
                "Seleccione una opcion (1-3): ", 1, 3, opc);
        } else if (opc == 1) {
            OrdenServicio** lista = listaTodasLasOrdenes(s, &cantidad);
            mostrarListaOrdenes(s, lista, cantidad);
            delete[] lista;
            resultado = ResultadoEntrada::Correcto;
        } else if (opc == 2) {
            char estado[20] = {};
            resultado = Validaciones::leerTexto(
                "Ingrese estado a buscar (Abierta, Cerrada o Anulada): ", estado, sizeof(estado));
            if (resultado == ResultadoEntrada::Correcto &&
                !igualSinMayusculas(estado, "Abierta") &&
                !igualSinMayusculas(estado, "Cerrada") &&
                !igualSinMayusculas(estado, "Anulada")) {
                cout << "[!] Estado invalido." << endl;
                resultado = ResultadoEntrada::Reintentar;
            }
            if (resultado == ResultadoEntrada::Correcto) {
                OrdenServicio** lista = listaOrdenesPorEstado(s, estado, &cantidad);
                mostrarListaOrdenes(s, lista, cantidad);
                delete[] lista;
            }
        } else {
            int idV = 0;
            resultado = Validaciones::leerEnteroRango(
                "Ingrese ID del Vehiculo: ", 1, numeric_limits<int>::max(), idV);
            if (resultado == ResultadoEntrada::Correcto) {
                OrdenServicio** lista = listaOrdenesPorVehiculo(s, idV, &cantidad);
                mostrarListaOrdenes(s, lista, cantidad);
                delete[] lista;
            }
        }
        if (!Validaciones::procesarResultado(resultado, campo)) return;
    }
    pausarParaContinuar();
}

void mostrarDetalleOrden(SistemaTaller* s, OrdenServicio* orden) {
    if (orden == nullptr) {
        cout << "[!] Orden de servicio no encontrada." << endl;
        return;
    }

    Vehiculo* v = buscarVehiculoPorID(s, orden->idVehiculo);
    Mecanico* m = buscarMecanicoporID(s, orden->idMecanico);

    cout << "\n----------------------------------------" << endl;
    cout << " ORDEN DE SERVICIO # " << orden->id << endl;
    cout << " Estado: " << orden->estado << endl;
    cout << " Fecha Ingreso: " << orden->fechaApertura << " | Cierre: " << orden->fechaCierre << endl;
    cout << " Kilometraje Ingreso: " << orden->kilometrajeIngreso << " km" << endl;
    cout << " Falla reportada: " << orden->descripcionFalla << endl;
    
    cout << " --- VEHICULO ASOCIADO ---" << endl;
    if (v != nullptr) {
        cout << " Placa: " << v->placa << " | " << v->marca << " " << v->modelo << " (" << v->anio << ")" << endl;
        cout << " Propietario: " << v->nombrePropietario << endl;
    } else {
        cout << " [!] Vehiculo con ID " << orden->idVehiculo << " no encontrado o eliminado." << endl;
    }

    cout << " --- MECANICO ASIGNADO ---" << endl;
    if (m != nullptr) {
        cout << " Nombre: " << m->nombre << " (Especialidad: " << m->especialidad << ")" << endl;
    } else {
        cout << " [!] Mecanico con ID " << orden->idMecanico << " no encontrado." << endl;
    }

    cout << " --- REPUESTOS UTILIZADOS (" << orden-> cantidadLineasUsadas << ") ---" << endl;
    for (int i = 0; i < orden->cantidadLineasUsadas; i++) {
        cout << "  - " << orden->lineasDetalle[i].nombreRepuesto 
             << " x" << orden->lineasDetalle[i].cantidad 
             << " ($" << orden->lineasDetalle[i].precioUnitario << " c/u)" << endl;
    }

    float total = calcularTotalOrden(orden);
    cout << " Mano de Obra: $" << orden->manoObra << endl;
    cout << " TOTAL A PAGAR: $" << total << endl;
    cout << "----------------------------------------" << endl;
}

void mostrarListaOrdenes(SistemaTaller* s, OrdenServicio** ordenes, int cantidad) {
    (void)s;
    if (ordenes == nullptr || cantidad == 0) {
        cout << "\n[!] No hay ordenes para mostrar." << endl;
        return;
    }
    cout << "\n=== LISTADO DE ORDENES (" << cantidad << ") ===" << endl;
    for (int i = 0; i < cantidad; i++) {
        float total = calcularTotalOrden(ordenes[i]);
        cout << "[ID Orden: " << ordenes[i]->id << "] - Vehiculo ID: " << ordenes[i]->idVehiculo 
             << " - Estado: " << ordenes[i]->estado << " - Total: $" << total << endl;
    }
}

void menuListaGeneral(SistemaTaller* s) {
    limpiarTerminal();
    cout << "\n=== LISTA GENERAL DE REGISTROS ===" << endl;
    cout << "Vehiculos registrados: " << s->numVehiculos << endl;
    if (s->numVehiculos == 0) {
        cout << "  No hay vehiculos." << endl;
    } else {
        for (int i = 0; i < s->numVehiculos; ++i) mostrarVehiculo(&s->vehiculos[i]);
    }

    cout << "\nMecanicos registrados: " << s->numMecanicos << endl;
    if (s->numMecanicos == 0) {
        cout << "  No hay mecanicos." << endl;
    } else {
        for (int i = 0; i < s->numMecanicos; ++i) {
            mostrarMecanicoUnico(&s->mecanicos[i]);
            cout << "Ordenes abiertas: "
                 << contarOrdenesAbiertasMecanico(s, s->mecanicos[i].id) << endl;
        }
    }

    cout << "\nRepuestos registrados: " << s->numRepuestos << endl;
    if (s->numRepuestos == 0) {
        cout << "  No hay repuestos." << endl;
    } else {
        for (int i = 0; i < s->numRepuestos; ++i) mostrarRepuesto(&s->repuestos[i]);
    }

    cout << "\nOrdenes de servicio registradas: " << s->numOrdenes << endl;
    if (s->numOrdenes == 0) {
        cout << "  No hay ordenes." << endl;
    } else {
        for (int i = 0; i < s->numOrdenes; ++i) mostrarDetalleOrden(s, &s->ordenes[i]);
    }

    pausarParaContinuar();
}

void menuVehiculos(SistemaTaller* s) {
    int opcion = -1;
    while (opcion != 0) {
        limpiarTerminal();
        cout << "\n--- VEHICULOS ---\n1. Registrar\n";
        if (s->numVehiculos > 0) {
            cout << "2. Buscar\n3. Actualizar\n4. Eliminar\n5. Listar\n";
        }
        cout << "0. Volver\nOpcion: ";
        if (!(cin >> opcion)) {
            if (cin.eof()) return;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            entradaConSaltoPendiente = false;
            cout << "[!] Ingrese un numero." << endl;
            continue;
        }
        entradaConSaltoPendiente = true;
        if (s->numVehiculos == 0 && opcion != 0 && opcion != 1) {
            cout << "[!] Opcion invalida." << endl;
            continue;
        }
        switch (opcion) {
            case 1: menuRegistrarVehiculo(s); pausarParaContinuar(); break;
            case 2: menuBuscarVehiculo(s); pausarParaContinuar(); break;
            case 3: menuActualizarVehiculo(s); pausarParaContinuar(); break;
            case 4: menuEliminarVehiculo(s); pausarParaContinuar(); break;
            case 5: menuListarVehiculos(s); break;
            case 0: break;
            default: cout << "[!] Opcion invalida." << endl;
        }
    }
}

void menuMecanicos(SistemaTaller* s) {
    int opcion = -1;
    while (opcion != 0) {
        limpiarTerminal();
        cout << "\n--- MECANICOS ---\n1. Registrar\n";
        if (s->numMecanicos > 0) cout << "2. Listar\n";
        cout << "0. Volver\nOpcion: ";
        if (!(cin >> opcion)) {
            if (cin.eof()) return;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            entradaConSaltoPendiente = false;
            cout << "[!] Ingrese un numero." << endl;
            continue;
        }
        entradaConSaltoPendiente = true;
        if (s->numMecanicos == 0 && opcion != 0 && opcion != 1) {
            cout << "[!] Opcion invalida." << endl;
            continue;
        }
        switch (opcion) {
            case 1: menuRegistrarMecanico(s); pausarParaContinuar(); break;
            case 2: menuListarMecanico(s); break;
            case 0: break;
            default: cout << "[!] Opcion invalida." << endl;
        }
    }
}

void menuRepuestos(SistemaTaller* s) {
    int opcion = -1;
    while (opcion != 0) {
        limpiarTerminal();
        cout << "\n--- REPUESTOS ---\n1. Registrar\n";
        if (s->numRepuestos > 0) {
            cout << "2. Buscar\n3. Entrada de mercancia\n4. Cambiar precio\n"
                 << "5. Eliminar\n6. Listar\n";
        }
        cout << "0. Volver\nOpcion: ";
        if (!(cin >> opcion)) {
            if (cin.eof()) return;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            entradaConSaltoPendiente = false;
            cout << "[!] Ingrese un numero." << endl;
            continue;
        }
        entradaConSaltoPendiente = true;
        if (s->numRepuestos == 0 && opcion != 0 && opcion != 1) {
            cout << "[!] Opcion invalida." << endl;
            continue;
        }
        switch (opcion) {
            case 1: menuRegistrarRepuesto(s); pausarParaContinuar(); break;
            case 2: menuBuscarRepuesto(s); pausarParaContinuar(); break;
            case 3: menuEntradaMercancia(s); pausarParaContinuar(); break;
            case 4: menuCambiarPrecio(s); pausarParaContinuar(); break;
            case 5: menuEliminarRepuesto(s); pausarParaContinuar(); break;
            case 6: menuListarRepuestos(s); break;
            case 0: break;
            default: cout << "[!] Opcion invalida." << endl;
        }
    }
}

void menuOrdenes(SistemaTaller* s) {
    if (s->numVehiculos == 0 || s->numMecanicos == 0 || s->numRepuestos == 0) return;
    int opcion = -1;
    while (opcion != 0) {
        limpiarTerminal();
        cout << "\n--- ORDENES DE SERVICIO ---\n1. Abrir orden\n";
        if (s->numOrdenes > 0) {
            cout << "2. Agregar repuesto\n3. Quitar repuesto\n4. Cerrar orden\n"
                 << "5. Anular orden\n6. Ver detalle\n7. Listar ordenes\n";
        }
        cout << "0. Volver\nOpcion: ";
        if (!(cin >> opcion)) {
            if (cin.eof()) return;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            entradaConSaltoPendiente = false;
            cout << "[!] Ingrese un numero." << endl;
            continue;
        }
        entradaConSaltoPendiente = true;
        if (s->numOrdenes == 0 && opcion != 0 && opcion != 1) {
            cout << "[!] Opcion invalida." << endl;
            continue;
        }
        switch (opcion) {
            case 1: menuAbrirOrden(s); pausarParaContinuar(); break;
            case 2: menuAgregarRepuestoAOrden(s); pausarParaContinuar(); break;
            case 3: menuQuitarRepuestoDeOrden(s); pausarParaContinuar(); break;
            case 4: menuCerrarOrden(s); pausarParaContinuar(); break;
            case 5: menuAnularOrden(s); pausarParaContinuar(); break;
            case 6: menuVerDetalleOrden(s); pausarParaContinuar(); break;
            case 7: menuListarOrdenes(s); break;
            case 0: break;
            default: cout << "[!] Opcion invalida." << endl;
        }
    }
}

//SISTEMA MAIN
int main() {
    Taller taller = {};
    cout << "Nombre del taller: ";
    cin.getline(taller.nombre, sizeof(taller.nombre));
    cout << "RIF: ";
    cin.getline(taller.rif, sizeof(taller.rif));
    cout << "Direccion: ";
    cin.getline(taller.direccion, sizeof(taller.direccion));
    cout << "Telefono: ";
    cin.getline(taller.telefono, sizeof(taller.telefono));

    SistemaTaller sistema = {};
    inicializarSistema(sistema, taller);
    
    cargarDatos(sistema);

    int opcion = -1;
    bool salir = false;
    while (!salir) {
        limpiarTerminal();
        cout << "\n=== " << sistema.taller.nombre << " ===\n"
             << "1. Vehiculos\n2. Mecanicos\n3. Repuestos\n";
        if (sistema.numVehiculos > 0 && sistema.numMecanicos > 0 && sistema.numRepuestos > 0) {
            cout << "4. Ordenes de servicio\n";
        }
        cout << "5. Lista general de registros\n0. Salir\nOpcion: ";
        if (!(cin >> opcion)) {
            if (cin.eof()) break;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            entradaConSaltoPendiente = false;
            cout << "[!] Ingrese un numero." << endl;
            continue;
        }
        entradaConSaltoPendiente = true;
        switch (opcion) {
            case 1: menuVehiculos(&sistema); break;
            case 2: menuMecanicos(&sistema); break;
            case 3: menuRepuestos(&sistema); break;
            case 4:
                if (sistema.numVehiculos > 0 && sistema.numMecanicos > 0 && sistema.numRepuestos > 0) {
                    menuOrdenes(&sistema);
                } else {
                    cout << "[!] Registre al menos un vehiculo, un mecanico y un repuesto antes de gestionar ordenes." << endl;
                }
                break;
            case 5: menuListaGeneral(&sistema); break;
            case 0: 
                salir = confirmarAccion("Salir del sistema"); 
                if (salir) {
                    guardarDatos(sistema);
                }
                break;
            default: cout << "[!] Opcion invalida." << endl;
        }
    }
    liberarSistema(sistema);
    return 0;
}
