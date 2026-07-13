#include <windows.h>
#include <string>

// ===================== CONFIGURACION DEL TABLERO =====================
const int CELL = 70;                 // tamaño de cada casilla
const int MARGIN = 40;               // margen para las coordenadas (a-h, 1-8)
const int BOARD_SIZE = CELL * 8;
const int WIN_WIDTH  = BOARD_SIZE + MARGIN * 2;
const int WIN_HEIGHT = BOARD_SIZE + MARGIN * 2 + 40; // +40 para el texto de instrucciones

// Posicion actual del alfil (fila, columna) -> 0 a 7
int alfilRow = 0;
int alfilCol = 2;

// ===================== FUNCIONES AUXILIARES =====================

// Verifica si (row, col) es un movimiento diagonal valido desde la posicion del alfil
bool esMovimientoDiagonalValido(int row, int col) {
    if (row == alfilRow && col == alfilCol) return false; // misma casilla, no cuenta
    int dr = row - alfilRow;
    int dc = col - alfilCol;
    if (dr < 0) dr = -dr;
    if (dc < 0) dc = -dc;
    return (dr == dc); // diagonal valida si el desplazamiento es igual en ambos ejes
}

// Convierte coordenadas de pantalla (x,y) a (row, col) del tablero
bool pantallaATablero(int x, int y, int &row, int &col) {
    if (x < MARGIN || y < MARGIN) return false;
    if (x >= MARGIN + BOARD_SIZE || y >= MARGIN + BOARD_SIZE) return false;
    col = (x - MARGIN) / CELL;
    row = (y - MARGIN) / CELL;
    return true;
}

// Dibuja el alfil (figura grafica) centrado en la casilla (row, col)
void dibujarAlfil(HDC hdc, int row, int col) {
    int cx = MARGIN + col * CELL + CELL / 2; // centro X de la casilla
    int cy = MARGIN + row * CELL + CELL / 2; // centro Y de la casilla

    HBRUSH brushCuerpo = CreateSolidBrush(RGB(20, 20, 20));
    HBRUSH brushViejo = (HBRUSH)SelectObject(hdc, brushCuerpo);
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 215, 0)); // contorno dorado
    HPEN penViejo = (HPEN)SelectObject(hdc, pen);

    // Base del alfil (rectangulo pequeño)
    RECT baseRect = { cx - 18, cy + 18, cx + 18, cy + 26 };
    Rectangle(hdc, baseRect.left, baseRect.top, baseRect.right, baseRect.bottom);

    // Cuerpo del alfil (triangulo/cono)
    // En vez de usar un arreglo con corchetes [ ], reservamos memoria
    // con "new" y usamos un puntero para llenar cada punto.
    POINT* cuerpo = new POINT[4];
    (cuerpo + 0)->x = cx - 14; (cuerpo + 0)->y = cy + 18;
    (cuerpo + 1)->x = cx + 14; (cuerpo + 1)->y = cy + 18;
    (cuerpo + 2)->x = cx + 4;  (cuerpo + 2)->y = cy - 15;
    (cuerpo + 3)->x = cx - 4;  (cuerpo + 3)->y = cy - 15;
    Polygon(hdc, cuerpo, 4);
    delete[] cuerpo; // liberamos la memoria que reservamos

    // Cabeza del alfil (circulo)
    Ellipse(hdc, cx - 13, cy - 30, cx + 13, cy - 4);

    // Corte diagonal caracteristico del alfil (linea blanca sobre la cabeza)
    HPEN penCorte = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    SelectObject(hdc, penCorte);
    MoveToEx(hdc, cx - 6, cy - 22, NULL);
    LineTo(hdc, cx + 6, cy - 10);
    DeleteObject(penCorte);

    // Bolita superior (cruz/remate del alfil)
    SelectObject(hdc, brushCuerpo);
    Ellipse(hdc, cx - 5, cy - 40, cx + 5, cy - 30);

    SelectObject(hdc, brushViejo);
    SelectObject(hdc, penViejo);
    DeleteObject(brushCuerpo);
    DeleteObject(pen);
}

// Dibuja todo el tablero, resaltados y el alfil
void dibujarTablero(HDC hdc) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            int x = MARGIN + col * CELL;
            int y = MARGIN + row * CELL;

            bool esClara = ((row + col) % 2 == 0);
            HBRUSH brush;

            if (esMovimientoDiagonalValido(row, col)) {
                // Casilla diagonal valida -> resaltada en verde
                brush = CreateSolidBrush(esClara ? RGB(140, 220, 140) : RGB(90, 170, 90));
            } else {
                brush = CreateSolidBrush(esClara ? RGB(240, 217, 181) : RGB(181, 136, 99));
            }

            RECT r = { x, y, x + CELL, y + CELL };
            FillRect(hdc, &r, brush);
            DeleteObject(brush);

            // Borde de la casilla
            HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
            HPEN penViejo = (HPEN)SelectObject(hdc, pen);
            HBRUSH brushHueco = (HBRUSH)GetStockObject(NULL_BRUSH);
            HBRUSH brushViejo = (HBRUSH)SelectObject(hdc, brushHueco);
            Rectangle(hdc, x, y, x + CELL + 1, y + CELL + 1);
            SelectObject(hdc, penViejo);
            SelectObject(hdc, brushViejo);
            DeleteObject(pen);
        }
    }

    // Coordenadas de columnas (a-h)
    SetBkMode(hdc, TRANSPARENT);
    for (int col = 0; col < 8; col++) {
        std::string letra(1, 'a' + col);
        TextOutA(hdc, MARGIN + col * CELL + CELL / 2 - 5, MARGIN + BOARD_SIZE + 5, letra.c_str(), (int)letra.size());
    }
    // Coordenadas de filas (8 arriba -> 1 abajo, estilo ajedrez)
    for (int row = 0; row < 8; row++) {
        std::string numero = std::to_string(8 - row);
        TextOutA(hdc, MARGIN - 25, MARGIN + row * CELL + CELL / 2 - 8, numero.c_str(), (int)numero.size());
    }

    // Dibujar el alfil en su posicion actual
    dibujarAlfil(hdc, alfilRow, alfilCol);

    // Texto de instrucciones
    std::string instrucciones = "Haz clic en una casilla verde (diagonal) para mover el alfil";
    TextOutA(hdc, MARGIN, MARGIN + BOARD_SIZE + 25, instrucciones.c_str(), (int)instrucciones.size());
}

// ===================== VENTANA (WndProc) =====================
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Doble buffer para evitar parpadeo
            RECT rc;
            GetClientRect(hwnd, &rc);
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
            HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

            HBRUSH fondo = CreateSolidBrush(RGB(255, 255, 255));
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
                if (esMovimientoDiagonalValido(row, col)) {
                    alfilRow = row;
                    alfilCol = col;
                    InvalidateRect(hwnd, NULL, TRUE); // redibuja todo
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

// ===================== MAIN =====================
int main() {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    const char CLASS_NAME[] = "VentanaAlfil";

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
        "Movimiento del Alfil - Tablero de Ajedrez",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
        WIN_WIDTH + 16, WIN_HEIGHT + 39, // ajuste por bordes de ventana
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, SW_SHOW);

    printf("Programa iniciado. Se abrio la ventana grafica del tablero.\n");
    printf("Alfil en posicion inicial: fila %d, columna %d\n", alfilRow, alfilCol);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
