#pragma once
#include "Vehiculo.h"

class NodoVehiculo {
private:
    Vehiculo* vehiculo;
    NodoVehiculo* siguiente;
public:
    NodoVehiculo(Vehiculo* v);
    ~NodoVehiculo();
    Vehiculo* getVehiculo();
    NodoVehiculo* getSiguiente();
    void setSiguiente(NodoVehiculo* sig);
};

class ListaVehiculos {
private:
    NodoVehiculo* cabeza;
public:
    ListaVehiculos();
    ~ListaVehiculos();
    void crear(Vehiculo* v);
    void reportar() const;
    Vehiculo* buscar(std::string placa) const;
    bool actualizar(std::string placa, std::string nuevaPlaca);
    bool eliminar(std::string placa);


    NodoVehiculo* getCabeza() const;
};