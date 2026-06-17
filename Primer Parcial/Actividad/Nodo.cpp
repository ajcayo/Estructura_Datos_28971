#include "Nodo.h"
using namespace std;

Nodo::Nodo(string c, string n, string a) {
    cedula = c;
    nombre = n;
    apellido = a;
    siguiente = nullptr;
}

void Nodo::setCedula(string c) { cedula = c; }
void Nodo::setNombre(string n) { nombre = n; }
void Nodo::setApellido(string a) { apellido = a; }
void Nodo::setSiguiente(Nodo* s) { siguiente = s; }

string Nodo::getCedula() { return cedula; }
string Nodo::getNombre() { return nombre; }
string Nodo::getApellido() { return apellido; }
Nodo* Nodo::getSiguiente() { return siguiente; }