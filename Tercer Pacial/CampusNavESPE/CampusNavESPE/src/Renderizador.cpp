#include "Renderizador.h"
#include <iostream>
#include <limits>

namespace {
    const int ANCHO = 60;
    const int ALTO = 20;

    char simboloTipo(TipoNodo tipo) {
        switch (tipo) {
            case TipoNodo::Garita:      return 'G';
            case TipoNodo::Entrada:     return 'E';
            case TipoNodo::Parqueadero: return 'P';
            case TipoNodo::Biblioteca:  return 'B';
            case TipoNodo::Cafeteria:   return 'C';
            case TipoNodo::Laboratorio: return 'L';
            case TipoNodo::Auditorio:   return 'U';
            case TipoNodo::Rampa:       return 'R';
            case TipoNodo::Escalera:    return 'S';
            case TipoNodo::Cruce:       return 'X';
            case TipoNodo::Edificio:    return 'H';
        }
        return '?';
    }

    // Construye una rejilla dinamica (char**) y ubica cada nodo segun
    // su latitud/longitud, escaladas al tamaño de la rejilla en consola.
    char** construirRejilla(const Grafo& grafo, int* filaPorNodo, int* colPorNodo) {
        char** rejilla = new char*[ALTO];
        for (int i = 0; i < ALTO; ++i) {
            rejilla[i] = new char[ANCHO];
            for (int j = 0; j < ANCHO; ++j) rejilla[i][j] = '.';
        }

        int n = grafo.getCantidadNodos();
        if (n == 0) return rejilla;

        double latMin = 1e9, latMax = -1e9, lonMin = 1e9, lonMax = -1e9;
        for (int i = 0; i < n; ++i) {
            Nodo* nd = grafo.getNodoEnPosicion(i);
            if (nd->getLatitud() < latMin) latMin = nd->getLatitud();
            if (nd->getLatitud() > latMax) latMax = nd->getLatitud();
            if (nd->getLongitud() < lonMin) lonMin = nd->getLongitud();
            if (nd->getLongitud() > lonMax) lonMax = nd->getLongitud();
        }
        double rangoLat = (latMax - latMin) == 0 ? 1.0 : (latMax - latMin);
        double rangoLon = (lonMax - lonMin) == 0 ? 1.0 : (lonMax - lonMin);

        for (int i = 0; i < n; ++i) {
            Nodo* nd = grafo.getNodoEnPosicion(i);
            int col = static_cast<int>(((nd->getLongitud() - lonMin) / rangoLon) * (ANCHO - 1));
            int fila = static_cast<int>((1.0 - (nd->getLatitud() - latMin) / rangoLat) * (ALTO - 1));
            if (col < 0) { col = 0; }
            if (col >= ANCHO) { col = ANCHO - 1; }
            if (fila < 0) { fila = 0; }
            if (fila >= ALTO) { fila = ALTO - 1; }

            filaPorNodo[i] = fila;
            colPorNodo[i] = col;
            rejilla[fila][col] = simboloTipo(nd->getTipo());
        }
        return rejilla;
    }

    void liberarRejilla(char** rejilla) {
        for (int i = 0; i < ALTO; ++i) delete[] rejilla[i];
        delete[] rejilla;
    }
}

void Renderizador::dibujarMapa(const Grafo& grafo) {
    int n = grafo.getCantidadNodos();
    if (n == 0) {
        std::cout << "El grafo no tiene nodos cargados todavia.\n";
        return;
    }

    int* filas = new int[n];
    int* cols = new int[n];
    char** rejilla = construirRejilla(grafo, filas, cols);

    std::cout << "\n=== MAPA ESPE - SANGOLQUI (vista aproximada) ===\n";
    for (int i = 0; i < ALTO; ++i) {
        for (int j = 0; j < ANCHO; ++j) std::cout << rejilla[i][j];
        std::cout << "\n";
    }
    std::cout << "\nLeyenda: G=Garita E=Entrada P=Parqueadero B=Biblioteca "
                 "C=Cafeteria L=Laboratorio U=Auditorio R=Rampa S=Escalera "
                 "X=Cruce H=Edificio\n";

    liberarRejilla(rejilla);
    delete[] filas;
    delete[] cols;
}

void Renderizador::dibujarRuta(const Grafo& grafo, const ResultadoRuta& ruta) {
    if (!ruta.encontrada) {
        std::cout << "No hay ruta que dibujar.\n";
        return;
    }

    int n = grafo.getCantidadNodos();
    int* filas = new int[n];
    int* cols = new int[n];
    char** rejilla = construirRejilla(grafo, filas, cols);

    // Marcar el camino en la rejilla con '*'
    NodoRuta* actual = ruta.camino;
    while (actual != nullptr) {
        for (int i = 0; i < n; ++i) {
            if (grafo.getNodoEnPosicion(i)->getId() == actual->nodo->getId()) {
                rejilla[filas[i]][cols[i]] = '*';
                break;
            }
        }
        actual = actual->siguiente;
    }

    std::cout << "\n=== RUTA SOBRE EL MAPA ===\n";
    for (int i = 0; i < ALTO; ++i) {
        for (int j = 0; j < ANCHO; ++j) std::cout << rejilla[i][j];
        std::cout << "\n";
    }
    std::cout << "(* marca los puntos de la ruta calculada)\n";

    liberarRejilla(rejilla);
    delete[] filas;
    delete[] cols;
}
