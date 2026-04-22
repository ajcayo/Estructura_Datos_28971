#include <iostream>
#include "Matriz.h"
#include "Proceso.h"

using namespace std;

int main() {
    int f, c, p;

    cout << "Filas: ";
    cin >> f;
    cout << "Columnas: ";
    cin >> c;
    cout << "Profundidad: ";
    cin >> p;

    Matriz<int> m1(f, c, p);
    Matriz<int> m2(f, c, p);

    cout << "\n--- Matriz 1 ---\n";
    m1.ingresar();

    cout << "\n--- Matriz 2 ---\n";
    m2.ingresar();

    Proceso<int> proceso;

    Matriz<int> resta = proceso.restar(m1, m2);

    cout << "\n--- Resultado (Resta) ---\n";
    proceso.mostrar(resta);

    return 0;
}