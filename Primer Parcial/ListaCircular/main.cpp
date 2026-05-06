#include <iostream>
#include "ListaCircular.h"
using namespace std;

int main() {
    ListaCircular lista;
    int opcion;
    string cedula, nombre;

    do {
        cout << "\n--- LISTA CIRCULAR ---\n";
        cout << "1. Insertar\n";
        cout << "2. Buscar\n";
        cout << "3. Eliminar\n";
        cout << "4. Imprimir\n";
        cout << "0. Salir\n";
        cout << "Opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            cout << "Cedula: ";
            cin >> cedula;
            cout << "Nombre: ";
            cin >> nombre;
            lista.insertar(cedula, nombre);
            break;

        case 2: {
            cout << "Cedula: ";
            cin >> cedula;
            Nodo* e = lista.buscar(cedula);

            if (e != nullptr)
                cout << "Encontrado: " << e->getNombre() << endl;
            else
                cout << "No encontrado\n";
            break;
        }

        case 3:
            cout << "Cedula: ";
            cin >> cedula;
            lista.eliminar(cedula);
            break;

        case 4:
            lista.imprimir();
            break;
        }

    } while (opcion != 0);

    return 0;
}