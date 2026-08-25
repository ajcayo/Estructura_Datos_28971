#include "Dijkstra.h"
#include <iostream>
#include <limits>

namespace {
    const double INF = std::numeric_limits<double>::infinity();
}

ResultadoRuta Dijkstra::calcularRuta(const Grafo& grafo, int idOrigen, int idDestino) {
    ResultadoRuta resultado;
    resultado.camino = nullptr;
    resultado.distanciaTotal = INF;
    resultado.encontrada = false;

    int n = grafo.getCantidadNodos();
    if (n == 0) return resultado;

    // Traducimos id -> posicion dentro del arreglo interno del grafo.
    int posOrigen = -1, posDestino = -1;
    for (int i = 0; i < n; ++i) {
        if (grafo.getNodoEnPosicion(i)->getId() == idOrigen) posOrigen = i;
        if (grafo.getNodoEnPosicion(i)->getId() == idDestino) posDestino = i;
    }
    if (posOrigen == -1 || posDestino == -1) return resultado;

    // Arreglos dinamicos dimensionados en TIEMPO DE EJECUCION segun n
    // (no son arreglos de tamaño fijo definidos en compilacion).
    double* distancia = new double[n];
    bool* visitado = new bool[n];
    int* anterior = new int[n];

    for (int i = 0; i < n; ++i) {
        distancia[i] = INF;
        visitado[i] = false;
        anterior[i] = -1;
    }
    distancia[posOrigen] = 0.0;

    for (int iteracion = 0; iteracion < n; ++iteracion) {
        // Buscar el nodo no visitado con menor distancia (version O(V^2), sin heap).
        int u = -1;
        double mejor = INF;
        for (int i = 0; i < n; ++i) {
            if (!visitado[i] && distancia[i] < mejor) {
                mejor = distancia[i];
                u = i;
            }
        }
        if (u == -1) break; // el resto es inalcanzable
        visitado[u] = true;
        if (u == posDestino) break; // llegamos al destino, podemos parar antes

        // Relajar aristas incidentes a u recorriendo su lista de adyacencia dinamica.
        Nodo* nodoU = grafo.getNodoEnPosicion(u);
        const ListaAdyacencia& lista = grafo.getListaAdyacenciaEnPosicion(u);
        ListaAdyacencia::NodoLista* actual = lista.getCabeza();
        while (actual != nullptr) {
            if (actual->arista->estaAbierta()) {
                Nodo* vecino = actual->arista->obtenerVecino(nodoU->getId());
                // Buscar la posicion del vecino
                int v = -1;
                for (int i = 0; i < n; ++i) {
                    if (grafo.getNodoEnPosicion(i)->getId() == vecino->getId()) { v = i; break; }
                }
                if (v != -1 && !visitado[v]) {
                    double nuevaDist = distancia[u] + actual->arista->getDistancia();
                    if (nuevaDist < distancia[v]) {
                        distancia[v] = nuevaDist;
                        anterior[v] = u;
                    }
                }
            }
            actual = actual->siguiente;
        }
    }

    if (distancia[posDestino] < INF) {
        resultado.encontrada = true;
        resultado.distanciaTotal = distancia[posDestino];

        // Reconstruir el camino hacia atras (destino -> ... -> origen)
        // y luego invertir el orden de insercion para que quede origen -> ... -> destino.
        NodoRuta* invertido = nullptr;
        int actualPos = posDestino;
        while (actualPos != -1) {
            NodoRuta* nuevo = new NodoRuta(grafo.getNodoEnPosicion(actualPos));
            nuevo->siguiente = invertido;
            invertido = nuevo;
            actualPos = anterior[actualPos];
        }
        resultado.camino = invertido;
    }

    delete[] distancia;
    delete[] visitado;
    delete[] anterior;

    return resultado;
}

void Dijkstra::liberarRuta(ResultadoRuta& resultado) {
    NodoRuta* actual = resultado.camino;
    while (actual != nullptr) {
        NodoRuta* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
    resultado.camino = nullptr;
}

void Dijkstra::imprimirRuta(const ResultadoRuta& resultado) {
    if (!resultado.encontrada) {
        std::cout << "No existe una ruta disponible entre esos dos puntos.\n";
        return;
    }
    std::cout << "\n--- RUTA MAS CORTA (" << resultado.distanciaTotal << " m) ---\n";
    NodoRuta* actual = resultado.camino;
    bool primero = true;
    while (actual != nullptr) {
        if (!primero) std::cout << " -> ";
        std::cout << actual->nodo->getNombre();
        primero = false;
        actual = actual->siguiente;
    }
    std::cout << "\n";
}
