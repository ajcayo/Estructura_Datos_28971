#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    // 1. Abrir el archivo generado por el primer programa
    ifstream archivo("tablero.txt");
    if (!archivo.is_open()) {
        cout << "Error: No se encontro 'tablero.txt'." << endl;
        cout << "Por favor, ejecuta el programa generador primero." << endl;
        return 1;
    }

    // 2. Leer todos los ceros y unos del archivo
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

    // 3. Calcular la dimensión de la matriz (raíz cuadrada de los datos totales)
    int n = 0;
    while (n * n < datos.size()) n++;

    cout << "--- MODO GRAFICO ---" << endl;
    cout << "Dibujando tablero de " << n << "x" << n << "..." << endl;
    cout << "\nNOTA: No muevas ni redimensiones la ventana, o el dibujo se borrara." << endl;
    cout << "Presiona ENTER en esta consola para salir..." << endl;

    // 4. Configuración Gráfica usando windows.h
    HWND console = GetConsoleWindow(); // Obtener la ventana de la consola actual
    HDC hdc = GetDC(console);          // Obtener el contexto de dispositivo para dibujar

    int tamanoCelda = 50; // Tamaño de cada cuadro en píxeles
    int offsetX = 50;     // Margen izquierdo
    int offsetY = 120;    // Margen superior (para no sobreescribir el texto de arriba)

    // Crear "brochas" de colores para pintar
    HBRUSH brochaBlanca = CreateSolidBrush(RGB(255, 255, 255));
    HBRUSH brochaNegra = CreateSolidBrush(RGB(50, 50, 50)); // Un gris muy oscuro para mejor contraste

    // 5. Dibujar el tablero leyendo el vector de datos
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int index = i * n + j;
            if (index >= datos.size()) break;

            // Calcular las coordenadas del rectángulo
            RECT rect;
            rect.left = offsetX + (j * tamanoCelda);
            rect.top = offsetY + (i * tamanoCelda);
            rect.right = rect.left + tamanoCelda;
            rect.bottom = rect.top + tamanoCelda;

            // Rellenar el rectángulo dependiendo de si es 0 (Blanco) o 1 (Negro)
            if (datos[index] == 0) {
                FillRect(hdc, &rect, brochaBlanca);
            } else {
                FillRect(hdc, &rect, brochaNegra);
            }
        }
    }

    // 6. Limpieza de memoria gráfica
    DeleteObject(brochaBlanca);
    DeleteObject(brochaNegra);
    ReleaseDC(console, hdc);

    // Pausar el programa para que el usuario pueda ver el dibujo
    cin.ignore();
    cin.get();

    return 0;
}