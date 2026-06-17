#pragma once
#include <ctime>
#include "Usuario.h"
#include "Vehiculo.h"

class Reserva {
private:
    Usuario* usuario;
    Vehiculo* vehiculo;
    std::tm fechaAsignada;
    int horaAsignada; // hora en formato 24h (8-16)
public:
    Reserva(Usuario* u, Vehiculo* v, std::tm f, int hora = 8);
    ~Reserva();

    Usuario* getUsuario() const;
    Vehiculo* getVehiculo() const;
    std::tm getFechaAsignada() const;
    void setFechaAsignada(std::tm f);
    int getHoraAsignada() const;
    void setHoraAsignada(int h);
};