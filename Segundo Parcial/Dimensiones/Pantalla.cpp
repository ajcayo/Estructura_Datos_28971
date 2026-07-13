#include "Pantalla.h"
#include <windows.h>

Pantalla::Pantalla()
{
    ancho = 0;
    alto = 0;
}

void Pantalla::obtenerDatos()
{
    ancho = GetSystemMetrics(SM_CXSCREEN);
    alto = GetSystemMetrics(SM_CYSCREEN);
}

int Pantalla::getAncho()
{
    return ancho;
}

int Pantalla::getAlto()
{
    return alto;
}