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

struct DetalleArchivo {
    int idDetalle;
    int idOrden;
    int idRepuesto;
    char codigoRepuesto[20];
    char nombreRepuesto[100];
    int cantidad;
    float precioUnitario;
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;
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
const char* ARCHIVO_DETALLES  = "detalles.bin";
const int   VERSION_ARCHIVO   = 1;
const size_t MAX_DETALLES_POR_CONSULTA = 100;

enum class ResultadoArchivoDetalles {
    Correcto,
    ArchivoNoDisponible,
    EncabezadoInvalido,
    IdInvalido,
    ParametrosInvalidos,
    RegistroInconsistente,
    RegistroNoEncontrado,
    ErrorLectura,
    ErrorEscritura
};

ResultadoArchivoDetalles leerHeaderDetalles(ArchivoHeader& header) {
    std::ifstream archivo(ARCHIVO_DETALLES, ios::in | ios::binary);
    if (!archivo.is_open()) return ResultadoArchivoDetalles::ArchivoNoDisponible;

    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    if (!archivo) return ResultadoArchivoDetalles::ErrorLectura;
    if (header.cantidadRegistros < 0 ||
        header.cantidadRegistros == numeric_limits<int>::max() ||
        header.proximoID != header.cantidadRegistros + 1 ||
        header.registrosActivos < 0 ||
        header.registrosActivos > header.cantidadRegistros ||
        header.version != VERSION_ARCHIVO) {
        return ResultadoArchivoDetalles::EncabezadoInvalido;
    }

    return ResultadoArchivoDetalles::Correcto;
}

ResultadoArchivoDetalles actualizarHeaderDetalles(const ArchivoHeader& header) {
    if (header.cantidadRegistros < 0 ||
        header.cantidadRegistros == numeric_limits<int>::max() ||
        header.proximoID != header.cantidadRegistros + 1 ||
        header.registrosActivos < 0 ||
        header.registrosActivos > header.cantidadRegistros ||
        header.version != VERSION_ARCHIVO) {
        return ResultadoArchivoDetalles::EncabezadoInvalido;
    }

    std::fstream archivo(ARCHIVO_DETALLES, ios::in | ios::out | ios::binary);
    if (!archivo.is_open()) return ResultadoArchivoDetalles::ArchivoNoDisponible;

    archivo.seekp(0, ios::beg);
    if (!archivo) return ResultadoArchivoDetalles::ErrorEscritura;
    archivo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
    return archivo ? ResultadoArchivoDetalles::Correcto : ResultadoArchivoDetalles::ErrorEscritura;
}

ResultadoArchivoDetalles crearDetalleArchivo(DetalleArchivo& detalle) {
    ArchivoHeader header;
    ResultadoArchivoDetalles resultado = leerHeaderDetalles(header);
    if (resultado != ResultadoArchivoDetalles::Correcto) return resultado;

    std::fstream archivo(ARCHIVO_DETALLES, ios::in | ios::out | ios::binary);
    if (!archivo.is_open()) return ResultadoArchivoDetalles::ArchivoNoDisponible;

    archivo.seekg(0, ios::end);
    if (!archivo) return ResultadoArchivoDetalles::ErrorLectura;
    const streamoff posicionFinalEsperada =
        static_cast<streamoff>(sizeof(ArchivoHeader)) +
        static_cast<streamoff>(header.cantidadRegistros) *
            static_cast<streamoff>(sizeof(DetalleArchivo));
    if (archivo.tellg() != posicionFinalEsperada) {
        return ResultadoArchivoDetalles::RegistroInconsistente;
    }

    DetalleArchivo nuevo = detalle;
    nuevo.idDetalle = header.proximoID;
    nuevo.eliminado = false;
    nuevo.fechaCreacion = time(nullptr);
    nuevo.fechaUltimaModificacion = nuevo.fechaCreacion;

    archivo.clear();
    archivo.seekp(posicionFinalEsperada, ios::beg);
    if (!archivo) return ResultadoArchivoDetalles::ErrorEscritura;
    archivo.write(reinterpret_cast<const char*>(&nuevo), sizeof(DetalleArchivo));
    if (!archivo) return ResultadoArchivoDetalles::ErrorEscritura;
    archivo.close();
    if (archivo.fail()) return ResultadoArchivoDetalles::ErrorEscritura;

    ++header.cantidadRegistros;
    ++header.registrosActivos;
    ++header.proximoID;
    resultado = actualizarHeaderDetalles(header);
    if (resultado != ResultadoArchivoDetalles::Correcto) return resultado;

    detalle = nuevo;
    return ResultadoArchivoDetalles::Correcto;
}

ResultadoArchivoDetalles leerDetallePorID(int id, DetalleArchivo& detalle) {
    ArchivoHeader header;
    ResultadoArchivoDetalles resultado = leerHeaderDetalles(header);
    if (resultado != ResultadoArchivoDetalles::Correcto) return resultado;
    if (id < 1 || id > header.cantidadRegistros || id >= header.proximoID) {
        return ResultadoArchivoDetalles::IdInvalido;
    }

    const streamoff posicion =
        static_cast<streamoff>(sizeof(ArchivoHeader)) +
        static_cast<streamoff>(id - 1) * static_cast<streamoff>(sizeof(DetalleArchivo));
    std::ifstream archivo(ARCHIVO_DETALLES, ios::in | ios::binary);
    if (!archivo.is_open()) return ResultadoArchivoDetalles::ArchivoNoDisponible;

    archivo.seekg(posicion, ios::beg);
    if (!archivo) return ResultadoArchivoDetalles::ErrorLectura;
    archivo.read(reinterpret_cast<char*>(&detalle), sizeof(DetalleArchivo));
    if (!archivo) return ResultadoArchivoDetalles::ErrorLectura;
    if (detalle.idDetalle != id) return ResultadoArchivoDetalles::RegistroInconsistente;

    return ResultadoArchivoDetalles::Correcto;
}

ResultadoArchivoDetalles buscarDetallesDeOrden(
    int idOrden,
    DetalleArchivo* resultados,
    size_t capacidad,
    size_t& cantidad,
    bool& hayMas) {
    cantidad = 0;
    hayMas = false;
    if (idOrden < 1 || resultados == nullptr || capacidad == 0) {
        return ResultadoArchivoDetalles::ParametrosInvalidos;
    }

    ArchivoHeader header;
    ResultadoArchivoDetalles resultado = leerHeaderDetalles(header);
    if (resultado != ResultadoArchivoDetalles::Correcto) return resultado;

    std::ifstream archivo(ARCHIVO_DETALLES, ios::in | ios::binary);
    if (!archivo.is_open()) return ResultadoArchivoDetalles::ArchivoNoDisponible;

    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    if (!archivo) return ResultadoArchivoDetalles::ErrorLectura;

    const size_t limite = capacidad < MAX_DETALLES_POR_CONSULTA
        ? capacidad
        : MAX_DETALLES_POR_CONSULTA;
    for (int indice = 0; indice < header.cantidadRegistros; ++indice) {
        DetalleArchivo detalle;
        archivo.read(reinterpret_cast<char*>(&detalle), sizeof(DetalleArchivo));
        if (!archivo) return ResultadoArchivoDetalles::ErrorLectura;
        if (detalle.idDetalle != indice + 1) {
            return ResultadoArchivoDetalles::RegistroInconsistente;
        }
        if (detalle.idOrden != idOrden || detalle.eliminado) continue;

        if (cantidad == limite) {
            hayMas = true;
            break;
        }
        resultados[cantidad++] = detalle;
    }

    return ResultadoArchivoDetalles::Correcto;
}

ResultadoArchivoDetalles buscarDetalleOrdenRepuesto(
    int idOrden,
    int idRepuesto,
    DetalleArchivo& encontrado) {
    if (idOrden < 1 || idRepuesto < 1) {
        return ResultadoArchivoDetalles::ParametrosInvalidos;
    }

    ArchivoHeader header;
    ResultadoArchivoDetalles resultado = leerHeaderDetalles(header);
    if (resultado != ResultadoArchivoDetalles::Correcto) return resultado;

    std::ifstream archivo(ARCHIVO_DETALLES, ios::in | ios::binary);
    if (!archivo.is_open()) return ResultadoArchivoDetalles::ArchivoNoDisponible;
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    if (!archivo) return ResultadoArchivoDetalles::ErrorLectura;

    for (int indice = 0; indice < header.cantidadRegistros; ++indice) {
        DetalleArchivo detalle;
        archivo.read(reinterpret_cast<char*>(&detalle), sizeof(DetalleArchivo));
        if (!archivo) return ResultadoArchivoDetalles::ErrorLectura;
        if (detalle.idDetalle != indice + 1) {
            return ResultadoArchivoDetalles::RegistroInconsistente;
        }
        if (detalle.idOrden == idOrden && detalle.idRepuesto == idRepuesto &&
            !detalle.eliminado) {
            encontrado = detalle;
            return ResultadoArchivoDetalles::Correcto;
        }
    }

    return ResultadoArchivoDetalles::RegistroNoEncontrado;
}

Repuesto* buscarRepuestoPorID(SistemaTaller* s, int id);

ResultadoArchivoDetalles contarDetallesDeOrden(int idOrden, size_t& cantidad) {
    cantidad = 0;
    if (idOrden < 1) return ResultadoArchivoDetalles::ParametrosInvalidos;

    ArchivoHeader header;
    ResultadoArchivoDetalles resultado = leerHeaderDetalles(header);
    if (resultado != ResultadoArchivoDetalles::Correcto) return resultado;

    std::ifstream archivo(ARCHIVO_DETALLES, ios::in | ios::binary);
    if (!archivo.is_open()) return ResultadoArchivoDetalles::ArchivoNoDisponible;
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    if (!archivo) return ResultadoArchivoDetalles::ErrorLectura;

    for (int indice = 0; indice < header.cantidadRegistros; ++indice) {
        DetalleArchivo detalle;
        archivo.read(reinterpret_cast<char*>(&detalle), sizeof(DetalleArchivo));
        if (!archivo) return ResultadoArchivoDetalles::ErrorLectura;
        if (detalle.idDetalle != indice + 1) {
            return ResultadoArchivoDetalles::RegistroInconsistente;
        }
        if (detalle.idOrden == idOrden && !detalle.eliminado) ++cantidad;
    }

    return ResultadoArchivoDetalles::Correcto;
}

ResultadoArchivoDetalles calcularCostoRepuestosOrden(int idOrden, float& total) {
    total = 0.0f;
    if (idOrden < 1) return ResultadoArchivoDetalles::ParametrosInvalidos;

    ArchivoHeader header;
    ResultadoArchivoDetalles resultado = leerHeaderDetalles(header);
    if (resultado != ResultadoArchivoDetalles::Correcto) return resultado;

    std::ifstream archivo(ARCHIVO_DETALLES, ios::in | ios::binary);
    if (!archivo.is_open()) return ResultadoArchivoDetalles::ArchivoNoDisponible;
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    if (!archivo) return ResultadoArchivoDetalles::ErrorLectura;

    for (int indice = 0; indice < header.cantidadRegistros; ++indice) {
        DetalleArchivo detalle;
        archivo.read(reinterpret_cast<char*>(&detalle), sizeof(DetalleArchivo));
        if (!archivo) return ResultadoArchivoDetalles::ErrorLectura;
        if (detalle.idDetalle != indice + 1) {
            return ResultadoArchivoDetalles::RegistroInconsistente;
        }
        if (detalle.idOrden == idOrden && !detalle.eliminado) {
            total += detalle.precioUnitario * detalle.cantidad;
        }
    }

    return ResultadoArchivoDetalles::Correcto;
}

ResultadoArchivoDetalles existeDetalleDeRepuesto(int idRepuesto, bool& existe) {
    existe = false;
    if (idRepuesto < 1) return ResultadoArchivoDetalles::ParametrosInvalidos;

    ArchivoHeader header;
    ResultadoArchivoDetalles resultado = leerHeaderDetalles(header);
    if (resultado != ResultadoArchivoDetalles::Correcto) return resultado;

    std::ifstream archivo(ARCHIVO_DETALLES, ios::in | ios::binary);
    if (!archivo.is_open()) return ResultadoArchivoDetalles::ArchivoNoDisponible;
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    if (!archivo) return ResultadoArchivoDetalles::ErrorLectura;

    for (int indice = 0; indice < header.cantidadRegistros; ++indice) {
        DetalleArchivo detalle;
        archivo.read(reinterpret_cast<char*>(&detalle), sizeof(DetalleArchivo));
        if (!archivo) return ResultadoArchivoDetalles::ErrorLectura;
        if (detalle.idDetalle != indice + 1) {
            return ResultadoArchivoDetalles::RegistroInconsistente;
        }
        if (detalle.idRepuesto == idRepuesto && !detalle.eliminado) {
            existe = true;
            return ResultadoArchivoDetalles::Correcto;
        }
    }

    return ResultadoArchivoDetalles::Correcto;
}

ResultadoArchivoDetalles validarRestitucionStockOrden(SistemaTaller* s, int idOrden) {
    ArchivoHeader header;
    ResultadoArchivoDetalles resultado = leerHeaderDetalles(header);
    if (resultado != ResultadoArchivoDetalles::Correcto) return resultado;

    std::ifstream archivo(ARCHIVO_DETALLES, ios::in | ios::binary);
    if (!archivo.is_open()) return ResultadoArchivoDetalles::ArchivoNoDisponible;
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    if (!archivo) return ResultadoArchivoDetalles::ErrorLectura;

    for (int indice = 0; indice < header.cantidadRegistros; ++indice) {
        DetalleArchivo detalle;
        archivo.read(reinterpret_cast<char*>(&detalle), sizeof(DetalleArchivo));
        if (!archivo) return ResultadoArchivoDetalles::ErrorLectura;
        if (detalle.idDetalle != indice + 1) {
            return ResultadoArchivoDetalles::RegistroInconsistente;
        }
        if (detalle.idOrden != idOrden || detalle.eliminado) continue;

        Repuesto* repuesto = buscarRepuestoPorID(s, detalle.idRepuesto);
        if (detalle.cantidad <= 0 || repuesto == nullptr ||
            repuesto->stock > numeric_limits<int>::max() - detalle.cantidad) {
            return ResultadoArchivoDetalles::RegistroInconsistente;
        }
    }

    return ResultadoArchivoDetalles::Correcto;
}

ResultadoArchivoDetalles restituirStockOrden(SistemaTaller* s, int idOrden) {
    ArchivoHeader header;
    ResultadoArchivoDetalles resultado = leerHeaderDetalles(header);
    if (resultado != ResultadoArchivoDetalles::Correcto) return resultado;

    std::ifstream archivo(ARCHIVO_DETALLES, ios::in | ios::binary);
    if (!archivo.is_open()) return ResultadoArchivoDetalles::ArchivoNoDisponible;
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    if (!archivo) return ResultadoArchivoDetalles::ErrorLectura;

    for (int indice = 0; indice < header.cantidadRegistros; ++indice) {
        DetalleArchivo detalle;
        archivo.read(reinterpret_cast<char*>(&detalle), sizeof(DetalleArchivo));
        if (!archivo) return ResultadoArchivoDetalles::ErrorLectura;
        if (detalle.idDetalle != indice + 1) {
            return ResultadoArchivoDetalles::RegistroInconsistente;
        }
        if (detalle.idOrden == idOrden && !detalle.eliminado) {
            if (detalle.cantidad <= 0) return ResultadoArchivoDetalles::RegistroInconsistente;
            Repuesto* repuesto = buscarRepuestoPorID(s, detalle.idRepuesto);
            if (repuesto == nullptr) return ResultadoArchivoDetalles::RegistroInconsistente;
            repuesto->stock += detalle.cantidad;
            repuesto->fechaUltimaModificacion = time(nullptr);
        }
    }

    return ResultadoArchivoDetalles::Correcto;
}

ResultadoArchivoDetalles actualizarDetallePorID(int id, const DetalleArchivo& detalle) {
    if (detalle.idDetalle != id) return ResultadoArchivoDetalles::IdInvalido;

    ArchivoHeader header;
    ResultadoArchivoDetalles resultado = leerHeaderDetalles(header);
    if (resultado != ResultadoArchivoDetalles::Correcto) return resultado;
    if (id < 1 || id > header.cantidadRegistros || id >= header.proximoID) {
        return ResultadoArchivoDetalles::IdInvalido;
    }

    const streamoff posicion =
        static_cast<streamoff>(sizeof(ArchivoHeader)) +
        static_cast<streamoff>(id - 1) * static_cast<streamoff>(sizeof(DetalleArchivo));
    std::fstream archivo(ARCHIVO_DETALLES, ios::in | ios::out | ios::binary);
    if (!archivo.is_open()) return ResultadoArchivoDetalles::ArchivoNoDisponible;

    DetalleArchivo existente;
    archivo.seekg(posicion, ios::beg);
    if (!archivo) return ResultadoArchivoDetalles::ErrorLectura;
    archivo.read(reinterpret_cast<char*>(&existente), sizeof(DetalleArchivo));
    if (!archivo) return ResultadoArchivoDetalles::ErrorLectura;
    if (existente.idDetalle != id) return ResultadoArchivoDetalles::RegistroInconsistente;
    if (existente.idOrden != detalle.idOrden) return ResultadoArchivoDetalles::RegistroInconsistente;

    DetalleArchivo actualizado = detalle;
    actualizado.idDetalle = id;
    actualizado.fechaCreacion = existente.fechaCreacion;
    actualizado.fechaUltimaModificacion = time(nullptr);
    bool actualizarEncabezado = false;
    if (!existente.eliminado && actualizado.eliminado) {
        if (header.registrosActivos == 0) return ResultadoArchivoDetalles::EncabezadoInvalido;
        --header.registrosActivos;
        actualizarEncabezado = true;
    } else if (existente.eliminado && !actualizado.eliminado) {
        if (header.registrosActivos >= header.cantidadRegistros) {
            return ResultadoArchivoDetalles::EncabezadoInvalido;
        }
        ++header.registrosActivos;
        actualizarEncabezado = true;
    }

    archivo.clear();
    archivo.seekp(posicion, ios::beg);
    if (!archivo) return ResultadoArchivoDetalles::ErrorEscritura;
    archivo.write(reinterpret_cast<const char*>(&actualizado), sizeof(DetalleArchivo));
    if (!archivo) return ResultadoArchivoDetalles::ErrorEscritura;
    archivo.close();

    if (actualizarEncabezado) return actualizarHeaderDetalles(header);
    return ResultadoArchivoDetalles::Correcto;
}

void guardarDatos(SistemaTaller& s) {
    {
        std::ofstream archivo(ARCHIVO_VEHICULOS, ios::out | ios::binary);
        if (archivo.is_open()) {
            int activos = 0;
            for (int i = 0; i < s.numVehiculos; i++) {
                if (!s.vehiculos[i].eliminado) activos++;
            }
            ArchivoHeader header = { s.numVehiculos, s.siguienteIdVehiculos, activos, VERSION_ARCHIVO };
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
            int activos = 0;
            for (int i = 0; i < s.numMecanicos; i++) {
                if (!s.mecanicos[i].eliminado) activos++;
            }
            ArchivoHeader header = { s.numMecanicos, s.siguienteIdMecanicos, activos, VERSION_ARCHIVO };
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
            int activos = 0;
            for (int i = 0; i < s.numRepuestos; ++i) {
                if (!s.repuestos[i].eliminado) activos++;
            }
            ArchivoHeader header = { s.numRepuestos, s.siguienteIdRepuestos, activos, VERSION_ARCHIVO };
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
            int activos = 0;
            for (int i = 0; i < s.numOrdenes; ++i) {
                if (!s.ordenes[i].eliminado) activos++;
            }
            ArchivoHeader header = { s.numOrdenes, s.siguienteIdOrdenes, activos, VERSION_ARCHIVO };
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
        std::ifstream existente(ARCHIVO_DETALLES, ios::in | ios::binary);
        if (!existente.is_open()) {
            std::ofstream nuevo(ARCHIVO_DETALLES, ios::out | ios::binary | ios::app);
            if (!nuevo.is_open()) {
                cerr << "[!] No se pudo crear el archivo de detalles." << endl;
                return;
            }

            ArchivoHeader header = {0, 1, 0, VERSION_ARCHIVO};
            nuevo.write(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
            if (!nuevo) {
                cerr << "[!] No se pudo inicializar el archivo de detalles." << endl;
                return;
            }
        }
    }

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
ResultadoArchivoDetalles calcularTotalOrden(const OrdenServicio* orden, float& total);
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
    int activos = 0;
    for (int i = 0; i < s->numVehiculos; i++) {
        if (!s->vehiculos[i].eliminado) activos++;
    }
    *cantidad = activos;
    if (activos == 0) return nullptr;

    Vehiculo** lista = new Vehiculo*[activos];
    int j = 0;
    for (int i = 0; i < s->numVehiculos; i++) {
        if (!s->vehiculos[i].eliminado) {
            lista[j++] = &(s->vehiculos[i]);
        }
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
    Vehiculo* v = buscarVehiculoPorID(s, id);
    if (v == nullptr || v->eliminado) return false;

    for (int i = 0; i < s->numOrdenes; ++i) {
        if (s->ordenes[i].idVehiculo == id) return false;
    }

    v->eliminado = true;
    v->fechaUltimaModificacion = time(nullptr);

    const streamoff posicion = 
        static_cast<streamoff>(sizeof(ArchivoHeader)) +
        static_cast<streamoff>(id - 1) * static_cast<streamoff>(sizeof(Vehiculo));

    std::fstream archivo(ARCHIVO_VEHICULOS, ios::in | ios::out | ios::binary);
    if (!archivo.is_open()) return false;

    archivo.seekp(posicion, ios::beg);
    if (!archivo) {
        archivo.close();
        return false;
    }
    archivo.write(reinterpret_cast<const char*>(v), sizeof(Vehiculo));
    archivo.close();

    ArchivoHeader header;
    std::ifstream lector(ARCHIVO_VEHICULOS, ios::in | ios::binary);
    if (lector.is_open()) {
        lector.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
        lector.close();
    }

    if (header.registrosActivos > 0) {
        header.registrosActivos--;
        std::fstream escritor(ARCHIVO_VEHICULOS, ios::in | ios::out | ios::binary);
        if (escritor.is_open()) {
            escritor.seekp(0, ios::beg);
            escritor.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
            escritor.close();
        }
    }

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

bool eliminarMecanico(SistemaTaller* s, int id) {
    Mecanico* m = buscarMecanicoporID(s, id);
    if (m == nullptr || m->eliminado) return false;

    if (contarOrdenesAbiertasMecanico(s, id) > 0) return false;

    m->eliminado = true;
    m->fechaUltimaModificacion = time(nullptr);

    const streamoff posicion =
        static_cast<streamoff>(sizeof(ArchivoHeader))+
        static_cast<streamoff>(id - 1) * static_cast<streamoff>(sizeof(Mecanico));

    std::fstream archivo(ARCHIVO_MECANICOS, ios::in | ios::out | ios::binary);
    if (!archivo.is_open()) return false;
    archivo.seekp(posicion, ios::beg);
    if (!archivo) {
        archivo.close();
        return false;
    }
    archivo.write(reinterpret_cast<const char*>(m), sizeof(Mecanico));
    archivo.close();

    ArchivoHeader header;
    std::ifstream lector(ARCHIVO_MECANICOS, ios::in | ios::binary);
    if (lector.is_open()) {
        lector.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
        lector.close();
    }

    if (header.registrosActivos > 0) {
        header.registrosActivos--;
        std::fstream escritor(ARCHIVO_MECANICOS, ios::in | ios::out | ios::binary);
        if (escritor.is_open()) {
            escritor.seekp(0, ios::beg);
            escritor.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
            escritor.close();
        }
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

void menuEliminarMecanico(SistemaTaller* s) {
    limpiarTerminal();
    int id = 0;
    cout << "\n--- ELIMINAR MECANICO ---\n";
    mostrarOpcionVolver();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (Validaciones::leerEnteroRango("ID: ", 1, numeric_limits<int>::max(), id) !=
        ResultadoEntrada::Correcto) return;
    if (!confirmarAccion("Eliminar mecanico")) return;
    cout << (eliminarMecanico(s, id) ? "[OK] Mecanico eliminado." :
             "[!] No existe, ya esta eliminado o tiene ordenes abiertas asignadas.") << endl;
}

void mostrarListaMecanico(SistemaTaller* s) {
    int activos = 0;
    for (int i = 0; i < s->numMecanicos; i++) {
        if (!s->mecanicos[i].eliminado) activos++;
    }

    if (activos == 0) {
        cout << "\n[!] No hay mecanicos registados para mostrar." << endl;
        return;
    }

    cout << "\n==== LISTA DE MECANICOS (" << activos << ") =====" << endl;
    for (int i = 0; i < s->numMecanicos; i++) {
        if (!s->mecanicos[i].eliminado) {
            mostrarMecanicoUnico(&s->mecanicos[i]);
            cout << "Ordenes Abiertas: " << contarOrdenesAbiertasMecanico(s, s->mecanicos[i].id) << endl;
        }
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

    DetalleArchivo detalle;
    ResultadoArchivoDetalles resultado =
        buscarDetalleOrdenRepuesto(idOrden, idRepuesto, detalle);
    if (resultado == ResultadoArchivoDetalles::Correcto) {
        if (detalle.cantidad > numeric_limits<int>::max() - cantidad) return false;
        detalle.cantidad += cantidad;
        resultado = actualizarDetallePorID(detalle.idDetalle, detalle);
    } else if (resultado == ResultadoArchivoDetalles::RegistroNoEncontrado) {
        DetalleArchivo nuevo = {};
        nuevo.idOrden = idOrden;
        nuevo.idRepuesto = r->id;
        strcpy(nuevo.codigoRepuesto, r->codigo);
        strcpy(nuevo.nombreRepuesto, r->nombre);
        nuevo.precioUnitario = r->precio;
        nuevo.cantidad = cantidad;
        resultado = crearDetalleArchivo(nuevo);
    }

    if (resultado != ResultadoArchivoDetalles::Correcto) return false;
    r->stock -= cantidad;
    r->fechaUltimaModificacion = time(nullptr);
    return true;
}

bool quitarRepuestoDeOrden(SistemaTaller* s, int idOrden, int idRepuesto) {
    OrdenServicio* o = buscarOrdenPorID(s, idOrden);
    if (o == nullptr || strcmp(o->estado, "Abierta") != 0) return false;

    DetalleArchivo detalle;
    if (buscarDetalleOrdenRepuesto(idOrden, idRepuesto, detalle) !=
        ResultadoArchivoDetalles::Correcto) return false;
    Repuesto* r = buscarRepuestoPorID(s, idRepuesto);
    if (r == nullptr || detalle.cantidad <= 0 ||
        r->stock > numeric_limits<int>::max() - detalle.cantidad) return false;

    detalle.eliminado = true;
    if (actualizarDetallePorID(detalle.idDetalle, detalle) !=
        ResultadoArchivoDetalles::Correcto) return false;
    r->stock += detalle.cantidad;
    r->fechaUltimaModificacion = time(nullptr);
    return true;
}

bool cerrarOrden(SistemaTaller* s, int idOrden, float manoObra, const char* fechaActual) {
    OrdenServicio* o = buscarOrdenPorID(s, idOrden);
    if (o == nullptr || strcmp(o->estado, "Abierta") != 0 || manoObra < 0.0f ||
        fechaActual == nullptr || !fechaValida(fechaActual)) return false;

    size_t cantidadDetalles = 0;
    if (contarDetallesDeOrden(idOrden, cantidadDetalles) !=
            ResultadoArchivoDetalles::Correcto ||
        (manoObra == 0.0f && cantidadDetalles == 0)) return false;

    o->manoObra = manoObra;
    strcpy(o->fechaCierre, fechaActual);
    strcpy(o->estado, "Cerrada");
    o->fechaUltimaModificacion = time(nullptr);
    return true;
}

bool anularOrden(SistemaTaller* s, int idOrden) {
    OrdenServicio* o = buscarOrdenPorID(s, idOrden);
    if (o == nullptr ||
        (strcmp(o->estado, "Abierta") != 0 && strcmp(o->estado, "Cerrada") != 0)) {
        return false;
    }
    if (validarRestitucionStockOrden(s, idOrden) != ResultadoArchivoDetalles::Correcto ||
        restituirStockOrden(s, idOrden) != ResultadoArchivoDetalles::Correcto) return false;
    strcpy(o->estado, "Anulada");
    o->fechaUltimaModificacion = time(nullptr);
    return true;
}

ResultadoArchivoDetalles calcularTotalOrden(const OrdenServicio* orden, float& total) {
    total = 0.0f;
    if (orden == nullptr) return ResultadoArchivoDetalles::ParametrosInvalidos;

    float costoRepuestos = 0.0f;
    ResultadoArchivoDetalles resultado =
        calcularCostoRepuestosOrden(orden->id, costoRepuestos);
    if (resultado != ResultadoArchivoDetalles::Correcto) return resultado;
    total = orden->manoObra + costoRepuestos;
    return ResultadoArchivoDetalles::Correcto;
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

    DetalleArchivo detalles[MAX_DETALLES_POR_CONSULTA];
    size_t cantidadDetalles = 0;
    bool hayMasDetalles = false;
    ResultadoArchivoDetalles resultado = buscarDetallesDeOrden(
        orden->id,
        detalles,
        MAX_DETALLES_POR_CONSULTA,
        cantidadDetalles,
        hayMasDetalles);
    if (resultado == ResultadoArchivoDetalles::Correcto) {
        cout << " --- REPUESTOS UTILIZADOS (" << cantidadDetalles;
        if (hayMasDetalles) cout << "+";
        cout << ") ---" << endl;
        for (size_t i = 0; i < cantidadDetalles; ++i) {
            cout << "  - " << detalles[i].nombreRepuesto
                 << " x" << detalles[i].cantidad
                 << " ($" << detalles[i].precioUnitario << " c/u)" << endl;
        }
        if (hayMasDetalles) {
            cout << "  [!] Se muestran los primeros " << cantidadDetalles
                 << " detalles." << endl;
        }
    } else {
        cout << " [!] No se pudo leer el detalle de repuestos de la orden." << endl;
    }

    float total = 0.0f;
    cout << " Mano de Obra: $" << orden->manoObra << endl;
    resultado = calcularTotalOrden(orden, total);
    if (resultado == ResultadoArchivoDetalles::Correcto) {
        cout << " TOTAL A PAGAR: $" << total << endl;
    } else {
        cout << " [!] No se pudo calcular el total de la orden." << endl;
    }
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
        float total = 0.0f;
        ResultadoArchivoDetalles resultado = calcularTotalOrden(ordenes[i], total);
        cout << "[ID Orden: " << ordenes[i]->id << "] - Vehiculo ID: " << ordenes[i]->idVehiculo
             << " - Estado: " << ordenes[i]->estado << " - Total: ";
        if (resultado == ResultadoArchivoDetalles::Correcto) {
            cout << "$" << total << endl;
        } else {
            cout << "[no disponible: error al leer detalles]" << endl;
        }
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

        int vehiculoActivos = 0;
        for (int i = 0; i < s->numVehiculos; i++) {
            if (!s->vehiculos[i].eliminado) vehiculoActivos++;
        }

        cout << "\n--- VEHICULOS ---\n1. Registrar\n";
        if (vehiculoActivos > 0) {
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
        if (vehiculoActivos == 0 && opcion != 0 && opcion != 1) {
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
        int mecanicosActivos = 0;
        for (int i = 0; i < s->numMecanicos; i++) {
            if (!s->mecanicos[i].eliminado) mecanicosActivos++;
        }

        cout << "\n--- MECANICOS ---\n1. Registrar\n";
        if (mecanicosActivos > 0) {
            cout << "2. Listar\n3. Eliminar\n";
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
        if (mecanicosActivos == 0 && opcion != 0 && opcion != 1) {
            cout << "[!] Opcion invalida." << endl;
            continue;
        }
        switch (opcion) {
            case 1: menuRegistrarMecanico(s); pausarParaContinuar(); break;
            case 2: menuListarMecanico(s); pausarParaContinuar(); break;
            case 3: menuEliminarMecanico(s); pausarParaContinuar(); break;
            case 0: break;
            default: cout << "[!] Opcion invalida." << endl;
        }
    }
}

void menuRepuestos(SistemaTaller* s) {
    int opcion = -1;
    while (opcion != 0) {
        limpiarTerminal();
        int repuestosActivos = 0;
        for (int i = 0; i < s->numRepuestos; i++) {
            if (!s->repuestos[i].eliminado) repuestosActivos++;
        }

        cout << "\n--- REPUESTOS ---\n1. Registrar\n";
        if (repuestosActivos > 0) {
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
        if (repuestosActivos == 0 && opcion != 0 && opcion != 1) {
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
    int vehiculosActivos = 0; for(int i=0; i<s->numVehiculos; ++i) if(!s->vehiculos[i].eliminado) vehiculosActivos++;
    int mecanicosActivos = 0; for(int i=0; i<s->numMecanicos; ++i) if(!s->mecanicos[i].eliminado) mecanicosActivos++;
    int repuestosActivos = 0; for(int i=0; i<s->numRepuestos; ++i) if(!s->repuestos[i].eliminado) repuestosActivos++;

    if (vehiculosActivos == 0 || mecanicosActivos == 0 || repuestosActivos == 0) return;

    int opcion = -1;
    while (opcion != 0) {
        limpiarTerminal();
        int ordenesActivas = 0;
        for (int i = 0; i < s->numOrdenes; i++) {
            if (!s->ordenes[i].eliminado) ordenesActivas++;
        }

        cout << "\n--- ORDENES DE SERVICIO ---\n1. Abrir orden\n";
        if (ordenesActivas > 0) {
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
        if (ordenesActivas == 0 && opcion != 0 && opcion != 1) {
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

//MODULO DE MANTENIMIENTO
void crearBackup() {
    time_t ahora = time(nullptr);
    tm* local = localtime(&ahora);
    char carpeta[64];
    strftime(carpeta, sizeof(carpeta), "backup_%Y-%m-%d_%H-%M-%S", local);

    string cmd = "mkdir ";
#ifdef _WIN32
    cmd = "mkdir " + string(carpeta);
#else 
    cmd = "mkdir -p " + string(carpeta);
#endif
    system(cmd.c_str());

    const char* archivos[] = {
        "taller.bin",
        "vehiculos.bin",
        "mecanicos.bin",
        "ordenes.bin",
        "repuestos.bin",
        "detalles.bin",
    };

    for (const char* nombreArch : archivos) {
        ifstream origen(nombreArch, ios::binary);
        if (!origen.is_open()) continue;

        string rutaDestino = string(carpeta) + "/" + nombreArch;
        ofstream destino(rutaDestino, ios::binary);
        if (destino.is_open()) {
            destino << origen.rdbuf();
            destino.close();
        }
        origen.close();
    }

    cout << "[OK] Respaldo creado correctamente en la carpeta: " << carpeta << endl;
}

void verificarIntegridad() {
    limpiarTerminal();
    cout << "\n==== INICIANDO VERIFICACION DE INTEGRIDAD ====" << endl;

    int vehiculosVerificados = 0;
    int mecanicosVerificados = 0;
    int repuestosVerificados = 0;
    int ordenesVerificados = 0;
    int problemasEncontrados = 0;

    {
        ifstream f(ARCHIVO_VEHICULOS, ios::binary);
        if (f.is_open()) {
            ArchivoHeader h;
            f.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));

            f.seekg(0, ios::end);
            streamoff tam = f.tellg();
            streamoff esperado = sizeof(ArchivoHeader) + static_cast<streamoff>(h.cantidadRegistros) * sizeof(Vehiculo);
            if (tam != esperado) {
                cout << "[ROTO] Header de vehiculos.bin indica " << h.cantidadRegistros << " registros, pero el espacio en disco no coincide." << endl;
                problemasEncontrados++;
            }

            f.seekg(sizeof(ArchivoHeader), ios::beg);
            for (int i = 0; i < h.cantidadRegistros; ++i) {
                Vehiculo v;
                f.read(reinterpret_cast<char*>(&v), sizeof(Vehiculo));
                vehiculosVerificados++;
            }
            f.close();
        }
    }

    {
        ifstream f(ARCHIVO_MECANICOS, ios::binary);
        if (f.is_open()) {
            ArchivoHeader h;
            f.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));
            f.seekg(sizeof(ArchivoHeader), ios::beg);
            for (int i = 0; i < h.cantidadRegistros; ++i) {
                Mecanico m;
                f.read(reinterpret_cast<char*>(&m), sizeof(Mecanico));
                mecanicosVerificados++;
            }
            f.close();
        }
    }

    {
        ifstream f(ARCHIVO_REPUESTOS, ios::binary);
        if (f.is_open()) {
            ArchivoHeader h;
            f.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));
            f.seekg(sizeof(ArchivoHeader), ios::beg);
            for (int i = 0; i < h.cantidadRegistros; ++i) {
                Repuesto r;
                f.read(reinterpret_cast<char*>(&r), sizeof(Repuesto));
                repuestosVerificados++;
                if (r.stock < 0) {
                    cout << "[ROTO] Repuesto ID " << r.id << " (" << r.codigo << ") tiene stock negativo: " << r.stock << endl;
                    problemasEncontrados++;
                }
            }
            f.close();
        }
    }

    {
        ifstream f(ARCHIVO_ORDENES, ios::binary);
        if (f.is_open()) {
            ArchivoHeader h;
            f.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));
            f.seekg(sizeof(ArchivoHeader), ios::beg);
            for (int i = 0; i < h.cantidadRegistros; ++i) {
                OrdenServicio o;
                f.read(reinterpret_cast<char*>(&o), sizeof(OrdenServicio));
                ordenesVerificados++;

                Vehiculo v;
                bool vExiste = false;
                {
                    ifstream fv(ARCHIVO_VEHICULOS, ios::binary);
                    if (fv.is_open()) {
                        ArchivoHeader hv; fv.read(reinterpret_cast<char*>(&hv), sizeof(ArchivoHeader));
                        for (int j = 0; j < hv.cantidadRegistros; ++j) {
                            fv.read(reinterpret_cast<char*>(&v), sizeof(Vehiculo));
                            if (v.id == o.idVehiculo) { vExiste = true; break; }
                        }
                        fv.close();
                    }
                }

                if (!vExiste) {
                    cout << "[ROTO] Orden#" << o.id << " apunta al vehiculo ID " << o.idVehiculo << " (no existe en vehiculos.bin)" << endl;
                    problemasEncontrados++;
                } else if (strcmp(o.estado, "Abierta") == 0 && v.eliminado) {
                    cout << "[ROTO] Orden ABIERTA #" << o.id << " apunta a un vehiculo inactivo/eliminado (ID " << v.id << ")" << endl;
                    problemasEncontrados++;
                }

                Mecanico m;
                bool mExiste = false;
                {
                    ifstream fm(ARCHIVO_MECANICOS, ios::binary);
                    if (fm.is_open()) {
                        ArchivoHeader hm; fm.read(reinterpret_cast<char*>(&hm), sizeof(ArchivoHeader));
                        for (int j = 0; j < hm.cantidadRegistros; ++j) {
                            fm.read(reinterpret_cast<char*>(&m), sizeof(Mecanico));
                            if (m.id == o.idMecanico) { mExiste = true; break; }
                        }
                        fm.close();
                    }
                }

                if (!mExiste) {
                    cout << "[ROTO] Orden #" << o.id << " apunta al mecanico ID" << o.idMecanico << " (no existe en mecanicos.bin)" << endl;
                    problemasEncontrados++;
                } else if (strcmp(o.estado, "Abierta") == 0 && m.eliminado) {
                   cout << "[ROTO] Orden ABIERTA #" << o.id << " apunta a un mecanico dado de baja (ID " << m.id << ")" << endl;
                   problemasEncontrados++;   
                }
            }
            f.close();
        }
    }

    cout << "\nREPORTE DE INTEGRIDAD DEL SISTEMA" << endl;
    cout << "Vehiculos verificados : " << vehiculosVerificados << endl;
    cout << "Mecanicos verificados : " << mecanicosVerificados << endl;
    cout << "Repuestos verificados : " << repuestosVerificados << endl;
    cout << "Ordenes verificadas   : " << ordenesVerificados << endl;
    cout << "Problemas encontrados : " << problemasEncontrados << endl;
    
    if (problemasEncontrados == 0) {
        cout << "Estado: SISTEMA ÍNTEGRO" << endl;
    } else {
        cout << "Estado: SE ENCONTRARON INCONSISTENCIAS" << endl;
    }
    
    pausarParaContinuar();
}

void menuMantenimiento(SistemaTaller* s) {
    (void)s;
    int opcion = -1;
    while (opcion != 0) {
        limpiarTerminal();
        cout << "\n--- MANTENIMIENTO ---\n"
             << "1. Verificacion de integridad\n"
             << "2. Crear respaldo (Backup)\n"
             << "0. Volver\nOpcion: ";
        if (!(cin >> opcion)) {
            if (cin.eof()) return;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            entradaConSaltoPendiente = false;
            continue;
        }
        entradaConSaltoPendiente = true;
        switch (opcion) {
            case 1: verificarIntegridad(); break;
            case 2: crearBackup(); pausarParaContinuar(); break;
            case 0: break;
            default: cout << "[!] Opcion invalida." << endl;
        }
    }
}

//MODULO DE REPORTES
void reporteHistorialVehiculo(SistemaTaller* s) {
    limpiarTerminal();
    char placa[10] = {};
    cout << "\n==== REPORTE 1: HISTORIAL DE VEHICULO ====" << endl;
    mostrarOpcionVolver();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (Validaciones::leerTexto("Ingrese la placa del vehiculo: ", placa, sizeof(placa)) != ResultadoEntrada::Correcto) return;
    Validaciones::convertirAMayusculas(placa);

    Vehiculo* v = buscarVehiculoPorPlaca(s, placa);
    if (v == nullptr) {
        cout << "[!] No se encontro un vehiculo activo con esa placa." << endl;
        pausarParaContinuar();
        return;
    }

    mostrarVehiculo(v);
    cout << "\n--- ORDENES ASOCIADAS AL VEHICULO ---" << endl;

    float totalFacturadoCerradas = 0.0f;
    int contarOrdenes = 0;

    for (int i = 0; i < s->numOrdenes; i++) {
        if (!s->ordenes[i].eliminado && s->ordenes[i].idVehiculo == v->id) {
            contarOrdenes++;
            Mecanico* m = buscarMecanicoporID(s, s->ordenes[i].idMecanico);
            float totalOrden = 0.0f;
            calcularTotalOrden(&s->ordenes[i], totalOrden);

            cout << "Orden #" << s->ordenes[i].id
                 << " | Fecha: " << s->ordenes[i].fechaApertura
                 << " | Estado: " << s->ordenes[i].estado
                 << " | Mecanico: " << (m != nullptr ? m->nombre : "[Desconocido]")
                 << " | Total: $" << totalOrden << endl;
                 
            if (igualSinMayusculas(s->ordenes[i].estado, "Cerrada")) {
                totalFacturadoCerradas += totalOrden;
            }
        }
    }

    if (contarOrdenes == 0) {
        cout << "[!] Este vehiculo no registra ordenes de servicio." << endl;
    } else {
        cout << "--------------------------------------------------" << endl;
        cout << "TOTAL FACTURADO EN ORDENES CERRADAS: $" << totalFacturadoCerradas << endl;
    }
    pausarParaContinuar();
}

void reporteRepuestosMasUtilizados(SistemaTaller* s) {
    limpiarTerminal();
    cout << "\n ==== REPORTE 2: REPUESTOS MAS UTILIZADOS (TOP 10) ====" << endl;

    struct ConsumoRepuesto {
        int idRepuesto;
        char codigo[20];
        char nombre[100];
        int unidadesTotales;
    };

    int maxR = s->numRepuestos;
    if (maxR == 0) {
        cout << "\n[!] No hay repuestos registrados." << endl;
        pausarParaContinuar();
        return;
    }

    ConsumoRepuesto* ranking = new ConsumoRepuesto[maxR];
    int totalRanking = 0;

    for (int i = 0; i < s->numRepuestos; ++i) {
        ranking[i].idRepuesto = s->repuestos[i].id;
        strcpy(ranking[i].codigo, s->repuestos[i].codigo);
        strcpy(ranking[i].nombre, s->repuestos[i].nombre);
        ranking[i].unidadesTotales = 0;
        totalRanking++;
    }

    ArchivoHeader hDetalles;
    if (leerHeaderDetalles(hDetalles) == ResultadoArchivoDetalles::Correcto && hDetalles.cantidadRegistros > 0) {
        std::ifstream archivoDetalles(ARCHIVO_DETALLES, ios::in | ios::binary);
        if (archivoDetalles.is_open()) {
            archivoDetalles.seekg(sizeof(ArchivoHeader), ios::beg);
            for (int i = 0; i < hDetalles.cantidadRegistros; ++i) {
                DetalleArchivo det;
                archivoDetalles.read(reinterpret_cast<char*>(&det), sizeof(DetalleArchivo));
                if (!det.eliminado) {
                   
                    OrdenServicio* ord = buscarOrdenPorID(s, det.idOrden);
                    if (ord != nullptr && !ord->eliminado && igualSinMayusculas(ord->estado, "Cerrada")) {
                        
                        for (int j = 0; j < totalRanking; ++j) {
                            if (ranking[j].idRepuesto == det.idRepuesto) {
                                ranking[j].unidadesTotales += det.cantidad;
                                break;
                            }
                        }
                    }
                }
            }
            archivoDetalles.close();
        }
    }

    for (int i = 0; i < totalRanking - 1; ++i) {
        for (int j = 0; j < totalRanking - i - 1; ++j) {
            if (ranking[j].unidadesTotales < ranking[j + 1].unidadesTotales) {
                ConsumoRepuesto temp = ranking[j];
                ranking[j] = ranking[j + 1];
                ranking[j + 1] = temp;
            }
        }
    }

    int mostrarLimite = (totalRanking < 10) ? totalRanking : 10;
    int mostradosReales = 0;

    cout << left << setw(8) << "ID" << setw(15) << "Codigo" << setw(35) << "Nombre" << "Unidades Usadas" << endl;
    cout << "------------------------------------------------------------------" << endl;
    for (int i = 0; i < mostrarLimite; ++i) {
        if (ranking[i].unidadesTotales > 0) {
            cout << left << setw(8) << ranking[i].idRepuesto 
                 << setw(15) << ranking[i].codigo 
                 << setw(35) << ranking[i].nombre 
                 << ranking[i].unidadesTotales << endl;
            mostradosReales++;
        }
    }

    if (mostradosReales == 0) {
        cout << "[!] No hay consumo de repuestos en ordenes cerradas todavia." << endl;
    }

    delete[] ranking;
    pausarParaContinuar();
}

void reporteFacturacionMecanicos(SistemaTaller* s) {
    limpiarTerminal();
    cout << "\n==== REPORTE 3: FACTURACION POR MECANICO ====" << endl;
    cout << left << setw(25) << "Mecanico" << setw(12) << "Ordenes" << setw(15) << "M. obra ($)" << "Repuestos ($)" << endl;
    cout << "-------------------------------------------------------------------" << endl;

    int mecanicosProcesados = 0;
    for (int i = 0; i < s->numMecanicos; ++i) {
        Mecanico m = s->mecanicos[i];
        
        int ordenesCerradas = 0;
        float totalManoObra = 0.0f;
        float totalRepuestosMec = 0.0f;

        for (int j = 0; j < s->numOrdenes; ++j) {
            if (!s->ordenes[j].eliminado && s->ordenes[j].idMecanico == m.id && igualSinMayusculas(s->ordenes[j].estado, "Cerrada")) {
                ordenesCerradas++;
                totalManoObra += s->ordenes[j].manoObra;
                float costoRep = 0.0f;
                calcularCostoRepuestosOrden(s->ordenes[j].id, costoRep);
                totalRepuestosMec += costoRep;
            }
        }

        char nombreConEstado[120];
        if (m.eliminado) {
            sprintf(nombreConEstado, "%s (BAJA)", m.nombre);
        } else {
            strcpy(nombreConEstado, m.nombre);
        }

        cout << left << setw(25) << nombreConEstado 
             << setw(12) << ordenesCerradas 
             << setw(15) << fixed << setprecision(2) << totalManoObra 
             << totalRepuestosMec << endl;
        mecanicosProcesados++;
    }

    if (mecanicosProcesados == 0) {
        cout << "[!] No hay mecanicos registrados en el sistema." << endl;
    }
    pausarParaContinuar();
}

void menuReportes(SistemaTaller* s) {
    int opcion = -1;
    while (opcion != 0) {
        limpiarTerminal();
        cout << "\n--- MODULO DE REPORTES ---\n"
             << "1. Historial de vehiculo por placa\n"
             << "2. Repuestos mas utilizados (Top 10)\n"
             << "3. Facturacion y carga por mecanico\n"
             << "0. Volver al menu principal\n"
             << "Opcion: ";
        
        if (!(cin >> opcion)) {
            if (cin.eof()) return;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            entradaConSaltoPendiente = false;
            cout << "[!] Ingrese un numero valido." << endl;
            continue;
        }
        entradaConSaltoPendiente = true;

        switch (opcion) {
            case 1: reporteHistorialVehiculo(s); break;
            case 2: reporteRepuestosMasUtilizados(s); break;
            case 3: reporteFacturacionMecanicos(s); break;
            case 0: break;
            default: cout << "[!] Opcion invalida." << endl; pausarParaContinuar();
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

        int vehiculosActivos = 0; for(int i=0; i<sistema.numVehiculos; ++i) if(!sistema.vehiculos[i].eliminado) vehiculosActivos++;
        int mecanicosActivos = 0; for(int i=0; i<sistema.numMecanicos; ++i) if(!sistema.mecanicos[i].eliminado) mecanicosActivos++;
        int repuestosActivos = 0; for(int i=0; i<sistema.numRepuestos; ++i) if(!sistema.repuestos[i].eliminado) repuestosActivos++;

        cout << "\n=== " << sistema.taller.nombre << " ===\n"
             << "1. Vehiculos\n2. Mecanicos\n3. Repuestos\n";
        if (vehiculosActivos > 0 && mecanicosActivos > 0 && repuestosActivos > 0) {
            cout << "4. Ordenes de servicio\n";
        }
        cout << "5. Lista general de registros\n6. Mantenimeinto\n7. Reportes\n0. Salir\nOpcion: ";
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
                if (vehiculosActivos > 0 && mecanicosActivos > 0 && repuestosActivos > 0) {
                    menuOrdenes(&sistema);
                } else {
                    cout << "[!] Registre al menos un vehiculo, un mecanico y un repuesto antes de gestionar ordenes." << endl;
                    pausarParaContinuar();
                }
                break;
            case 5: menuListaGeneral(&sistema); break;
            case 6: menuMantenimiento(&sistema); break;
            case 7: menuReportes(&sistema); break;
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
