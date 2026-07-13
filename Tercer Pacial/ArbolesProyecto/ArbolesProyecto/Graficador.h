#pragma once
#include "ComunWin.h"
#include <functional>
#include <string>

class VentanaGrafica {
public:
    // Muestra una ventana grafica y llama a callbackDibujo(hdc, areaCliente) en cada WM_PAINT.
    // La llamada bloquea hasta que el usuario cierre la ventana.
    static void mostrar(const std::string& titulo, std::function<void(HDC, RECT)> callbackDibujo);

private:
    static std::function<void(HDC, RECT)> dibujoActual;
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
