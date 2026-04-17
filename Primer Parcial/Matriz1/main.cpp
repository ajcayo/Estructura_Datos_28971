#include <iostream>
#include "Matriz.h"
#include "Proceso.h"

using namespace std;

int main() {
    int f, c;

    cout << "Filas: ";
    cin >> f;
    cout << "Columnas: ";
    cin >> c;

    Matriz<int> m1(f, c);
    Matriz<int> m2(f, c);

    cout << "\n--- Matriz 1 ---" << endl;
    m1.ingresar();

    cout << "\n--- Matriz 2 ---" << endl;
    m2.ingresar();

    Proceso<int> proceso;
    // Ahora esto funcionará sin corromper la memoria
    Matriz<int> resultado = proceso.sumar(m1, m2);

    cout << "\n--- Resultado ---" << endl;
    proceso.mostrar(resultado);

    return 0;
}

