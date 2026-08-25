/*
======================================================================================
  PROYECTO: SIMULACIÓN DE LABERINTO MEDIANTE BÚSQUEDA EXHAUSTIVA
======================================================================================
  DESCRIPCIÓN:
  Este programa resuelve un laberinto bidimensional utilizando el enfoque de 
  Búsqueda Exhaustiva (Fuerza Bruta) implementado de forma recursiva. El algoritmo
  explora sistemáticamente todas las rutas posibles (Arriba, Abajo, Izquierda, 
  Derecha) desde el punto de inicio ('E') hasta alcanzar la salida ('S').
  
  Si el camino actual choca con un obstáculo o un callejón sin salida, el sistema 
  aplica 'Backtracking' (retroceso), desmarcando la ruta fallida en tiempo real 
  para probar alternativas pendientes hasta agotar el árbol de decisiones.

  RESTRICCIONES TÉCNICAS REQUERIDAS:
  - Manipulación y acceso a matrices dinámicas mediante ARITMÉTICA DE PUNTEROS PURA
    utilizando la convención *(*(matriz + f) + c) sin el uso de corchetes [].
  - Gestión explícita y manual de memoria dinámica (new / delete[]).
  
  INTERFAZ VISUAL:
  - Animación interactiva en consola que refresca el mapa en cada paso (system("cls")).
  - Codificación UTF-8 habilitada para la representación gráfica limpia de paredes 
    (■), caminos libres (·) y el rastro activo del explorador (○).
======================================================================================
*/
#include <iostream>
#include <cstdlib>   // Para system("cls")
#include <windows.h> // Para Sleep() y configuracion de consola

using namespace std;

const int FILAS = 5;
const int COLUMNAS = 5;

// Función para redibujar el laberinto traducido a gráficos limpios
void mostrarLaberintoAnimado(char** laberinto) {
    system("cls"); 
    
    cout << "=========================" << endl;
    cout << "   SIMULACION EN VIVO    " << endl;
    cout << "=========================" << endl;
    
    for (int i = 0; i < FILAS; i++) {
        cout << "  "; 
        for (int j = 0; j < COLUMNAS; j++) {
            char celda = *(*(laberinto + i) + j);
            
            // Renderizado con símbolos legibles
            if (celda == 'X') {
                cout << "■  "; // Pared sólida
            } else if (celda == '0') {
                cout << "·  "; // Camino libre
            } else if (celda == '*') {
                cout << "○  "; // Rastro del explorador
            } else if (celda == 'E') {
                cout << "E  "; // Entrada
            } else if (celda == 'S') {
                cout << "S  "; // Salida
            }
        }
        cout << endl;
    }
    cout << "=========================" << endl;
    cout << " Leyenda: E=Inicio | S=Fin | ■=Pared | ·=Vacio | ○=Ruta" << endl;
    
    Sleep(500); 
}

// Función exhaustiva recursiva con punteros puros
bool resolverLaberintoAnimado(char** laberinto, int f, int c, int** visitado) {
    if (f < 0 || f >= FILAS || c < 0 || c >= COLUMNAS) {
        return false;
    }

    if (*(*(laberinto + f) + c) == 'X' || *(*(visitado + f) + c) == 1) {
        return false;
    }

    if (*(*(laberinto + f) + c) == 'S') {
        return true;
    }

    char caracterOriginal = *(*(laberinto + f) + c);

    if (caracterOriginal != 'E') {
        *(*(laberinto + f) + c) = '*'; 
    }
    
    *(*(visitado + f) + c) = 1;

    mostrarLaberintoAnimado(laberinto);

    // Búsqueda exhaustiva en las 4 direcciones
    if (resolverLaberintoAnimado(laberinto, f + 1, c, visitado) ||
        resolverLaberintoAnimado(laberinto, f - 1, c, visitado) ||
        resolverLaberintoAnimado(laberinto, f, c + 1, visitado) ||
        resolverLaberintoAnimado(laberinto, f, c - 1, visitado)) {
        return true;
    }

    // --- Backtracking ---
    if (caracterOriginal != 'E') {
        *(*(laberinto + f) + c) = caracterOriginal; 
    }
    *(*(visitado + f) + c) = 0;

    mostrarLaberintoAnimado(laberinto);

    return false;
}

int main() {
    // ESTA LÍNEA SOLUCIONA EL GLITCH VISUAL: Fuerza a la consola a usar UTF-8
    SetConsoleOutputCP(65001);

    // Alojar matrices dinámicas con aritmética de punteros
    char** laberinto = new char*[FILAS];
    for (int i = 0; i < FILAS; i++) {
        *(laberinto + i) = new char[COLUMNAS];
    }

    char mapa[FILAS][COLUMNAS] = {
        {'E', '0', 'X', 'X', 'X'},
        {'X', '0', '0', 'X', 'X'},
        {'X', 'X', '0', '0', 'X'},
        {'X', '0', 'X', '0', 'X'},
        {'X', 'X', 'X', '0', 'S'}
    };

    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            *(*(laberinto + i) + j) = mapa[i][j];
        }
    }

    int** visitado = new int*[FILAS];
    for (int i = 0; i < FILAS; i++) {
        *(visitado + i) = new int[COLUMNAS];
        for (int j = 0; j < COLUMNAS; j++) {
            *(*(visitado + i) + j) = 0;
        }
    }

    mostrarLaberintoAnimado(laberinto);

    if (resolverLaberintoAnimado(laberinto, 0, 0, visitado)) {
        cout << "\n[OK] ¡La busqueda exhaustiva encontro el camino correcto!" << endl;
    } else {
        cout << "\n[FAIL] No se encontro solucion." << endl;
    }

    // Liberación de memoria manual
    for (int i = 0; i < FILAS; i++) {
        delete[] *(laberinto + i);
        delete[] *(visitado + i);
    }
    delete[] laberinto;
    delete[] visitado;

    return 0;
}






