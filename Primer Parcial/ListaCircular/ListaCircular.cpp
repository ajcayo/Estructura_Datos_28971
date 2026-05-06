#include <iostream>
#include "ListaCircular.h"
using namespace std;

ListaCircular::ListaCircular() {
    cabeza = nullptr;
}

ListaCircular::~ListaCircular() {
    if (cabeza == nullptr) return;

    Nodo* aux = cabeza->getSiguiente();
    while (aux != cabeza) {
        Nodo* temp = aux;
        aux = aux->getSiguiente();
        delete temp;
    }
    delete cabeza;
}

void ListaCircular::insertar(string cedula, string nombre) {
    Nodo* nuevo = new Nodo(cedula, nombre);

    if (cabeza == nullptr) {
        cabeza = nuevo;
        cabeza->setSiguiente(cabeza);
    } else {
        Nodo* aux = cabeza;
        while (aux->getSiguiente() != cabeza) {
            aux = aux->getSiguiente();
        }
        aux->setSiguiente(nuevo);
        nuevo->setSiguiente(cabeza);
    }
}

void ListaCircular::imprimir() {
    if (cabeza == nullptr) return;

    Nodo* aux = cabeza;
    do {
        cout << aux->getCedula() << " - " << aux->getNombre() << endl;
        aux = aux->getSiguiente();
    } while (aux != cabeza);
}

Nodo* ListaCircular::buscar(string cedula) {
    if (cabeza == nullptr) return nullptr;

    Nodo* aux = cabeza;
    do {
        if (aux->getCedula() == cedula)
            return aux;
        aux = aux->getSiguiente();
    } while (aux != cabeza);

    return nullptr;
}

void ListaCircular::eliminar(string cedula) {
    if (cabeza == nullptr) return;

    Nodo* actual = cabeza;
    Nodo* anterior = nullptr;

    do {
        if (actual->getCedula() == cedula) {


            if (actual == cabeza && actual->getSiguiente() == cabeza) {
                delete actual;
                cabeza = nullptr;
            }

            else if (actual == cabeza) {
                Nodo* ultimo = cabeza;
                while (ultimo->getSiguiente() != cabeza)
                    ultimo = ultimo->getSiguiente();

                cabeza = cabeza->getSiguiente();
                ultimo->setSiguiente(cabeza);
                delete actual;
            }

            else {
                anterior->setSiguiente(actual->getSiguiente());
                delete actual;
            }

            cout << "Elemento eliminado\n";
            return;
        }

        anterior = actual;
        actual = actual->getSiguiente();

    } while (actual != cabeza);

    cout << "No se encontro\n";
}