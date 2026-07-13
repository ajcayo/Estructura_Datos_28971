#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// ==========================================================
//  IMAGEN DEL CABALLO (sprite en baja resolucion)
//  Cada caracter representa un "pixel logico":
//  '1' = se pinta,  '0' = se deja transparente
// ==========================================================
vector<string> spriteCaballo = {
    "0000000000000000",
    "0000000111000000",
    "0000001111100000",
    "0000011111110000",
    "0000111100111000",
    "0001111000011100",
    "0011110000001100",
    "0111100000000110",
    "0111000000000011",
    "0110000000000001",
    "0110000000000000",
    "0111000000000000",
    "0111100000000000",
    "0011111111111100",
    "0011111111111100",
    "0001111111111000",
    "0001111111111000",
    "0000000000000000"
};

// ==========================================================
//  FUNCION DE PIXELAR
//  Recorre una imagen logica (matriz de 0 y 1) y la dibuja
//  en la pantalla agrandando cada "pixel" a un bloque de
//  tamano "escala". Mientras mas grande sea "escala", mas
//  "pixelada" (blocosa) se ve la imagen resultante.
//  (x0, y0) es el punto exacto donde se inserta la imagen.
// ==========================================================
void pixelarImagen(HDC hdc, const vector<string>& imagen, int x0, int y0, int escala, COLORREF color) {
    HBRUSH brocha = CreateSolidBrush(color);

    for (size_t i = 0; i < imagen.size(); i++) {
        for (size_t j = 0; j < imagen[i].size(); j++) {
            if (imagen[i][j] == '1') {
                RECT r;
                r.left   = x0 + (int)j * escala;
                r.top    = y0 + (int)i * escala;
                r.right  = r.left + escala;
                r.bottom = r.top + escala;
                FillRect(hdc, &r, brocha);
            }
        }
    }
    DeleteObject(brocha);
}

// ==========================================================
//  MOVIMIENTOS DEL CABALLO
//  Dada una posicion (fila, columna) calcula todas las
//  casillas validas (dentro del tablero n x n) a las que
//  el caballo se puede mover, siguiendo el movimiento en
//  forma de "L" del ajedrez.
// ==========================================================
int calcularMovimientosCaballo(int fila, int columna, int n, vector<pair<int,int>>& destinos) {
    int dFila[8]    = { 1,  1, -1, -1,  2,  2, -2, -2 };
    int dColumna[8] = { 2, -2,  2, -2,  1, -1,  1, -1 };

    destinos.clear();
    for (int k = 0; k < 8; k++) {
        int nf = fila + dFila[k];
        int nc = columna + dColumna[k];
        if (nf >= 0 && nf < n && nc >= 0 && nc < n) {
            destinos.push_back({nf, nc});
        }
    }
    return (int)destinos.size();
}

int main() {
    // 1. Abrir el archivo generado por el programa Generador
    ifstream archivo("tablero.txt");
    if (!archivo.is_open()) {
        cout << "Error: No se encontro 'tablero.txt'." << endl;
        cout << "Por favor, ejecuta el programa generador primero." << endl;
        return 1;
    }

    vector<int> datos;
    int valor;
    while (archivo >> valor) {
        datos.push_back(valor);
    }
    archivo.close();

    if (datos.empty()) {
        cout << "El archivo esta vacio." << endl;
        return 1;
    }

    // 2. Calcular la dimension n del tablero
    int n = 0;
    while (n * n < (int)datos.size()) n++;

    // 3. Pedir al usuario la posicion del caballo
    int fila, columna;
    cout << "--- CABALLO EN EL TABLERO (" << n << "x" << n << ") ---" << endl;
    cout << "Filas y columnas validas: 0 a " << (n - 1) << endl;
    cout << "Ingrese la fila donde colocar el caballo: ";
    cin >> fila;
    cout << "Ingrese la columna donde colocar el caballo: ";
    cin >> columna;

    if (fila < 0 || fila >= n || columna < 0 || columna >= n) {
        cout << "Error: posicion fuera del tablero." << endl;
        return 1;
    }

    // 4. Calcular los movimientos validos del caballo
    vector<pair<int,int>> destinos;
    int totalMovimientos = calcularMovimientosCaballo(fila, columna, n, destinos);

    cout << "\nEl caballo colocado en (" << fila << ", " << columna << ") tiene "
         << totalMovimientos << " movimiento(s) posible(s):" << endl;
    for (auto &d : destinos) {
        cout << "  -> (" << d.first << ", " << d.second << ")" << endl;
    }

    cout << "\n--- MODO GRAFICO ---" << endl;
    cout << "Dibujando tablero de " << n << "x" << n << " con el caballo..." << endl;
    cout << "NOTA: No muevas ni redimensiones la ventana, o el dibujo se borrara." << endl;
    cout << "Presiona ENTER en esta consola para salir..." << endl;

    // 5. Configuracion grafica (igual que en Grafico.cpp)
    HWND console = GetConsoleWindow();
    HDC hdc = GetDC(console);

    int tamanoCelda = 50;
    int offsetX = 50;
    int offsetY = 150;

    HBRUSH brochaBlanca    = CreateSolidBrush(RGB(255, 255, 255));
    HBRUSH brochaNegra     = CreateSolidBrush(RGB(50, 50, 50));
    HBRUSH brochaResaltado = CreateSolidBrush(RGB(120, 200, 120)); // verde para movimientos validos

    // 6. Dibujar el tablero
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int index = i * n + j;
            if (index >= (int)datos.size()) break;

            RECT rect;
            rect.left   = offsetX + (j * tamanoCelda);
            rect.top    = offsetY + (i * tamanoCelda);
            rect.right  = rect.left + tamanoCelda;
            rect.bottom = rect.top + tamanoCelda;

            if (datos[index] == 0) {
                FillRect(hdc, &rect, brochaBlanca);
            } else {
                FillRect(hdc, &rect, brochaNegra);
            }
        }
    }

    // 7. Resaltar las casillas donde el caballo puede moverse
    for (auto &d : destinos) {
        RECT rect;
        rect.left   = offsetX + (d.second * tamanoCelda);
        rect.top    = offsetY + (d.first * tamanoCelda);
        rect.right  = rect.left + tamanoCelda;
        rect.bottom = rect.top + tamanoCelda;
        FillRect(hdc, &rect, brochaResaltado);
    }

    // 8. Insertar la imagen pixelada del caballo en el punto elegido
    //    Se centra el sprite dentro de la celda (fila, columna).
    int escalaPixel = 2; // tamano de cada "pixel" del sprite -> ajustable
    int anchoSprite  = (int)spriteCaballo[0].size() * escalaPixel;
    int altoSprite   = (int)spriteCaballo.size() * escalaPixel;

    int xCaballo = offsetX + (columna * tamanoCelda) + (tamanoCelda - anchoSprite) / 2;
    int yCaballo = offsetY + (fila * tamanoCelda) + (tamanoCelda - altoSprite) / 2;

    COLORREF colorCaballo = RGB(150, 30, 30); // rojo oscuro para que se distinga del tablero
    pixelarImagen(hdc, spriteCaballo, xCaballo, yCaballo, escalaPixel, colorCaballo);

    // 9. Limpieza de memoria grafica
    DeleteObject(brochaBlanca);
    DeleteObject(brochaNegra);
    DeleteObject(brochaResaltado);
    ReleaseDC(console, hdc);

    cin.ignore();
    cin.get();

    return 0;
}