#include <iostream>
#include "Vector.h"

using namespace std;

int main() {
    Vector<int> v;

    v.agregar(10);
    v.agregar(20);
    v.agregar(30);

    cout << "Elementos del vector:" << endl;
    for (int i = 0; i < v.tamanio(); i++) {
        cout << v.obtener(i) << endl;
    }

    v.eliminar();

    cout << "\nDespues de eliminar:" << endl;
    for (int i = 0; i < v.tamanio(); i++) {
        cout << v.obtener(i) << endl;
    }

    return 0;
}