#include "dll.h"
#include <windows.h>
#include <iostream>

using namespace std;

DllClass::DllClass()
{
    ancho = 0;
    alto = 0;
}

DllClass::~DllClass()
{

}

void DllClass::capturarPantalla()
{
    int *pAncho;
    int *pAlto;

    pAncho = &ancho;
    pAlto = &alto;

    *pAncho = GetSystemMetrics(SM_CXSCREEN);
    *pAlto = GetSystemMetrics(SM_CYSCREEN);
}

int DllClass::obtenerAncho()
{
    return ancho;
}

int DllClass::obtenerAlto()
{
    return alto;
}

void DllClass::mostrarDatos()
{
    cout << "Ancho: " << ancho << endl;
    cout << "Alto : " << alto << endl;
}

BOOL WINAPI DllMain
(
    HINSTANCE hinstDLL,
    DWORD fdwReason,
    LPVOID lpvReserved
)
{
    switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            break;
        }

        case DLL_PROCESS_DETACH:
        {
            break;
        }

        case DLL_THREAD_ATTACH:
        {
            break;
        }

        case DLL_THREAD_DETACH:
        {
            break;
        }
    }

    return TRUE;
}