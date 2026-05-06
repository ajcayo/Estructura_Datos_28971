#ifndef VEHICULO_H
#define VEHICULO_H

#include "Persona.h"

class Vehiculo {
private:
    string placa;
    string modelo;
    bool reservado;
    Persona persona;

public:
    Vehiculo* siguiente; // puntero para lista

    Vehiculo(string p, string m);

    string getPlaca();
    string getModelo();
    bool getReservado();

    void reservar(Persona per);
    void liberar();

    Persona getPersona();
};

#endif