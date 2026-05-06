#include "Persona.h"

Persona::Persona() {
    cedula = "";
    nombre = "";
}

Persona::Persona(string c, string n) {
    cedula = c;
    nombre = n;
}

void Persona::setCedula(string c) {
    cedula = c;
}

void Persona::setNombre(string n) {
    nombre = n;
}

string Persona::getCedula() {
    return cedula;
}

string Persona::getNombre() {
    return nombre;
}