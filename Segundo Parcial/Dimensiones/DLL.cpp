#include <windows.h>
#include "Pantalla.h"

Pantalla pantalla;

extern "C" __declspec(dllexport) int obtenerAncho()
{
    pantalla.obtenerDatos();
    return pantalla.getAncho();
}

extern "C" __declspec(dllexport) int obtenerAlto()
{
    pantalla.obtenerDatos();
    return pantalla.getAlto();
}