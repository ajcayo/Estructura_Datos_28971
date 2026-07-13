#include <windows.h>
#include <shellapi.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// ===================== VARIABLES GLOBALES =====================
int dimension = 0;
vector<vector<int>> tablero;

const int TAM_CASILLA = 60;
const int MARGEN = 20;

// ===================== LEER ARCHIVO TXT =====================
bool leerArchivo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Error: no se pudo abrir el archivo " << nombreArchivo << endl;
        return false;
    }

    tablero.clear();
    string linea;
    while (getline(archivo, linea)) {
        vector<int> fila;
        for (char c : linea) {
            if (c == '0' || c == '1') {
                fila.push_back(c - '0');
            }
        }
        if (!fila.empty()) {
            tablero.push_back(fila);
        }
    }
    archivo.close();

    if (tablero.empty()) {
        cout << "Error: el archivo esta vacio o mal formado." << endl;
        return false;
    }

    dimension = (int)tablero.size();
    return true;
}

// ===================== GENERAR ARCHIVO TXT =====================
void generarArchivo(const string& nombreArchivo, int dim) {
    ofstream archivo(nombreArchivo);
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            int valor = (i + j) % 2; // patron tipico de ajedrez (alterna 0 y 1)
            archivo << valor;
            if (j < dim - 1) archivo << " ";
        }
        archivo << "\n";
    }
    archivo.close();
    cout << "Archivo '" << nombreArchivo << "' generado correctamente con patron " << dim << "x" << dim << "." << endl;
}

// ===================== VENTANA GRAFICA (WIN32) =====================
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // 0 = blanco, 1 = negro (segun lo pedido)
            HBRUSH brushBlanco = CreateSolidBrush(RGB(240, 217, 181));
            HBRUSH brushNegro  = CreateSolidBrush(RGB(101, 67, 33));
            HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
            HGDIOBJ oldPen = SelectObject(hdc, pen);

            for (int i = 0; i < dimension; i++) {
                for (int j = 0; j < dimension; j++) {
                    int x = MARGEN + j * TAM_CASILLA;
                    int y = MARGEN + i * TAM_CASILLA;

                    HGDIOBJ oldBrush;
                    if (tablero[i][j] == 0) {
                        oldBrush = SelectObject(hdc, brushBlanco);
                    } else {
                        oldBrush = SelectObject(hdc, brushNegro);
                    }

                    Rectangle(hdc, x, y, x + TAM_CASILLA, y + TAM_CASILLA);
                    SelectObject(hdc, oldBrush);
                }
            }

            SelectObject(hdc, oldPen);
            DeleteObject(brushBlanco);
            DeleteObject(brushNegro);
            DeleteObject(pen);

            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void mostrarVentana() {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    const char CLASS_NAME[] = "VentanaTableroAjedrez";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    int anchoVentana = dimension * TAM_CASILLA + MARGEN * 2 + 16;
    int altoVentana  = dimension * TAM_CASILLA + MARGEN * 2 + 39;

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Tablero de Ajedrez",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
        anchoVentana, altoVentana,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) {
        cout << "Error al crear la ventana." << endl;
        return;
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// ===================== MAIN =====================
int main() {
    int dim;
    string nombreArchivo = "tablero.txt";

    cout << "=== GENERADOR DE TABLERO DE AJEDREZ ===" << endl;
    cout << "Ingrese la dimension del tablero (maximo 8): ";
    cin >> dim;

    if (dim < 1 || dim > 8) {
        cout << "Dimension invalida. Debe ser un numero entre 1 y 8." << endl;
        return 1;
    }

    // 1. Generar el archivo .txt con ceros y unos
    generarArchivo(nombreArchivo, dim);

    // 2. Abrir el archivo en el editor predeterminado de Windows
    cout << "\nAbriendo el archivo en el editor predeterminado..." << endl;
    ShellExecuteA(NULL, "open", nombreArchivo.c_str(), NULL, NULL, SW_SHOWNORMAL);

    cout << "Puedes editar el archivo (0 = blanco, 1 = negro) y guardarlo." << endl;
    cout << "Presiona ENTER en esta consola cuando quieras generar el tablero grafico...";
    cin.ignore();
    cin.get();

    // 3. Leer el archivo (ya editado o no) y generar el tablero grafico
    if (!leerArchivo(nombreArchivo)) {
        return 1;
    }

    cout << "Generando tablero grafico..." << endl;
    mostrarVentana();

    return 0;
}