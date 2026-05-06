#include <iostream>
#include "ListaD.h"
using namespace std;

int main() {
    Lista lista;
    int opcion;
    string cedula, nombre;

    do {
        cout << "\n--- MENU LISTA DOBLE ---\n";
        cout << "1. Insertar en cabeza\n";
        cout << "2. Insertar en cola\n";
        cout << "3. Buscar\n";
        cout << "4. Eliminar\n";
        cout << "5. Imprimir\n";
        cout << "6. Imprimir reversa\n";
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

        case 2:
            cout << "Cedula: ";
            cin >> cedula;
            cout << "Nombre: ";
            cin >> nombre;
            lista.insertarCola(cedula, nombre);
            break;

        case 3: {
            cout << "Cedula a buscar: ";
            cin >> cedula;
            Nodo* e = lista.buscar(cedula);

            if (e != nullptr) {
                cout << "Encontrado: "
                     << e->getCedula()
                     << " - "
                     << e->getNombre() << endl;
            } else {
                cout << "No encontrado\n";
            }
            break;
        }

        case 4:
            cout << "Cedula a eliminar: ";
            cin >> cedula;
            lista.eliminar(cedula);
            break;

        case 5:
            cout << "\nLista:\n";
            lista.imprimir();
            break;

        case 6:
            cout << "\nLista en reversa:\n";
            lista.imprimirReversa();
            break;
        }

    } while (opcion != 0);

    return 0;
}