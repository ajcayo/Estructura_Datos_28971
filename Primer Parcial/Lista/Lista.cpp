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


Nodo* Lista::buscar(string cedula) {
    Nodo* aux = cabeza;

    while (aux != nullptr) {
        if (aux->getCedula() == cedula) {
            return aux;
        }
        aux = aux->getSiguiente();
    }
    return nullptr;
}

void Lista::eliminar(string cedula) {
    Nodo* aux = cabeza;
    Nodo* anterior = nullptr;

    while (aux != nullptr) {
        if (aux->getCedula() == cedula) {

            if (anterior == nullptr) {
                cabeza = aux->getSiguiente();

                if (cabeza == nullptr) {
                    cola = nullptr;
                }
            } else {
                anterior->setSiguiente(aux->getSiguiente());

                if (aux == cola) {
                    cola = anterior;
                }
            }

            delete aux;
            cout << "Elemento eliminado\n";
            return;
        }

        anterior = aux;
        aux = aux->getSiguiente();
    }

    cout << "No se encontro la cedula\n";
}