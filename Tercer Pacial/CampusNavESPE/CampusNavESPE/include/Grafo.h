#ifndef GRAFO_H
#define GRAFO_H

#include "Nodo.h"
#include "Arista.h"
#include "ListaAdyacencia.h"
#include <string>

// Grafo no dirigido y ponderado que modela el campus.
// Es el UNICO dueño real de la memoria dinamica de Nodo* y Arista*.
// Mantiene ademas dos representaciones equivalentes, por fines didacticos:
//   - Lista de adyacencia (dinamica, con listas enlazadas)
//   - Matriz de adyacencia (dinamica, con distancias o "infinito")
class Grafo {
private:
    Nodo** nodos;               // arreglo dinamico de punteros a Nodo (dueño)
    ListaAdyacencia* listaAdy;  // arreglo dinamico: una ListaAdyacencia por nodo
    double** matriz;            // arreglo dinamico 2D de distancias
    Arista** aristas;           // arreglo dinamico: TODAS las aristas (dueño real)

    int capacidadNodos;
    int cantidadNodos;
    int capacidadAristas;
    int cantidadAristas;

    void redimensionarAristas();
    int indiceDeId(int id) const;
    void liberarTodo();

public:
    explicit Grafo(int capacidadInicial);
    ~Grafo();

    // Sin copia: el grafo posee memoria dinamica compleja; una copia superficial
    // provocaria doble liberacion. Si se necesitara copiar, se implementaria
    // una copia profunda explicita mas adelante.
    Grafo(const Grafo&) = delete;
    Grafo& operator=(const Grafo&) = delete;

    bool agregarNodo(Nodo* nodo);
    bool agregarArista(int idOrigen, int idDestino, double distancia,
                        const std::string& tipoCamino, bool accesible, bool abierta = true);

    Nodo* buscarNodoPorId(int id) const;
    Nodo* buscarNodoPorNombre(const std::string& nombre) const;

    int getCantidadNodos() const;
    Nodo* getNodoEnPosicion(int pos) const;
    const ListaAdyacencia& getListaAdyacenciaEnPosicion(int pos) const;
    double getDistanciaMatriz(int posA, int posB) const;

    void imprimirListaAdyacencia() const;
    void imprimirMatrizAdyacencia() const;
};

#endif // GRAFO_H
