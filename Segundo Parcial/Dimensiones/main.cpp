#include <iostream>
#include <windows.h>

using namespace std;

typedef int (*Funcion)();

int main()
{
    HINSTANCE dll;

    dll = LoadLibrary("PantallaDLL.dll");

    if(dll == NULL)
    {
        cout << "No se pudo cargar la DLL";
        return 0;
    }

    Funcion ancho;
    Funcion alto;

    ancho = (Funcion)GetProcAddress(dll, "obtenerAncho");
    alto = (Funcion)GetProcAddress(dll, "obtenerAlto");

    cout << "Ancho: " << ancho() << endl;
    cout << "Alto : " << alto() << endl;

    FreeLibrary(dll);

    return 0;
}