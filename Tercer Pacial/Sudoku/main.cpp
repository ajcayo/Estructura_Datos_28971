#include <windows.h>
#include <string>
#include <iostream>

// ===================== CONFIGURACION DEL TABLERO =====================
const int CELL = 60;                 
const int MARGIN = 40;               
const int BOARD_SIZE = CELL * 9;     
const int WIN_WIDTH  = BOARD_SIZE + MARGIN * 2;
const int WIN_HEIGHT = BOARD_SIZE + MARGIN * 2 + 50; // Ajustado el alto ya que no hay boton

struct Casilla {
    int valor;        
    bool esFija;      
};

Casilla tablero[9][9] = {
    {{5,true},  {3,true},  {0,false}, {0,false}, {7,true},  {0,false}, {0,false}, {0,false}, {0,false}},
    {{6,true},  {0,false}, {0,false}, {1,true},  {9,true},  {5,true},  {0,false}, {0,false}, {0,false}},
    {{0,false}, {9,true},  {8,true},  {0,false}, {0,false}, {0,false}, {0,false}, {6,true},  {0,false}},
    {{8,true},  {0,false}, {0,false}, {0,false}, {6,true},  {0,false}, {0,false}, {0,false}, {3,true}},
    {{4,true},  {0,false}, {0,false}, {8,true},  {0,false}, {3,true},  {0,false}, {0,false}, {1,true}},
    {{7,true},  {0,false}, {0,false}, {0,false}, {2,true},  {0,false}, {0,false}, {0,false}, {6,true}},
    {{0,false}, {6,true},  {0,false}, {0,false}, {0,false}, {0,false}, {2,true},  {8,true},  {0,false}},
    {{0,false}, {0,false}, {0,false}, {4,true},  {1,true},  {9,true},  {0,false}, {0,false}, {5,true}},
    {{0,false}, {0,false}, {0,false}, {0,false}, {8,true},  {0,false}, {0,false}, {7,true},  {9,true}}
};

int selRow = -1;
int selCol = -1;
std::string mensajeEstado = "Clic para seleccionar. Presiona 1-9 para escribir, o BACKSPACE para borrar.";

// ===================== LOGICA CON ARITMETICA DE PUNTEROS =====================

bool esMovimientoValido(int row, int col, int num) {
    if (num == 0) return true; 

    for (int c = 0; c < 9; c++) {
        Casilla* actual = *(tablero + row) + c;
        if (c != col && actual->valor == num) return false;
    }

    for (int r = 0; r < 9; r++) {
        Casilla* actual = *(tablero + r) + col;
        if (r != row && actual->valor == num) return false;
    }

    int boxRowStart = (row / 3) * 3;
    int boxColStart = (col / 3) * 3;
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            int actualRow = boxRowStart + r;
            int actualCol = boxColStart + c;
            Casilla* actual = *(tablero + actualRow) + actualCol;
            if ((actualRow != row || actualCol != col) && actual->valor == num) return false;
        }
    }
    return true; 
}

bool comprobarVictoria() {
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            Casilla* actual = *(tablero + r) + c;
            if (actual->valor == 0) return false; 
        }
    }
    return true; 
}

void imprimirEnConsola() {
    std::cout << "\n=== SUDOKU SOLUCIONADO EN CONSOLA ===\n";
    for (int r = 0; r < 9; r++) {
        if (r > 0 && r % 3 == 0) {
            std::cout << "------+-------+------\n";
        }
        for (int c = 0; c < 9; c++) {
            if (c > 0 && c % 3 == 0) {
                std::cout << "| ";
            }
            Casilla* actual = *(tablero + r) + c;
            std::cout << actual->valor << " ";
        }
        std::cout << "\n";
    }
    std::cout << "=====================================\n\n";
}

bool resolverSudokuPunteros() {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            Casilla* actual = *(tablero + row) + col;
            
            if (actual->valor == 0) {
                for (int num = 1; num <= 9; num++) {
                    if (esMovimientoValido(row, col, num)) {
                        actual->valor = num; 

                        if (resolverSudokuPunteros()) {
                            return true; 
                        }

                        actual->valor = 0; 
                    }
                }
                return false; 
            }
        }
    }
    return true; 
}

// Resuelve una copia temporal para no alterar el juego visual de inmediato
bool resolverYMostrarEnConsola() {
    // Creamos una matriz temporal para resolverla sin destruir el estado inicial del juego visual
    Casilla tableroCopia[9][9];
    
    // Copiar datos usando punteros
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            (*(tableroCopia + r) + c)->valor = (*(tablero + r) + c)->valor;
            (*(tableroCopia + r) + c)->esFija = (*(tablero + r) + c)->esFija;
        }
    }

    // Intercambiamos temporalmente el puntero global para que apueste a la copia
    // Como tablero es un arreglo bidimensional estatico, pasamos una funcion lambda auxiliar local
    // para resolver de manera directa sobre la copia usando la misma logica de punteros.
    struct SolverInterno {
        static bool resolver(Casilla unTablero[9][9], bool (*valido)(int, int, int)) {
            for (int row = 0; row < 9; row++) {
                for (int col = 0; col < 9; col++) {
                    Casilla* actual = *(unTablero + row) + col;
                    if (actual->valor == 0) {
                        for (int num = 1; num <= 9; num++) {
                            // Simulacion de validacion local simplificada para la copia
                            bool esValido = true;
                            for (int c = 0; c < 9; c++) {
                                if (c != col && (*(unTablero + row) + c)->valor == num) esValido = false;
                            }
                            for (int r = 0; r < 9; r++) {
                                if (r != row && (*(unTablero + r) + col)->valor == num) esValido = false;
                            }
                            int bR = (row / 3) * 3, bC = (col / 3) * 3;
                            for (int r = 0; r < 3; r++) {
                                for (int c = 0; c < 3; c++) {
                                    if ((bR + r != row || bC + c != col) && (*(unTablero + bR + r) + bC + c)->valor == num) esValido = false;
                                }
                            }

                            if (esValido) {
                                actual->valor = num;
                                if (resolver(unTablero, valido)) return true;
                                actual->valor = 0;
                            }
                        }
                        return false;
                    }
                }
            }
            return true;
        }
    };

    if (SolverInterno::resolver(tableroCopia, esMovimientoValido)) {
        std::cout << "\n=== SUDOKU SOLUCIONADO EN CONSOLA ===\n";
        for (int r = 0; r < 9; r++) {
            if (r > 0 && r % 3 == 0) std::cout << "------+-------+------\n";
            for (int c = 0; c < 9; c++) {
                if (c > 0 && c % 3 == 0) std::cout << "| ";
                std::cout << (*(tableroCopia + r) + c)->valor << " ";
            }
            std::cout << "\n";
        }
        std::cout << "=====================================\n\n";
        return true;
    }
    std::cout << "No se pudo encontrar una solucion para este tablero.\n";
    return false;
}

// ===================== INTERFAZ GRAFICA =====================

bool pantallaATablero(int x, int y, int &row, int &col) {
    if (x < MARGIN || y < MARGIN) return false;
    if (x >= MARGIN + BOARD_SIZE || y >= MARGIN + BOARD_SIZE) return false;
    col = (x - MARGIN) / CELL;
    row = (y - MARGIN) / CELL;
    return true;
}

void dibujarTablero(HDC hdc) {
    HFONT hFontNum = CreateFontA(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
    HFONT hFontInst = CreateFontA(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
    
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            int x = MARGIN + col * CELL;
            int y = MARGIN + row * CELL;

            HBRUSH brush;
            if (row == selRow && col == selCol) {
                brush = CreateSolidBrush(RGB(173, 216, 230));
            } else {
                brush = CreateSolidBrush(RGB(255, 255, 255));
            }

            RECT r = { x, y, x + CELL, y + CELL };
            FillRect(hdc, &r, brush);
            DeleteObject(brush);

            HPEN penFino = CreatePen(PS_SOLID, 1, RGB(210, 210, 210));
            HPEN oldPen = (HPEN)SelectObject(hdc, penFino);
            HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
            Rectangle(hdc, x, y, x + CELL + 1, y + CELL + 1);
            SelectObject(hdc, oldPen);
            DeleteObject(penFino);

            Casilla* casillaActual = *(tablero + row) + col;

            if (casillaActual->valor != 0) {
                std::string numStr = std::to_string(casillaActual->valor);
                SetBkMode(hdc, TRANSPARENT);
                
                if (casillaActual->esFija) {
                    SetTextColor(hdc, RGB(0, 0, 0));
                } else {
                    SetTextColor(hdc, RGB(0, 102, 204));
                }

                HFONT oldFont = (HFONT)SelectObject(hdc, hFontNum);
                RECT textRect = { x, y + 10, x + CELL, y + CELL };
                DrawTextA(hdc, numStr.c_str(), -1, &textRect, DT_CENTER | DT_SINGLELINE);
                SelectObject(hdc, oldFont);
            }
            SelectObject(hdc, oldBrush);
        }
    }

    HPEN penGrueso = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
    HPEN oldPen = (HPEN)SelectObject(hdc, penGrueso);
    for (int i = 0; i <= 9; i += 3) {
        MoveToEx(hdc, MARGIN + i * CELL, MARGIN, NULL);
        LineTo(hdc, MARGIN + i * CELL, MARGIN + BOARD_SIZE);
        MoveToEx(hdc, MARGIN, MARGIN + i * CELL, NULL);
        LineTo(hdc, MARGIN + BOARD_SIZE, MARGIN + i * CELL);
    }
    SelectObject(hdc, oldPen);
    DeleteObject(penGrueso);

    HFONT oldFont = (HFONT)SelectObject(hdc, hFontInst);
    
    if (mensajeEstado.find("GANASTE") != std::string::npos) {
        SetTextColor(hdc, RGB(34, 139, 34)); 
    } else if (mensajeEstado.find("ERROR") != std::string::npos) {
        SetTextColor(hdc, RGB(220, 20, 60));  
    } else {
        SetTextColor(hdc, RGB(40, 40, 40));    
    }

    TextOutA(hdc, MARGIN, MARGIN + BOARD_SIZE + 15, mensajeEstado.c_str(), (int)mensajeEstado.size());
    SelectObject(hdc, oldFont);

    DeleteObject(hFontNum);
    DeleteObject(hFontInst);
}

// ===================== MANEJADOR DE EVENTOS =====================

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rc;
            GetClientRect(hwnd, &rc);
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
            HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

            HBRUSH fondo = CreateSolidBrush(RGB(240, 240, 240)); 
            FillRect(memDC, &rc, fondo);
            DeleteObject(fondo);

            dibujarTablero(memDC);

            BitBlt(hdc, 0, 0, rc.right, rc.bottom, memDC, 0, 0, SRCCOPY);
            SelectObject(memDC, oldBitmap);
            DeleteObject(memBitmap);
            DeleteDC(memDC);
            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_LBUTTONDOWN: {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            int row, col;
            if (pantallaATablero(x, y, row, col)) {
                selRow = row;
                selCol = col;
            } else {
                selRow = -1; 
                selCol = -1;
            }
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        }

        case WM_KEYDOWN: {
            if (selRow != -1 && selCol != -1) {
                Casilla* casillaActual = *(tablero + selRow) + selCol;

                if (!casillaActual->esFija) {
                    int numeroIngresado = -1;

                    if (wParam >= '1' && wParam <= '9') {
                        numeroIngresado = wParam - '0';
                    }
                    else if (wParam >= VK_NUMPAD1 && wParam <= VK_NUMPAD9) {
                        numeroIngresado = wParam - VK_NUMPAD1 + 1;
                    }
                    else if (wParam == VK_BACK || wParam == VK_DELETE) {
                        numeroIngresado = 0; 
                    }

                    if (numeroIngresado != -1) {
                        if (esMovimientoValido(selRow, selCol, numeroIngresado)) {
                            casillaActual->valor = numeroIngresado;
                            
                            if (comprobarVictoria()) {
                                mensajeEstado = "GANASTE! Has completado el Sudoku correctamente. Felicitaciones!";
                            } else {
                                mensajeEstado = "Movimiento aceptado. Sigue asi!";
                            }
                        } else {
                            Beep(600, 250); 
                            mensajeEstado = "ERROR matematico! El numero ya existe en la fila, columna o region de 3x3.";
                        }
                        InvalidateRect(hwnd, NULL, TRUE);
                    }
                }
            }
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int main() {
    // Ejecutar resolucion automatica en el CMD inmediatamente al abrir el programa
    std::cout << "Iniciando Sudoku...\n";
    resolverYMostrarEnConsola();

    HINSTANCE hInstance = GetModuleHandle(NULL);
    const char CLASS_NAME[] = "VentanaSudokuResolucionDirecta";

    WNDCLASS wc = { };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "Sudoku Arbitrado - Solucion Instantanea en Consola",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
        WIN_WIDTH + 16, WIN_HEIGHT + 39, 
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, SW_SHOW);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}









