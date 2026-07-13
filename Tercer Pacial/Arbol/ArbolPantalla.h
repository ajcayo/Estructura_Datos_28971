#ifndef ARBOLPANTALLA_H
#define ARBOLPANTALLA_H

#include <windows.h>

class Nodo
{
public:
    int valor;
    int radio;
    Nodo* izquierdo;
    Nodo* derecho;

    Nodo(int val, int rad)
    {
        valor     = val;
        radio     = rad;
        izquierdo = NULL;
        derecho   = NULL;
    }
};

class ArbolBinario
{
private:
    Nodo* raiz;

    Nodo* insertarRecursivo(Nodo* nodo, int valor, int nivel);
    void  dibujarRecursivo(Nodo* nodo, HDC hdc, int x, int y, int separacion, COLORREF color);
    void  dibujarNodo(HDC hdc, int x, int y, int radio, int valor, COLORREF color);
    int   calcularRadio(int nivel);

public:
    ArbolBinario();
    void insertar(int valor);
    void dibujar(HDC hdc, int x, int y, int separacion, COLORREF color);
};

class Pantalla
{
private:
    int anchoTotal;
    int altoTotal;

public:
    Pantalla();
    int getAncho();
    int getAlto();
};

extern "C"
{
    __declspec(dllexport) int  ObtenerAnchoPantalla();
    __declspec(dllexport) int  ObtenerAltoPantalla();
    __declspec(dllexport) void DibujarArbolEnVentana(HWND hwnd);
}

#endif
