#ifndef CARGADOR_DATOS_H
#define CARGADOR_DATOS_H

#include "Grafo.h"

// Responsable de poblar un Grafo con el catalogo predefinido de puntos
// del campus ESPE Sangolqui (coordenadas reales aproximadas) y sus conexiones.
// El grafo resultante sigue siendo dueño de su propia memoria dinamica;
// esta clase solo construye y entrega los datos, no los almacena.
class CargadorDatos {
public:
    // Cantidad total de nodos disponibles en el catalogo interno del campus.
    static int totalNodosDisponibles();

    // Construye un grafo nuevo (memoria dinamica) cargando unicamente los
    // primeros `cantidad` nodos del catalogo, mas las conexiones entre ellos.
    // El llamador es responsable de liberar el Grafo* con delete.
    static Grafo* construirGrafoESPE(int cantidad);

private:
    static double haversineMetros(double lat1, double lon1, double lat2, double lon2);
};

#endif // CARGADOR_DATOS_H
