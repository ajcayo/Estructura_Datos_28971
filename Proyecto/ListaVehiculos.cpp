#include <iostream>
#include "ListaVehiculos.h"

using namespace std;

ListaVehiculos::ListaVehiculos() {
    cabeza = NULL;
}

void ListaVehiculos::insertar(string placa, string modelo) {
    Vehiculo* nuevo = new Vehiculo(placa, modelo);

    if (cabeza == NULL) {
        cabeza = nuevo;
    } else {
        Vehiculo* aux = cabeza;
        while (aux->siguiente != NULL) {
            aux = aux->siguiente;
        }
        aux->siguiente = nuevo;
    }
}

Vehiculo* ListaVehiculos::buscar(string placa) {
    Vehiculo* aux = cabeza;

    while (aux != NULL) {
        if (aux->getPlaca() == placa) {
            return aux;
        }
        aux = aux->siguiente;
    }
    return NULL;
}

void ListaVehiculos::eliminar(string placa) {
    Vehiculo* aux = cabeza;
    Vehiculo* anterior = NULL;

    while (aux != NULL) {
        if (aux->getPlaca() == placa) {
            if (anterior == NULL) {
                cabeza = aux->siguiente;
            } else {
                anterior->siguiente = aux->siguiente;
            }
            delete aux;
            cout << "Vehiculo eliminado\n";
            return;
        }
        anterior = aux;
        aux = aux->siguiente;
    }

    cout << "No encontrado\n";
}

void ListaVehiculos::imprimir() {
    Vehiculo* aux = cabeza;

    while (aux != NULL) {
        cout << "Placa: " << aux->getPlaca() << endl;
        cout << "Modelo: " << aux->getModelo() << endl;

        if (aux->getReservado()) {
            cout << "Reservado por: " << aux->getPersona().getNombre() << endl;
        } else {
            cout << "Disponible\n";
        }

        cout << "------------------\n";

        aux = aux->siguiente;
    }
}

void ListaVehiculos::reservarVehiculo(string placa, Persona p) {
    Vehiculo* v = buscar(placa);

    if (v != NULL) {
        if (!v->getReservado()) {
            v->reservar(p);
            cout << "Vehiculo reservado\n";
        } else {
            cout << "Ya esta reservado\n";
        }
    } else {
        cout << "Vehiculo no encontrado\n";
    }
}