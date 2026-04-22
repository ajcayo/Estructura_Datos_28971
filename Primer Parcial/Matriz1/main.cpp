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

    m1.encerar();
    m2.encerar();

    cout << "\n--- Matriz 1 ---\n";
    m1.ingresar();

    cout << "\n--- Matriz 2 ---\n";
    m2.ingresar();

    Proceso<int> p;

    Matriz<int> r = p.sumar(m1, m2);

    cout << "\n--- Resultado ---\n";
    p.imprimir(r);

    return 0;
}
