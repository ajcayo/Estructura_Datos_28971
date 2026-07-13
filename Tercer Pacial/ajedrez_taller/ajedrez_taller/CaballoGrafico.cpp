#include <windows.h>
#include <iostream>
#include <fstream>

using namespace std;

// Tamano maximo del tablero (segun el Generador, de 1 a 8)
const int TAMANO_MAXIMO = 8;

// Tamano de la imagen del caballo (filas x columnas del archivo caballo.txt)
const int FILAS_CABALLO = 18;
const int COLUMNAS_CABALLO = 16;

// ==========================================================
//  FUNCION DE PIXELAR
//  Recibe la imagen del caballo ya cargada en un arreglo de
//  0 y 1, y la dibuja en la pantalla agrandando cada "pixel"
//  a un cuadro de tamano "escala". Con "escala" mas grande,
//  la imagen se ve mas pixelada (mas blocosa).
//  (x0, y0) es el punto exacto de la pantalla donde se
//  inserta la imagen.
// ==========================================================
void pixelarImagen(HDC hdc, int imagen[FILAS_CABALLO][COLUMNAS_CABALLO], int x0, int y0, int escala, COLORREF color) {
    HBRUSH brocha = CreateSolidBrush(color);

    for (int i = 0; i < FILAS_CABALLO; i++) {
        for (int j = 0; j < COLUMNAS_CABALLO; j++) {
            if (imagen[i][j] == 1) {
                RECT r;
                r.left   = x0 + j * escala;
                r.top    = y0 + i * escala;
                r.right  = r.left + escala;
                r.bottom = r.top + escala;
                FillRect(hdc, &r, brocha);
            }
        }
    }
    DeleteObject(brocha);
}

int main() {
    // ------------------------------------------------------
    // 1. Leer el tablero generado por Generador.cpp
    // ------------------------------------------------------
    ifstream archivoTablero("tablero.txt");
    if (!archivoTablero.is_open()) {
        cout << "Error: No se encontro 'tablero.txt'." << endl;
        cout << "Por favor, ejecuta el programa generador primero." << endl;
        return 1;
    }

    // Guardamos todos los numeros leidos uno por uno en un arreglo simple
    int numerosLeidos[TAMANO_MAXIMO * TAMANO_MAXIMO];
    int totalNumeros = 0;
    int valor;

    while (archivoTablero >> valor) {
        numerosLeidos[totalNumeros] = valor;
        totalNumeros++;
    }
    archivoTablero.close();

    if (totalNumeros == 0) {
        cout << "El archivo 'tablero.txt' esta vacio." << endl;
        return 1;
    }

    // Calculamos el tamano n del tablero (n x n = totalNumeros)
    int n = 0;
    while (n * n < totalNumeros) n++;

    // Pasamos los numeros leidos a una matriz tablero[n][n]
    int tablero[TAMANO_MAXIMO][TAMANO_MAXIMO];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            tablero[i][j] = numerosLeidos[i * n + j];
        }
    }

    // ------------------------------------------------------
    // 2. Leer la imagen del caballo desde caballo.txt
    // ------------------------------------------------------
    ifstream archivoCaballo("caballo.txt");
    if (!archivoCaballo.is_open()) {
        cout << "Error: No se encontro 'caballo.txt'." << endl;
        return 1;
    }

    int imagenCaballo[FILAS_CABALLO][COLUMNAS_CABALLO];
    for (int i = 0; i < FILAS_CABALLO; i++) {
        for (int j = 0; j < COLUMNAS_CABALLO; j++) {
            archivoCaballo >> imagenCaballo[i][j];
        }
    }
    archivoCaballo.close();

    // ------------------------------------------------------
    // 3. Pedir al usuario la posicion del caballo
    // ------------------------------------------------------
    int filaCaballo, columnaCaballo;
    cout << "--- CABALLO EN EL TABLERO (" << n << "x" << n << ") ---" << endl;
    cout << "Filas y columnas validas: 0 a " << (n - 1) << endl;
    cout << "Ingrese la fila donde colocar el caballo: ";
    cin >> filaCaballo;
    cout << "Ingrese la columna donde colocar el caballo: ";
    cin >> columnaCaballo;

    if (filaCaballo < 0 || filaCaballo >= n || columnaCaballo < 0 || columnaCaballo >= n) {
        cout << "Error: posicion fuera del tablero." << endl;
        return 1;
    }

    // ------------------------------------------------------
    // 4. Calcular los movimientos validos del caballo
    //    (los 8 posibles saltos en forma de "L" del ajedrez)
    // ------------------------------------------------------
    int desplazamientoFila[8];
    int desplazamientoColumna[8];

    desplazamientoFila[0] = 1;   desplazamientoColumna[0] = 2;
    desplazamientoFila[1] = 1;   desplazamientoColumna[1] = -2;
    desplazamientoFila[2] = -1;  desplazamientoColumna[2] = 2;
    desplazamientoFila[3] = -1;  desplazamientoColumna[3] = -2;
    desplazamientoFila[4] = 2;   desplazamientoColumna[4] = 1;
    desplazamientoFila[5] = 2;   desplazamientoColumna[5] = -1;
    desplazamientoFila[6] = -2;  desplazamientoColumna[6] = 1;
    desplazamientoFila[7] = -2;  desplazamientoColumna[7] = -1;

    // Arreglos donde guardamos las casillas de destino validas
    int destinoFila[8];
    int destinoColumna[8];
    int totalMovimientos = 0;

    for (int k = 0; k < 8; k++) {
        int nuevaFila = filaCaballo + desplazamientoFila[k];
        int nuevaColumna = columnaCaballo + desplazamientoColumna[k];

        // Solo se guarda si cae dentro del tablero
        if (nuevaFila >= 0 && nuevaFila < n && nuevaColumna >= 0 && nuevaColumna < n) {
            destinoFila[totalMovimientos] = nuevaFila;
            destinoColumna[totalMovimientos] = nuevaColumna;
            totalMovimientos++;
        }
    }

    cout << "\nEl caballo colocado en (" << filaCaballo << ", " << columnaCaballo << ") tiene "
         << totalMovimientos << " movimiento(s) posible(s):" << endl;
    for (int k = 0; k < totalMovimientos; k++) {
        cout << "  -> (" << destinoFila[k] << ", " << destinoColumna[k] << ")" << endl;
    }

    cout << "\n--- MODO GRAFICO ---" << endl;
    cout << "Dibujando tablero de " << n << "x" << n << " con el caballo..." << endl;
    cout << "NOTA: No muevas ni redimensiones la ventana, o el dibujo se borrara." << endl;
    cout << "Presiona ENTER en esta consola para salir..." << endl;

    // ------------------------------------------------------
    // 5. Configuracion grafica (igual que en Grafico.cpp)
    // ------------------------------------------------------
    HWND consola = GetConsoleWindow();
    HDC hdc = GetDC(consola);

    int tamanoCelda = 50;
    int offsetX = 50;
    int offsetY = 150;

    HBRUSH brochaBlanca    = CreateSolidBrush(RGB(255, 255, 255));
    HBRUSH brochaNegra     = CreateSolidBrush(RGB(50, 50, 50));
    HBRUSH brochaResaltado = CreateSolidBrush(RGB(120, 200, 120)); // verde para movimientos validos

    // Dibujar el tablero
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            RECT rect;
            rect.left   = offsetX + (j * tamanoCelda);
            rect.top    = offsetY + (i * tamanoCelda);
            rect.right  = rect.left + tamanoCelda;
            rect.bottom = rect.top + tamanoCelda;

            if (tablero[i][j] == 0) {
                FillRect(hdc, &rect, brochaBlanca);
            } else {
                FillRect(hdc, &rect, brochaNegra);
            }
        }
    }

    // ------------------------------------------------------
    // 6. Resaltar en verde las casillas donde el caballo
    //    puede moverse
    // ------------------------------------------------------
    for (int k = 0; k < totalMovimientos; k++) {
        RECT rect;
        rect.left   = offsetX + (destinoColumna[k] * tamanoCelda);
        rect.top    = offsetY + (destinoFila[k] * tamanoCelda);
        rect.right  = rect.left + tamanoCelda;
        rect.bottom = rect.top + tamanoCelda;
        FillRect(hdc, &rect, brochaResaltado);
    }

    // ------------------------------------------------------
    // 7. Insertar la imagen pixelada del caballo en el punto
    //    elegido (se centra dentro de su casilla)
    // ------------------------------------------------------
    int escalaPixel = 2; // tamano de cada "pixel" del dibujo -> se puede cambiar
    int anchoImagen = COLUMNAS_CABALLO * escalaPixel;
    int altoImagen  = FILAS_CABALLO * escalaPixel;

    int xCaballo = offsetX + (columnaCaballo * tamanoCelda) + (tamanoCelda - anchoImagen) / 2;
    int yCaballo = offsetY + (filaCaballo * tamanoCelda) + (tamanoCelda - altoImagen) / 2;

    COLORREF colorCaballo = RGB(150, 30, 30); // rojo oscuro para distinguirlo del tablero
    pixelarImagen(hdc, imagenCaballo, xCaballo, yCaballo, escalaPixel, colorCaballo);

    // ------------------------------------------------------
    // 8. Limpieza de memoria grafica
    // ------------------------------------------------------
    DeleteObject(brochaBlanca);
    DeleteObject(brochaNegra);
    DeleteObject(brochaResaltado);
    ReleaseDC(consola, hdc);

    cin.ignore();
    cin.get();

    return 0;
}
