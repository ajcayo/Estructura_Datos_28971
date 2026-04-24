#include <iostream>
#include "Lista.h"
using namespace std;

Lista::Lista() {
    cabeza = nullptr;
    cola = nullptr;
}

Lista::~Lista() {
    Nodo* aux = cabeza;
    while (aux != nullptr) {
        Nodo* temp = aux;
        aux = aux->getSiguiente();
        delete temp;
    }
}

void Lista::insertarCabeza(string cedula, string nombre) {
    Nodo* nuevo = new Nodo(cedula, nombre);

    if (cabeza == nullptr) {
        cabeza = nuevo;
        cola = nuevo;
    } else {
        nuevo->setSiguiente(cabeza);
        cabeza = nuevo;
    }
}

void Lista::imprimir() {
    Nodo* aux = cabeza;
    while (aux != nullptr) {
        cout << aux->getCedula() << " - " << aux->getNombre() << endl;
        aux = aux->getSiguiente();
    }
}