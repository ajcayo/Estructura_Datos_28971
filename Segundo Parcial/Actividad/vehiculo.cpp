#include "Vehiculo.h"

Vehiculo::Vehiculo(std::string pla) {
    placa = pla;
}

std::string Vehiculo::getPlaca() const {
    return placa;
}

void Vehiculo::setPlaca(std::string& pla) {
    placa = pla;
}