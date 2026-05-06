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
        cabeza = cola = nuevo;
    } else {
        nuevo->setSiguiente(cabeza);
        cabeza->setAnterior(nuevo);
        cabeza = nuevo;
    }
}

void Lista::insertarCola(string cedula, string nombre) {
    Nodo* nuevo = new Nodo(cedula, nombre);

    if (cola == nullptr) {
        cabeza = cola = nuevo;
    } else {
        cola->setSiguiente(nuevo);
        nuevo->setAnterior(cola);
        cola = nuevo;
    }
}

void Lista::imprimir() {
    Nodo* aux = cabeza;
    while (aux != nullptr) {
        cout << aux->getCedula() << " - " << aux->getNombre() << endl;
        aux = aux->getSiguiente();
    }
}

void Lista::imprimirReversa() {
    Nodo* aux = cola;
    while (aux != nullptr) {
        cout << aux->getCedula() << " - " << aux->getNombre() << endl;
        aux = aux->getAnterior();
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

    while (aux != nullptr) {
        if (aux->getCedula() == cedula) {

            if (aux == cabeza) {
                cabeza = aux->getSiguiente();
                if (cabeza != nullptr)
                    cabeza->setAnterior(nullptr);
                else
                    cola = nullptr;
            }
            else if (aux == cola) {
                cola = aux->getAnterior();
                if (cola != nullptr)
                    cola->setSiguiente(nullptr);
                else
                    cabeza = nullptr;
            }
            else {
                aux->getAnterior()->setSiguiente(aux->getSiguiente());
                aux->getSiguiente()->setAnterior(aux->getAnterior());
            }

            delete aux;
            cout << "Elemento eliminado\n";
            return;
        }

        aux = aux->getSiguiente();
    }

    cout << "No se encontro la cedula\n";
}