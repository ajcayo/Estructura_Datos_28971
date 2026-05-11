#include "Nodo.h"

Nodo::Nodo(string c, string n) {
    cedula = c;
    nombre = n;
    siguiente = nullptr;
}

void Nodo::setCedula(string c) { cedula = c; }
void Nodo::setNombre(string n) { nombre = n; }
void Nodo::setSiguiente(Nodo* s) { siguiente = s; }

string Nodo::getCedula() { return cedula; }
string Nodo::getNombre() { return nombre; }
Nodo* Nodo::getSiguiente() { return siguiente; }