#define NOMINMAX
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <windows.h>

// Variables globales para el estado del tablero
int g_n = 8;
int g_filaCaballo = 0;
int g_colCaballo = 0;
int** g_tablero = nullptr;

// 1. Genera el archivo limpio con ceros en disco[cite: 3]
void generarArchivoTablero(int n) {
    std::ofstream archivo("tablero.txt"); // Nombre exacto requerido[cite: 3]
    if (!archivo.is_open()) return;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            archivo << 0 << " ";
        }
        archivo << "\n";
    }
    archivo.close();
}

// 2. Lee el archivo TXT y crea la matriz con punteros dobles (Sin corchetes [] ni vectores)[cite: 3]
void leerYCrearTableroMatriz() {
    std::ifstream archivo("tablero.txt");
    if (!archivo.is_open()) return;

    g_tablero = (int**)malloc(g_n * sizeof(int*));
    for (int i = 0; i < g_n; ++i) {
        *(g_tablero + i) = (int*)malloc(g_n * sizeof(int));
    }

    for (int i = 0; i < g_n; ++i) {
        for (int j = 0; j < g_n; ++j) {
            archivo >> *(*(g_tablero + i) + j);
        }
    }
    archivo.close();
}

// 3. Recalcula los movimientos válidos en "L" (distancia al cuadrado == 5)[cite: 3]
void actualizarMovimientosCaballo() {
    // Limpiar el tablero previo
    for (int i = 0; i < g_n; ++i) {
        for (int j = 0; j < g_n; ++j) {
            *(*(g_tablero + i) + j) = 0;
        }
    }

    // Ubicar al caballo (representado con el 9)[cite: 3]
    *(*(g_tablero + g_filaCaballo) + g_colCaballo) = 9;

    // Buscar posiciones en "L" analíticamente[cite: 3]
    for (int i = 0; i < g_n; ++i) {
        for (int j = 0; j < g_n; ++j) {
            int difFila = abs(i - g_filaCaballo);
            int difCol = abs(j - g_colCaballo);
            if ((difFila * difFila + difCol * difCol) == 5) {
                *(*(g_tablero + i) + j) = 1; // 1 = Casilla destino válida
            }
        }
    }
}

// 4. Guarda el estado actual en el archivo de texto tras cada movimiento válido
void guardarProgresoEnArchivo() {
    std::ofstream archivo("tablero.txt");
    if (!archivo.is_open()) return;

    for (int i = 0; i < g_n; ++i) {
        for (int j = 0; j < g_n; ++j) {
            archivo << *(*(g_tablero + i) + j) << " ";
        }
        archivo << "\n";
    }
    archivo.close();
}

// Procedimiento de Ventana: Renderizado y captura de clics del mouse
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    int tamCasilla = 50; // Dimensión de cada celda[cite: 3]

    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            for (int i = 0; i < g_n; ++i) {
                for (int j = 0; j < g_n; ++j) {
                    int x1 = j * tamCasilla;
                    int y1 = i * tamCasilla;
                    int x2 = x1 + tamCasilla;
                    int y2 = y1 + tamCasilla;

                    int valorCelda = *(*(g_tablero + i) + j);
                    HBRUSH brocha;

                    if (valorCelda == 9) {
                        brocha = CreateSolidBrush(RGB(0, 110, 230)); // Azul: Caballo[cite: 3]
                    } else if (valorCelda == 1) {
                        brocha = CreateSolidBrush(RGB(50, 210, 105)); // Verde: Destinos válidos[cite: 3]
                    } else {
                        // Fondo del tablero de ajedrez
                        if ((i + j) % 2 == 0) brocha = CreateSolidBrush(RGB(245, 245, 245));
                        else brocha = CreateSolidBrush(RGB(70, 70, 70));
                    }

                    SelectObject(hdc, brocha);
                    Rectangle(hdc, x1, y1, x2, y2);

                    // Círculo decorativo sobre el caballo[cite: 3]
                    if (valorCelda == 9) {
                        HBRUSH brochaInterior = CreateSolidBrush(RGB(255, 255, 255));
                        SelectObject(hdc, brochaInterior);
                        Ellipse(hdc, x1 + 12, y1 + 12, x2 - 12, y2 - 12);
                        DeleteObject(brochaInterior);
                    }
                    DeleteObject(brocha);
                }
            }
            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_LBUTTONDOWN: {
            // Obtener coordenadas de los píxeles del clic
            int xClick = LOWORD(lParam);
            int yClick = HIWORD(lParam);

            // Convertir píxeles a índices de la matriz (fila, columna)
            int colDestino = xClick / tamCasilla;
            int filaDestino = yClick / tamCasilla;

            // Validar que el clic esté dentro de los límites del tablero
            if (filaDestino >= 0 && filaDestino < g_n && colDestino >= 0 && colDestino < g_n) {
                // RESTRICCIÓN: Solo se mueve si la casilla seleccionada tiene un '1' (es verde)
                if (*(*(g_tablero + filaDestino) + colDestino) == 1) {
                    g_filaCaballo = filaDestino;
                    g_colCaballo = colDestino;

                    // Actualizar matriz y refrescar el archivo físico de texto
                    actualizarMovimientosCaballo();
                    guardarProgresoEnArchivo();

                    // Forzar el redibujado de la ventana gráfica
                    InvalidateRect(hwnd, NULL, TRUE);
                }
            }
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int main() {
    system("");

    std::cout << "=====================================================\n";
    std::cout << "     TALLER AJEDREZ: MOVIMIENTOS RESTRINGIDOS EN L    \n";
    std::cout << "=====================================================\n\n";

    std::cout << "Ingrese la dimension N para el tablero (Ej: 8): ";
    std::cin >> g_n;

    if (g_n < 3) {
        std::cout << "El tablero debe ser de al menos 3x3.\n";
        return 1;
    }

    generarArchivoTablero(g_n);[cite: 3]
    leerYCrearTableroMatriz();[cite: 3]

    std::cout << "\nUbicacion inicial del caballo:\n";
    std::cout << "Fila (0 a " << g_n - 1 << "): ";
    std::cin >> g_filaCaballo;
    std::cout << "Columna (0 a " << g_n - 1 << "): ";
    std::cin >> g_colCaballo;

    if (g_filaCaballo < 0 || g_filaCaballo >= g_n || g_colCaballo < 0 || g_colCaballo >= g_n) {
        std::cout << "Posicion fuera de rango.\n";
        return 1;
    }

    actualizarMovimientosCaballo();[cite: 3]
    guardarProgresoEnArchivo();

    std::cout << "\n[OK] ¡Abriendo ventana grafica!\n";
    std::cout << "-> Haz CLIC IZQUIERDO sobre cualquiera de las CASILLAS VERDES para mover el caballo.\n";

    // Inicialización de la ventana de Windows (Win32)
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "AjedrezLogicaEstricta";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, "AjedrezLogicaEstricta", "Reglas del Ajedrez: Saltos en L",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, (g_n * 50) + 16, (g_n * 50) + 39,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 0;

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Liberación estricta de la memoria dinámica[cite: 3]
    for (int i = 0; i < g_n; ++i) {
        free(*(g_tablero + i));
    }
    free(g_tablero);

    return 0;
}
