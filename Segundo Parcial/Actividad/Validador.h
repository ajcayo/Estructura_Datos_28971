#pragma once
#include <string>

class Validador {
public:
    static std::string aMayusculas(std::string texto);

    static std::string trim(std::string texto);

    static bool validarTexto(std::string texto);

    static std::string leerTextoValido(std::string mensaje);

    static bool validarCedula(std::string cedula);

    static bool validarPlaca(std::string placa);

    static int leerEntero(std::string mensaje, int min, int max);

    static std::string cifrarPlacaCesar(std::string placa, int desplazamiento);
};
