#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "Grafo.h"

// Nodo de la lista enlazada dinamica que representa el camino resultante.
struct NodoRuta {
    Nodo* nodo;
    NodoRuta* siguiente;
    NodoRuta(Nodo* n) : nodo(n), siguiente(nullptr) {}
};

// Resultado de calcular una ruta: lista enlazada del camino + distancia total.
// Quien reciba un ResultadoRuta es responsable de liberarlo con Dijkstra::liberarRuta.
struct ResultadoRuta {
    NodoRuta* camino;
    double distanciaTotal;
    bool encontrada;
};

class Dijkstra {
public:
    // Calcula la ruta mas corta entre idOrigen e idDestino sobre el grafo dado.
    // Solo considera aristas abiertas (estado == abierta).
    static ResultadoRuta calcularRuta(const Grafo& grafo, int idOrigen, int idDestino);

    static void liberarRuta(ResultadoRuta& resultado);
    static void imprimirRuta(const ResultadoRuta& resultado);
};

#endif // DIJKSTRA_H
