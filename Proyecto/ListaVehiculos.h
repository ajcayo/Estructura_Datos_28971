#ifndef LISTA_H
#define LISTA_H

#include "Vehiculo.h"

class ListaVehiculos {
private:
    Vehiculo* cabeza;

public:
    ListaVehiculos();

    void insertar(string placa, string modelo);
    void eliminar(string placa);
    Vehiculo* buscar(string placa);
    void imprimir();
    void reservarVehiculo(string placa, Persona p);
};

#endif