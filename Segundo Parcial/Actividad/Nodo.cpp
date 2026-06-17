#include "Nodo.h"

Nodo::Nodo(Reserva* r) {
    reserva = r;
    siguiente = nullptr;
    anterior = nullptr;
}
Nodo::~Nodo() {
    delete reserva; 
}

Reserva* Nodo::getReserva() {
     return reserva; 
    }
Nodo* Nodo::getSiguiente() {
     return siguiente; 
    }
Nodo* Nodo::getAnterior() {
     return anterior; 
    }

void Nodo::setSiguiente(Nodo* sig) {
     siguiente = sig; }
void Nodo::setAnterior(Nodo* ant) {
     anterior = ant; }