#pragma once
#include "Nodo.h"

class ListaDoble {
private:
    Nodo* cabeza;
    Nodo* cola;
public:
    ListaDoble();
    ~ListaDoble();
    void agregarReserva(Reserva* r);
    void mostrarReservas() const;
    Reserva* buscarPorPlaca(std::string placa) const;
    bool eliminarReserva(std::string placa);
    void eliminarCascadaUsuario(std::string cedula);
    void eliminarCascadaVehiculo(std::string placa);


    Nodo* getCabeza() const;
};
