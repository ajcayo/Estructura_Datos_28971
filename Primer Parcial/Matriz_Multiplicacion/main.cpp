#include <iostream>
#include "Matriz.h"
#include "Proceso.h"

using namespace std;

int main() {
    int f1, c1, f2, c2, p;

    cout << "Filas A: "; cin >> f1;
    cout << "Columnas A: "; cin >> c1;

    cout << "Filas B: "; cin >> f2;
    cout << "Columnas B: "; cin >> c2;

    cout << "Profundidad: "; cin >> p;

    if(c1 != f2){
        cout << "No se puede multiplicar\n";
        return 0;
    }

    Matriz<int> A(f1,c1,p);
    Matriz<int> B(f2,c2,p);

    cout << "\n--- Matriz A ---\n";
    A.ingresar();

    cout << "\n--- Matriz B ---\n";
    B.ingresar();

    Proceso<int> proceso;
    Matriz<int> R = proceso.multiplicar(A,B);

    cout << "\n--- Resultado ---\n";
    proceso.mostrar(R);

    return 0;
}