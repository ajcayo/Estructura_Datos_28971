#include "Vehiculo.h"

Vehiculo::Vehiculo(string p, string m) {
    placa = p;
    modelo = m;
    reservado = false;
    siguiente = NULL;
}

string Vehiculo::getPlaca() {
    return placa;
}

string Vehiculo::getModelo() {
    return modelo;
}

bool Vehiculo::getReservado() {
    return reservado;
}

void Vehiculo::reservar(Persona per) {
    persona = per;
    reservado = true;
}

void Vehiculo::liberar() {
    reservado = false;
}

Persona Vehiculo::getPersona() {
    return persona;
}