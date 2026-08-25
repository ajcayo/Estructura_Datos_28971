#include "Grafo.h"
#include <iostream>
#include <iomanip>
#include <limits>

namespace {
    const double INF = std::numeric_limits<double>::infinity();
}

Grafo::Grafo(int capacidadInicial)
    : nodos(nullptr), listaAdy(nullptr), matriz(nullptr), aristas(nullptr),
      capacidadNodos(capacidadInicial), cantidadNodos(0),
      capacidadAristas(capacidadInicial * 2 > 0 ? capacidadInicial * 2 : 4),
      cantidadAristas(0) {

    // Arreglo dinamico de punteros a Nodo (aun sin nodos dentro).
    nodos = new Nodo*[capacidadNodos];
    for (int i = 0; i < capacidadNodos; ++i) nodos[i] = nullptr;

    // Un objeto ListaAdyacencia por cada posible nodo.
    listaAdy = new ListaAdyacencia[capacidadNodos];

    // Matriz de adyacencia cuadrada, inicializada en "infinito" (sin conexion).
    matriz = new double*[capacidadNodos];
    for (int i = 0; i < capacidadNodos; ++i) {
        matriz[i] = new double[capacidadNodos];
        for (int j = 0; j < capacidadNodos; ++j) {
            matriz[i][j] = (i == j) ? 0.0 : INF;
        }
    }

    // Arreglo dinamico de todas las aristas del grafo (dueño real de la memoria).
    aristas = new Arista*[capacidadAristas];
    for (int i = 0; i < capacidadAristas; ++i) aristas[i] = nullptr;
}

void Grafo::liberarTodo() {
    for (int i = 0; i < cantidadAristas; ++i) delete aristas[i];
    delete[] aristas;

    for (int i = 0; i < cantidadNodos; ++i) delete nodos[i];
    delete[] nodos;

    delete[] listaAdy; // llama al destructor de cada ListaAdyacencia

    for (int i = 0; i < capacidadNodos; ++i) delete[] matriz[i];
    delete[] matriz;
}

Grafo::~Grafo() {
    liberarTodo();
}

void Grafo::redimensionarAristas() {
    int nuevaCapacidad = capacidadAristas * 2;
    Arista** nuevoArreglo = new Arista*[nuevaCapacidad];
    for (int i = 0; i < cantidadAristas; ++i) nuevoArreglo[i] = aristas[i];
    for (int i = cantidadAristas; i < nuevaCapacidad; ++i) nuevoArreglo[i] = nullptr;
    delete[] aristas;
    aristas = nuevoArreglo;
    capacidadAristas = nuevaCapacidad;
}

int Grafo::indiceDeId(int id) const {
    for (int i = 0; i < cantidadNodos; ++i) {
        if (nodos[i]->getId() == id) return i;
    }
    return -1;
}

bool Grafo::agregarNodo(Nodo* nodo) {
    if (cantidadNodos >= capacidadNodos) return false; // capacidad fijada al crear el grafo
    nodos[cantidadNodos] = nodo;
    cantidadNodos++;
    return true;
}

bool Grafo::agregarArista(int idOrigen, int idDestino, double distancia,
                           const std::string& tipoCamino, bool accesible, bool abierta) {
    int posO = indiceDeId(idOrigen);
    int posD = indiceDeId(idDestino);
    if (posO == -1 || posD == -1) return false;

    if (cantidadAristas >= capacidadAristas) redimensionarAristas();

    Arista* nueva = new Arista(nodos[posO], nodos[posD], distancia, tipoCamino, accesible, abierta);
    aristas[cantidadAristas] = nueva;
    cantidadAristas++;

    // Una sola Arista representa la conexion; se referencia desde ambas listas
    // de adyacencia porque el grafo es no dirigido.
    listaAdy[posO].insertar(nueva);
    listaAdy[posD].insertar(nueva);

    matriz[posO][posD] = distancia;
    matriz[posD][posO] = distancia;

    return true;
}

Nodo* Grafo::buscarNodoPorId(int id) const {
    int pos = indiceDeId(id);
    return (pos == -1) ? nullptr : nodos[pos];
}

Nodo* Grafo::buscarNodoPorNombre(const std::string& nombre) const {
    for (int i = 0; i < cantidadNodos; ++i) {
        if (nodos[i]->getNombre() == nombre) return nodos[i];
    }
    return nullptr;
}

int Grafo::getCantidadNodos() const { return cantidadNodos; }

Nodo* Grafo::getNodoEnPosicion(int pos) const {
    if (pos < 0 || pos >= cantidadNodos) return nullptr;
    return nodos[pos];
}

const ListaAdyacencia& Grafo::getListaAdyacenciaEnPosicion(int pos) const {
    return listaAdy[pos];
}

double Grafo::getDistanciaMatriz(int posA, int posB) const {
    if (posA < 0 || posA >= cantidadNodos || posB < 0 || posB >= cantidadNodos) return INF;
    return matriz[posA][posB];
}

void Grafo::imprimirListaAdyacencia() const {
    std::cout << "\n--- LISTA DE ADYACENCIA ---\n";
    for (int i = 0; i < cantidadNodos; ++i) {
        std::cout << nodos[i]->getNombre() << " -> ";
        ListaAdyacencia::NodoLista* actual = listaAdy[i].getCabeza();
        bool primero = true;
        while (actual != nullptr) {
            Nodo* vecino = actual->arista->obtenerVecino(nodos[i]->getId());
            if (!primero) std::cout << ", ";
            std::cout << vecino->getNombre() << " (" << actual->arista->getDistancia() << "m)";
            primero = false;
            actual = actual->siguiente;
        }
        if (primero) std::cout << "(sin conexiones)";
        std::cout << "\n";
    }
}

void Grafo::imprimirMatrizAdyacencia() const {
    std::cout << "\n--- MATRIZ DE ADYACENCIA (metros) ---\n";
    std::cout << std::setw(14) << " ";
    for (int j = 0; j < cantidadNodos; ++j) {
        std::cout << std::setw(6) << nodos[j]->getId();
    }
    std::cout << "\n";
    for (int i = 0; i < cantidadNodos; ++i) {
        std::cout << std::setw(14) << nodos[i]->getNombre().substr(0, 13);
        for (int j = 0; j < cantidadNodos; ++j) {
            if (matriz[i][j] == INF) std::cout << std::setw(6) << "-";
            else std::cout << std::setw(6) << matriz[i][j];
        }
        std::cout << "\n";
    }
}
