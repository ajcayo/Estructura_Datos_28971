#include <iostream>
#include <fstream>

using namespace std;

int main() {
    int n;
    
    cout << "--- GENERADOR DE TABLERO ---" << endl;
    cout << "Ingrese el tamano de la matriz (1 a 8): ";
    cin >> n;

    // Validación
    if (n < 1 || n > 8) {
        cout << "Error: El numero debe estar entre 1 y 8." << endl;
        return 1;
    }

    // Crear y abrir el archivo
    ofstream archivo("tablero.txt");
    if (!archivo.is_open()) {
        cout << "Error al crear el archivo 'tablero.txt'." << endl;
        return 1;
    }

    // Generar la matriz NxN con ceros y unos intercalados
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // La fórmula (i + j) % 2 crea la alternancia perfecta 0 1 0 1...
            archivo << (i + j) % 2 << " ";
        }
        archivo << endl; // Salto de línea por cada fila
    }

    cout << "Exito: Archivo 'tablero.txt' generado correctamente (Matriz " << n << "x" << n << ")." << endl;
    
    archivo.close();
    return 0;
}