#include <windows.h>
#include <iostream>
#include <sstream>
#include <vector>
#include "ArbolExpresion.h"
using namespace std;

ArbolExpresion arbol;

LRESULT CALLBACK ProcedimientoVentana(HWND hwnd, UINT mensaje, WPARAM wParam, LPARAM lParam) {
    switch (mensaje) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT clientRect;
            GetClientRect(hwnd, &clientRect);

            arbol.dibujarArbol(hdc, clientRect.right);

            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, mensaje, wParam, lParam);
}

void mostrarVentanaGrafica(HINSTANCE hInstancia) {
    const char* nombreClase = "VentanaArbolExpresion";

    WNDCLASS wc = {};
    wc.lpfnWndProc = ProcedimientoVentana;
    wc.hInstance = hInstancia;
    wc.lpszClassName = nombreClase;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, nombreClase, "Arbol de Expresion - Notacion Postfija (Polaca Inversa)",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 900, 600,
        NULL, NULL, hInstancia, NULL
    );

    ShowWindow(hwnd, SW_SHOW);

    MSG mensaje;
    while (GetMessage(&mensaje, NULL, 0, 0)) {
        TranslateMessage(&mensaje);
        DispatchMessage(&mensaje);
    }
}

int main() {
    string linea;
    cout << "=== ARBOL DE EXPRESION (NOTACION POSTFIJA / POLACA INVERSA) ===" << endl;
    cout << "Ingresa la expresion postfija separando cada token con un espacio." << endl;
    cout << "Ejemplo: 3 4 + 2 *   (equivale a (3 + 4) * 2)" << endl;
    cout << "> ";
    getline(cin, linea);

    stringstream ss(linea);
    string token;
    vector<string> tokens;
    while (ss >> token) {
        tokens.push_back(token);
    }

    arbol.construirDesdePostfija(tokens);

    cout << endl << "--- RECORRIDOS DEL ARBOL ---" << endl;
    arbol.preorden();
    arbol.inorden();
    arbol.postorden();

    if (arbol.obtenerRaiz() != nullptr) {
        cout << endl << "Abriendo ventana grafica con el arbol..." << endl;
        HINSTANCE hInstancia = GetModuleHandle(NULL);
        mostrarVentanaGrafica(hInstancia);
    }

    return 0;
}
