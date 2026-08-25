#include "CargadorDatos.h"
#include <cmath>

namespace {

    // Estructura auxiliar de solo datos (tabla de siembra), NO es la estructura
    // interna del sistema (esa es dinamica y vive dentro de Grafo/Nodo/Arista).
    struct DatoNodo {
        int id;
        const char* nombre;
        TipoNodo tipo;
        double lat;
        double lon;
        bool accesible;
    };

    struct DatoArista {
        int idA;
        int idB;
        const char* tipoCamino;
        bool accesible;
        bool abierta;
    };

    // Coordenadas aproximadas dentro del campus ESPE - Sangolqui
    // (base real: aprox. -0.317, -78.448). Los desplazamientos entre puntos
    // son ilustrativos del layout del campus para fines academicos.
    const DatoNodo CATALOGO[] = {
        {  1, "Garita Principal",           TipoNodo::Garita,      -0.317200, -78.447900, true  },
        {  2, "Entrada Peatonal",           TipoNodo::Entrada,     -0.316900, -78.447700, true  },
        {  3, "Parqueadero Norte",          TipoNodo::Parqueadero, -0.316600, -78.447800, true  },
        {  4, "Bloque A - Rectorado",       TipoNodo::Edificio,    -0.316400, -78.447500, true  },
        {  5, "Bloque B - Aulas",           TipoNodo::Edificio,    -0.316200, -78.447300, true  },
        {  6, "Biblioteca Central",         TipoNodo::Biblioteca,  -0.316300, -78.447000, true  },
        {  7, "Cafeteria Central",          TipoNodo::Cafeteria,   -0.316100, -78.446900, true  },
        {  8, "Bloque C - Laboratorios",    TipoNodo::Laboratorio, -0.315900, -78.447100, true  },
        {  9, "Bloque H - Sistemas",        TipoNodo::Edificio,    -0.315700, -78.446800, true  },
        { 10, "Auditorio Politecnico",      TipoNodo::Auditorio,   -0.315600, -78.447200, true  },
        { 11, "Coliseo",                    TipoNodo::Cruce,       -0.315400, -78.447400, true  },
        { 12, "Rampa Acceso Biblioteca",    TipoNodo::Rampa,       -0.316300, -78.446950, true  },
        { 13, "Escalera Bloque C",          TipoNodo::Escalera,    -0.315950, -78.447050, false },
        { 14, "Parqueadero Sur",            TipoNodo::Parqueadero, -0.315200, -78.447500, true  },
    };
    const int TOTAL_CATALOGO = sizeof(CATALOGO) / sizeof(DatoNodo);

    // Conexiones. Se organizan como una cadena principal (garantiza que el
    // grafo quede conectado sea cual sea el prefijo de nodos que se cargue)
    // mas algunos atajos para que la malla se parezca mas a un campus real.
    const DatoArista CONEXIONES[] = {
        {  1,  2, "peatonal",  true,  true },
        {  2,  3, "vehicular", true,  true },
        {  3,  4, "vehicular", true,  true },
        {  4,  5, "peatonal",  true,  true },
        {  5,  6, "peatonal",  true,  true },
        {  6,  7, "peatonal",  true,  true },
        {  7,  8, "peatonal",  true,  true },
        {  8,  9, "peatonal",  true,  true },
        {  9, 10, "peatonal",  true,  true },
        { 10, 11, "peatonal",  true,  true },
        { 11, 12, "peatonal",  true,  true },
        { 12, 13, "peatonal",  true,  true },
        { 13, 14, "peatonal",  true,  true },
        // Atajos que enriquecen la malla (solo se agregan si ambos extremos
        // ya fueron cargados, por eso no rompen la conectividad de prefijos)
        {  4,  6, "peatonal",  true,  true },
        {  6,  9, "peatonal",  true,  true },
        {  6, 12, "rampa",     true,  true },
        {  8, 13, "escalera",  false, true },
        {  9, 11, "peatonal",  true,  true },
    };
    const int TOTAL_CONEXIONES = sizeof(CONEXIONES) / sizeof(DatoArista);

} // namespace

int CargadorDatos::totalNodosDisponibles() {
    return TOTAL_CATALOGO;
}

double CargadorDatos::haversineMetros(double lat1, double lon1, double lat2, double lon2) {
    const double R = 6371000.0; // radio de la Tierra en metros
    const double gradARad = 3.14159265358979323846 / 180.0;

    double dLat = (lat2 - lat1) * gradARad;
    double dLon = (lon2 - lon1) * gradARad;

    double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
               std::cos(lat1 * gradARad) * std::cos(lat2 * gradARad) *
               std::sin(dLon / 2) * std::sin(dLon / 2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    return R * c;
}

Grafo* CargadorDatos::construirGrafoESPE(int cantidad) {
    if (cantidad < 0) cantidad = 0;
    if (cantidad > TOTAL_CATALOGO) cantidad = TOTAL_CATALOGO;

    Grafo* grafo = new Grafo(cantidad);

    for (int i = 0; i < cantidad; ++i) {
        const DatoNodo& d = CATALOGO[i];
        Nodo* nodo = new Nodo(d.id, d.nombre, d.tipo, d.lat, d.lon, d.accesible);
        grafo->agregarNodo(nodo);
    }

    for (int i = 0; i < TOTAL_CONEXIONES; ++i) {
        const DatoArista& c = CONEXIONES[i];
        if (c.idA > cantidad || c.idB > cantidad) continue; // ese nodo aun no fue cargado

        const DatoNodo& a = CATALOGO[c.idA - 1];
        const DatoNodo& b = CATALOGO[c.idB - 1];
        double distancia = haversineMetros(a.lat, a.lon, b.lat, b.lon);

        grafo->agregarArista(c.idA, c.idB, distancia, c.tipoCamino, c.accesible, c.abierta);
    }

    return grafo;
}
