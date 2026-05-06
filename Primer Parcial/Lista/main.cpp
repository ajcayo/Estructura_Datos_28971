#include <iostream>
#include "Lista.h"
using namespace std;

int main() {
    Lista lista;
    int opcion;
    string cedula, nombre;

    do {
        cout << "\n--- MENU ---\n";
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
            lista.insertarCabeza(cedula, nombre);
            break;

        case 2: {
            cout << "Cedula a buscar: ";
            cin >> cedula;

            Nodo* encontrado = lista.buscar(cedula);

            if (encontrado != nullptr) {
                cout << "Encontrado: "
                     << encontrado->getCedula()
                     << " - "
                     << encontrado->getNombre() << endl;
            } else {
                cout << "No encontrado\n";
            }
            break;
        }

        case 3:
            cout << "Cedula a eliminar: ";
            cin >> cedula;
            lista.eliminar(cedula);
            break;

        case 4:
            cout << "\nLista:\n";
            lista.imprimir();
            break;
        }

    } while (opcion != 0);

    return 0;
}