#include "ArbolPantalla.h"
#include <stdio.h>

// ============================================
// PANTALLA
// Captura el ancho y alto real de la pantalla
// donde se esta ejecutando el programa
// ============================================
Pantalla::Pantalla()
{
    anchoTotal = GetSystemMetrics(SM_CXSCREEN);
    altoTotal  = GetSystemMetrics(SM_CYSCREEN);
}

int Pantalla::getAncho()
{
    return anchoTotal;
}

int Pantalla::getAlto()
{
    return altoTotal;
}

// ============================================
// ARBOL BINARIO
// ============================================
ArbolBinario::ArbolBinario()
{
    raiz = NULL;
}

// El circulo se hace mas pequeno segun el nivel
int ArbolBinario::calcularRadio(int nivel)
{
    int radioBase = 30;
    if (nivel == 0)
    {
        return radioBase;
    }
    int resultado = calcularRadio(nivel - 1) / 2;
    if (resultado < 8) resultado = 8;
    return resultado;
}

// Inserta en el lugar correcto segun si es menor o mayor
Nodo* ArbolBinario::insertarRecursivo(Nodo* nodo, int valor, int nivel)
{
    if (nodo == NULL)
    {
        return new Nodo(valor, calcularRadio(nivel));
    }
    if (valor < nodo->valor)
    {
        nodo->izquierdo = insertarRecursivo(nodo->izquierdo, valor, nivel + 1);
    }
    else if (valor > nodo->valor)
    {
        nodo->derecho = insertarRecursivo(nodo->derecho, valor, nivel + 1);
    }
    return nodo;
}

void ArbolBinario::insertar(int valor)
{
    raiz = insertarRecursivo(raiz, valor, 0);
}

// Dibuja un circulo con el numero adentro
void ArbolBinario::dibujarNodo(HDC hdc, int x, int y, int radio, int valor, COLORREF color)
{
    HBRUSH pincel = CreateSolidBrush(color);
    HPEN   lapiz  = CreatePen(PS_SOLID, 2, RGB(220, 220, 220));
    SelectObject(hdc, pincel);
    SelectObject(hdc, lapiz);

    Ellipse(hdc, x - radio, y - radio, x + radio, y + radio);

    char texto[15];
    sprintf(texto, "%d", valor);

    SIZE tamTexto;
    GetTextExtentPoint32A(hdc, texto, lstrlenA(texto), &tamTexto);
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    TextOutA(hdc, x - tamTexto.cx / 2, y - tamTexto.cy / 2, texto, lstrlenA(texto));

    DeleteObject(pincel);
    DeleteObject(lapiz);
}

// Recorre el arbol y dibuja lineas y circulos
void ArbolBinario::dibujarRecursivo(Nodo* nodo, HDC hdc, int x, int y, int separacion, COLORREF color)
{
    if (nodo == NULL) return;

    int nuevaSep = separacion / 2;
    int bajar    = 80;
    int xIzq     = x - separacion;
    int xDer     = x + separacion;
    int yHijo    = y + bajar;

    if (nodo->izquierdo != NULL)
    {
        HPEN lapiz = CreatePen(PS_SOLID, 2, RGB(160, 160, 160));
        SelectObject(hdc, lapiz);
        MoveToEx(hdc, x, y, NULL);
        LineTo(hdc, xIzq, yHijo);
        DeleteObject(lapiz);
    }

    if (nodo->derecho != NULL)
    {
        HPEN lapiz = CreatePen(PS_SOLID, 2, RGB(160, 160, 160));
        SelectObject(hdc, lapiz);
        MoveToEx(hdc, x, y, NULL);
        LineTo(hdc, xDer, yHijo);
        DeleteObject(lapiz);
    }

    dibujarRecursivo(nodo->izquierdo, hdc, xIzq, yHijo, nuevaSep, color);
    dibujarRecursivo(nodo->derecho,   hdc, xDer, yHijo, nuevaSep, color);
    dibujarNodo(hdc, x, y, nodo->radio, nodo->valor, color);
}

void ArbolBinario::dibujar(HDC hdc, int x, int y, int separacion, COLORREF color)
{
    dibujarRecursivo(raiz, hdc, x, y, separacion, color);
}

// ============================================
// FUNCIONES EXPORTADAS DEL DLL
// ============================================
int ObtenerAnchoPantalla()
{
    Pantalla p;
    return p.getAncho();
}

int ObtenerAltoPantalla()
{
    Pantalla p;
    return p.getAlto();
}

// ============================================
// FUNCION PRINCIPAL
//
// 1. Captura el ancho y alto de la pantalla
// 2. Divide el ancho en partes
// 3. Esos valores entran al arbol
// 4. Se dibuja UN SOLO arbol centrado
// ============================================
void DibujarArbolEnVentana(HWND hwnd)
{
    HDC hdc = GetDC(hwnd);

    // Leer la pantalla actual
    Pantalla pantalla;
    int ancho = pantalla.getAncho();
    int alto  = pantalla.getAlto();

    // Fondo negro
    RECT rect;
    rect.left = 0; rect.top = 0;
    rect.right = ancho; rect.bottom = alto;
    HBRUSH fondo = CreateSolidBrush(RGB(0, 0, 0));
    FillRect(hdc, &rect, fondo);
    DeleteObject(fondo);

    // Valores que vienen de dividir el ancho de esta pantalla
    // En cada computador seran distintos
    int v1 = ancho / 2;
    int v2 = ancho / 3;
    int v3 = ancho / 4;
    int v4 = ancho / 5;
    int v5 = ancho / 6;
    int v6 = ancho / 7;
    int v7 = ancho / 8;

    // Llenar el arbol con esos valores
    ArbolBinario arbol;
    arbol.insertar(v1);
    arbol.insertar(v2);
    arbol.insertar(v3);
    arbol.insertar(v4);
    arbol.insertar(v5);
    arbol.insertar(v6);
    arbol.insertar(v7);

    // Texto informativo
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 220, 80));
    char info[150];
    sprintf(info,
            "Pantalla capturada: %d x %d px  |  Nodos: %d, %d, %d, %d, %d, %d, %d",
            ancho, alto, v1, v2, v3, v4, v5, v6, v7);
    TextOutA(hdc, 10, 10, info, lstrlenA(info));

    // Dibujar UN SOLO arbol centrado en toda la pantalla
    int centroX    = ancho / 2;
    int inicioY    = alto  / 5;
    int separacion = ancho / 4;

    arbol.dibujar(hdc, centroX, inicioY, separacion, RGB(60, 120, 220));

    ReleaseDC(hwnd, hdc);
}
