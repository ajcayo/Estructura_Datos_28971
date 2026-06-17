#pragma once
#include "Reserva.h"

class Nodo {
private:
    Reserva* reserva;
    Nodo* siguiente;
    Nodo* anterior;
public:
    Nodo(Reserva* r);
    ~Nodo();

    Reserva* getReserva();
    Nodo* getSiguiente();
    Nodo* getAnterior();
    
    void setSiguiente(Nodo* sig);
    void setAnterior(Nodo* ant);

    
};