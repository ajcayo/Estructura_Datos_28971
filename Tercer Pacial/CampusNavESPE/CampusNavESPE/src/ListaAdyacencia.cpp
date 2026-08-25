#include "ListaAdyacencia.h"

ListaAdyacencia::ListaAdyacencia() : cabeza(nullptr), cola(nullptr), cantidad(0) {
}

ListaAdyacencia::~ListaAdyacencia() {
    NodoLista* actual = cabeza;
    while (actual != nullptr) {
        NodoLista* siguiente = actual->siguiente;
        delete actual;          // solo liberamos el nodo de la lista, no la Arista
        actual = siguiente;
    }
    cabeza = nullptr;
    cola = nullptr;
}

ListaAdyacencia::ListaAdyacencia(ListaAdyacencia&& otra) noexcept
    : cabeza(otra.cabeza), cola(otra.cola), cantidad(otra.cantidad) {
    otra.cabeza = nullptr;
    otra.cola = nullptr;
    otra.cantidad = 0;
}

ListaAdyacencia& ListaAdyacencia::operator=(ListaAdyacencia&& otra) noexcept {
    if (this != &otra) {
        // Liberar lo que esta lista ya tenia antes de robar los datos de "otra"
        NodoLista* actual = cabeza;
        while (actual != nullptr) {
            NodoLista* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
        cabeza = otra.cabeza;
        cola = otra.cola;
        cantidad = otra.cantidad;
        otra.cabeza = nullptr;
        otra.cola = nullptr;
        otra.cantidad = 0;
    }
    return *this;
}

void ListaAdyacencia::insertar(Arista* arista) {
    NodoLista* nuevo = new NodoLista(arista);
    if (cabeza == nullptr) {
        cabeza = nuevo;
        cola = nuevo;
    } else {
        cola->siguiente = nuevo;
        cola = nuevo;
    }
    cantidad++;
}

ListaAdyacencia::NodoLista* ListaAdyacencia::getCabeza() const {
    return cabeza;
}

int ListaAdyacencia::getCantidad() const {
    return cantidad;
}
