#include <windows.h>
#include <string>
#include <iostream>

// ===================== CONFIGURACIÓN DE LA VENTANA GRÁFICA =====================
const int CELL = 60;                // Tamaño de cada celda del Cuadrado Latino
const int MARGIN = 50;              // Margen superior e izquierdo
int N = 5;                          // Orden del cuadrado (por defecto 5, se actualizará)
int* cuadrado = nullptr;            // Puntero dinámico para almacenar la matriz

// Dimensiones de la ventana (se calcularán dinámicamente según N)
int winWidth = 400;
int winHeight = 400;

// ===================== FUNCIONES AUXILIARES =====================

// Genera la lógica del cuadrado latino usando aritmética de punteros
void generarCuadradoLatino() {
    // Reservamos memoria dinámicamente para NxN enteros sin usar corchetes
    cuadrado = (int*)malloc(N * N * sizeof(int));
    if (cuadrado == nullptr) return;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int valor = (i + j) % N + 1;
            // Equivalente a cuadrado[i][j] = valor sin usar corchetes
            *(cuadrado + (i * N) + j) = valor;
        }
    }
}

// Dibuja la cuadrícula y los números del cuadrado latino en la ventana
void dibujarCuadradoLatino(HDC hdc) {
    if (cuadrado == nullptr) return;

    SetBkMode(hdc, TRANSPARENT);
    
    // Configurar fuente de texto más grande para los números
    HFONT hFont = CreateFontA(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int x = MARGIN + j * CELL;
            int y = MARGIN + i * CELL;

            // Conseguimos el valor usando aritmética de punteros
            int numero = *(cuadrado + (i * N) + j);

            // Generar un color único y suave de fondo basado en el número para cada casilla
            int r = (numero * 45) % 150 + 100;
            int g = (numero * 75) % 150 + 100;
            int b = (numero * 105) % 150 + 100;

            HBRUSH brushCasilla = CreateSolidBrush(RGB(r, g, b));
            RECT rRect = { x, y, x + CELL, y + CELL };
            FillRect(hdc, &rRect, brushCasilla);
            DeleteObject(brushCasilla);

            // Dibujar los bordes negros de la celda
            HPEN penBorde = CreatePen(PS_SOLID, 2, RGB(40, 40, 40));
            HPEN oldPen = (HPEN)SelectObject(hdc, penBorde);
            HBRUSH brushHueco = (HBRUSH)GetStockObject(NULL_BRUSH);
            HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brushHueco);
            
            Rectangle(hdc, x, y, x + CELL + 1, y + CELL + 1);
            
            SelectObject(hdc, oldPen);
            SelectObject(hdc, oldBrush);
            DeleteObject(penBorde);

            // Dibujar el número centrado dentro de la celda
            std::string textoNum = std::to_string(numero);
            SIZE textSize;
            GetTextExtentPoint32A(hdc, textoNum.c_str(), (int)textoNum.size(), &textSize);
            int textX = x + (CELL - textSize.cx) / 2;
            int textY = y + (CELL - textSize.cy) / 2;

            SetTextColor(hdc, RGB(20, 20, 20));
            TextOutA(hdc, textX, textY, textoNum.c_str(), (int)textoNum.size());
        }
    }

    // Dibujar título informativo en la ventana gráfica
    std::string titulo = "Cuadrado Latino de Orden " + std::to_string(N);
    SetTextColor(hdc, RGB(0, 0, 0));
    TextOutA(hdc, MARGIN, MARGIN - 35, titulo.c_str(), (int)titulo.size());

    SelectObject(hdc, oldFont);
    DeleteObject(hFont);
}

// ===================== PROCEDIMIENTO DE VENTANA (WndProc) =====================
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Doble buffer para evitar parpadeos al redibujar
            RECT rc;
            GetClientRect(hwnd, &rc);
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
            HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

            // Fondo blanco para la ventana gráfica
            HBRUSH fondo = CreateSolidBrush(RGB(245, 245, 245));
            FillRect(memDC, &rc, fondo);
            DeleteObject(fondo);

            // Renderizamos los elementos gráficos en el buffer secundario
            dibujarCuadradoLatino(memDC);

            // Copiamos al lienzo real de la pantalla
            BitBlt(hdc, 0, 0, rc.right, rc.bottom, memDC, 0, 0, SRCCOPY);

            SelectObject(memDC, oldBitmap);
            DeleteObject(memBitmap);
            DeleteDC(memDC);

            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            if (cuadrado != nullptr) {
                free(cuadrado); // Liberamos la memoria dinámica
            }
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// ===================== ENTRADA DE CONTROL PRINCIPAL =====================
int main() {
    // 1. Pedir los datos en la consola estándar primero
    std::cout << "========================================\n";
    std::cout << "      ENTRADA DE DATOS GRÁFICOS         \n";
    std::cout << "========================================\n";
    std::cout << "Ingrese el orden del cuadrado (N): ";
    std::cin >> N;

    if (N <= 0) {
        std::cout << "Orden no válido. Saliendo del programa.\n";
        return 1;
    }

    // Calcular el espacio de ventana necesario en base al N ingresado
    winWidth = (CELL * N) + (MARGIN * 2) + 16;
    winHeight = (CELL * N) + (MARGIN * 2) + 39;

    // Generar la matriz matemática en memoria
    generarCuadradoLatino();

    // 2. Iniciar la ventana gráfica independiente (Sub-página)
    HINSTANCE hInstance = GetModuleHandle(NULL);
    const char CLASS_NAME[] = "VentanaCuadradoLatino";

    WNDCLASS wc = { };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Lattice - Cuadrado Latino Visual",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, // Bloquea redimensionado
        CW_USEDEFAULT, CW_USEDEFAULT,
        winWidth, winHeight,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    // Mostramos la interfaz gráfica
    ShowWindow(hwnd, SW_SHOW);
    std::cout << "\n[OK] Ventana grafica abierta exitosamente.\n";

    // Bucle de mensajes clásico de Windows de tu plantilla
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}