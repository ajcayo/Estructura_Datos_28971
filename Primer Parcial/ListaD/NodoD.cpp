#include "NodoD.h"

Nodo::Nodo(string c, string n) {
    cedula = c;
    nombre = n;
    siguiente = nullptr;
    anterior = nullptr;
}

void Nodo::setCedula(string c) { 
cedula = c; 
}
void Nodo::setNombre(string n) { 
nombre = n; 
}
void Nodo::setSiguiente(Nodo* s) { 
siguiente = s; 
}
void Nodo::setAnterior(Nodo* a) { 
anterior = a; 
}

string Nodo::getCedula() { 
return cedula; 
}
string Nodo::getNombre() { 
return nombre; 
}
Nodo* Nodo::getSiguiente() {
return siguiente; 
}
Nodo* Nodo::getAnterior() { 
return anterior; 
}