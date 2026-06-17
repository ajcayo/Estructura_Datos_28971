#pragma once
#include <ctime>
#include <string>

class GestorFechas {
public:

    static std::string ajustarFechaLaborable(std::tm* fecha);

  
    static std::string validarRangoFecha(int dia, int mes, int anio);
};
