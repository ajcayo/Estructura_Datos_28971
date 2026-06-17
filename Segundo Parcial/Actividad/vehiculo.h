#pragma once
#include <string>

class Vehiculo {
private:
    std::string placa;
public:
    Vehiculo(std::string pla);
    std::string getPlaca() const;
    void setPlaca(std::string& pla);
};