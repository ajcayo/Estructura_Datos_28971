#include "Reserva.h"

Reserva::Reserva(Usuario* u, Vehiculo* v, std::tm f, int hora) {
    usuario = u;
    vehiculo = v;
    fechaAsignada = f;
    horaAsignada = hora;
}
Reserva::~Reserva() {
}

Usuario* Reserva::getUsuario() const {
     return usuario; 
    }
Vehiculo* Reserva::getVehiculo() const {
     return vehiculo; 
    }
std::tm Reserva::getFechaAsignada() const {
     return fechaAsignada; 
    }
void Reserva::setFechaAsignada(std::tm f) {
     fechaAsignada = f; 
    }
int Reserva::getHoraAsignada() const {
     return horaAsignada; 
    }
void Reserva::setHoraAsignada(int h) {
     horaAsignada = h; 
    }