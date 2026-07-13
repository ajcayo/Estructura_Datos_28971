/*
 * ============================================================
 *  PROBLEMA DE LAS N REINAS - BACKTRACKING (VERSION GRAFICA)
 *  Usando Win32 API puro (windows.h)
 * ============================================================
 *  Este programa:
 *   1. Pide N por consola.
 *   2. Abre una ventana grafica con el tablero de N x N.
 *   3. Va colocando y quitando reinas en pantalla en tiempo real
 *      mientras corre el backtracking (se ve el "retroceso").
 *
 *  COMPILAR (en la terminal de VS Code, con MinGW/g++ instalado):
 *      g++ n_reinas_grafico.cpp -o n_reinas_grafico.exe -lgdi32 -luser32
 *
 *  EJECUTAR:
 *      .\n_reinas_grafico.exe
 * ============================================================
 */

#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>

// ---------- Variables globales ----------
int N;                  // tamano del tablero / numero de reinas
int *tablero;            // tablero[fila] = columna de la reina en esa fila (-1 = vacia)
int pasos = 0;
int soluciones = 0;
int delayMs = 300;       // velocidad de la animacion
bool soloPrimera = true; // si true, se detiene al encontrar la primera solucion
bool detener = false;    // se activa si el usuario cierra la ventana

HWND hwndMain;
const int cellSize = 60;
const int margin = 20;
const wchar_t CLASS_NAME[] = L"VentanaReinas";

// ---------- Utilidades de la ventana ----------

// Procesa los mensajes pendientes de Windows para que la ventana
// no se quede "sin responder" mientras el backtracking trabaja.
void procesarMensajes() {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) { detener = true; }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// Pausa "inteligente": espera ms milisegundos pero sigue
// bombeando mensajes para que la ventana no se congele.
void pausa(int ms) {
    int transcurrido = 0;
    while (transcurrido < ms && !detener) {
        procesarMensajes();
        Sleep(10);
        transcurrido += 10;
    }
}

// Fuerza que se vuelva a dibujar la ventana ya mismo.
void refrescar() {
    InvalidateRect(hwndMain, NULL, FALSE);
    UpdateWindow(hwndMain);
    procesarMensajes();
}

// ---------- Dibujo del tablero (mensaje WM_PAINT) ----------
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_DESTROY:
            detener = true;
            PostQuitMessage(0);
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT rc;
            GetClientRect(hwnd, &rc);

            // Doble buffer para que no parpadee al redibujar
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBM = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
            HBITMAP oldBM = (HBITMAP)SelectObject(memDC, memBM);

            HBRUSH fondo = CreateSolidBrush(RGB(245, 245, 245));
            FillRect(memDC, &rc, fondo);
            DeleteObject(fondo);

            // Casillas del tablero, alternando colores
            for (int f = 0; f < N; f++) {
                for (int c = 0; c < N; c++) {
                    RECT celda;
                    celda.left   = margin + c * cellSize;
                    celda.top    = margin + f * cellSize;
                    celda.right  = celda.left + cellSize;
                    celda.bottom = celda.top + cellSize;

                    COLORREF color = ((f + c) % 2 == 0)
                                        ? RGB(238, 238, 210)   // clara
                                        : RGB(118, 150, 86);   // oscura
                    HBRUSH brocha = CreateSolidBrush(color);
                    FillRect(memDC, &celda, brocha);
                    DeleteObject(brocha);
                }
            }

            // Reinas (se dibujan como "Q" en rojo)
            HFONT fuente = CreateFontW(
                cellSize / 2, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Arial");
            HFONT oldFont = (HFONT)SelectObject(memDC, fuente);
            SetBkMode(memDC, TRANSPARENT);
            SetTextColor(memDC, RGB(180, 20, 20));

            for (int f = 0; f < N; f++) {
                if (tablero[f] != -1) {
                    int c = tablero[f];
                    RECT celda;
                    celda.left   = margin + c * cellSize;
                    celda.top    = margin + f * cellSize;
                    celda.right  = celda.left + cellSize;
                    celda.bottom = celda.top + cellSize;
                    DrawTextW(memDC, L"Q", -1, &celda,
                              DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                }
            }

            SelectObject(memDC, oldFont);
            DeleteObject(fuente);

            BitBlt(hdc, 0, 0, rc.right, rc.bottom, memDC, 0, 0, SRCCOPY);

            SelectObject(memDC, oldBM);
            DeleteObject(memBM);
            DeleteDC(memDC);

            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// ---------- Logica del backtracking ----------
int esSeguro(int fila, int col) {
    for (int f = 0; f < fila; f++) {
        int c = tablero[f];
        if (c == col) return 0;                      // misma columna
        if (abs(c - col) == abs(f - fila)) return 0;  // misma diagonal
    }
    return 1;
}

void colocarReinas(int fila) {
    if (detener) return;

    if (fila == N) {
        soluciones++;
        refrescar();
        pausa(1500); // pausa mas larga para apreciar la solucion
        return;
    }

    for (int col = 0; col < N && !detener; col++) {
        pasos++;

        if (esSeguro(fila, col)) {
            // Colocar la reina y mostrarlo en pantalla
            tablero[fila] = col;
            refrescar();
            pausa(delayMs);

            colocarReinas(fila + 1);

            if (soloPrimera && soluciones > 0) return; // ya encontramos una, no seguimos

            // BACKTRACK: quitar la reina y mostrarlo en pantalla
            tablero[fila] = -1;
            refrescar();
            pausa(delayMs / 2);
        }
    }
}

// ---------- Programa principal ----------
int main() {
    printf("=========================================\n");
    printf("   N REINAS - VERSION GRAFICA (Win32)\n");
    printf("=========================================\n\n");

    printf("Ingrese el numero de reinas N: ");
    if (scanf("%d", &N) != 1 || N <= 0) {
        printf("Numero invalido.\n");
        return 1;
    }

    printf("Milisegundos de espera entre pasos (ej. 300): ");
    scanf("%d", &delayMs);

    char opcion;
    printf("Detenerse en la primera solucion encontrada? (s/n): ");
    scanf(" %c", &opcion);
    soloPrimera = (opcion == 's' || opcion == 'S');

    tablero = new int[N];
    for (int i = 0; i < N; i++) tablero[i] = -1;

    // Registrar la clase de ventana
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASSW wc = {};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    RegisterClassW(&wc);

    int anchoVentana = margin * 2 + cellSize * N + 16;
    int altoVentana  = margin * 2 + cellSize * N + 40;

    wchar_t titulo[64];
    wsprintfW(titulo, L"N Reinas (N = %d) - Backtracking", N);

    hwndMain = CreateWindowExW(
        0, CLASS_NAME, titulo,
        (WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME),
        CW_USEDEFAULT, CW_USEDEFAULT, anchoVentana, altoVentana,
        NULL, NULL, hInstance, NULL);

    if (!hwndMain) {
        printf("No se pudo crear la ventana.\n");
        return 1;
    }

    ShowWindow(hwndMain, SW_SHOW);
    UpdateWindow(hwndMain);

    printf("\nVentana grafica abierta. Buscando solucion...\n");

    colocarReinas(0);

    if (!detener) {
        printf("\nProceso terminado.\n");
        printf("Soluciones encontradas: %d\n", soluciones);
        printf("Pasos (intentos) realizados: %d\n", pasos);
        printf("La ventana sigue abierta, cierrela para salir.\n");

        // Mantiene la ventana viva hasta que el usuario la cierre
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    delete[] tablero;
    return 0;
}
