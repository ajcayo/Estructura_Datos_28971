#ifndef LISTA_ADYACENCIA_H
#define LISTA_ADYACENCIA_H

#include "Arista.h"

// Lista enlazada dinamica de aristas incidentes a UN nodo del grafo.
// No es dueña de los Arista* que guarda (esos los posee Grafo);
// solo posee los nodos de la propia lista enlazada (NodoLista).
class ListaAdyacencia {
public:
    struct NodoLista {
        Arista* arista;
        NodoLista* siguiente;
        NodoLista(Arista* a) : arista(a), siguiente(nullptr) {}
    };

private:
    NodoLista* cabeza;
    NodoLista* cola;
    int cantidad;

public:
    ListaAdyacencia();
    ~ListaAdyacencia();

    // Sin copia: evita que dos listas liberen los mismos NodoLista.
    ListaAdyacencia(const ListaAdyacencia&) = delete;
    ListaAdyacencia& operator=(const ListaAdyacencia&) = delete;

    // Movible: necesario porque Grafo guarda un arreglo dinamico de ListaAdyacencia.
    ListaAdyacencia(ListaAdyacencia&& otra) noexcept;
    ListaAdyacencia& operator=(ListaAdyacencia&& otra) noexcept;

    void insertar(Arista* arista);
    NodoLista* getCabeza() const;
    int getCantidad() const;
};

#endif // LISTA_ADYACENCIA_H
