#include <iostream>
#include "ListaVehiculos.h"

using namespace std;

int main() {
    ListaVehiculos lista;
    int op;

    do {
        cout << "\n1. Insertar vehiculo\n";
        cout << "2. Eliminar vehiculo\n";
        cout << "3. Buscar vehiculo\n";
        cout << "4. Imprimir lista\n";
        cout << "5. Reservar vehiculo\n";
        cout << "0. Salir\n";
        cout << "Opcion: ";
        cin >> op;

        if (op == 1) {
            string placa, modelo;
            cout << "Placa: ";
            cin >> placa;
            cout << "Modelo: ";
            cin >> modelo;

            lista.insertar(placa, modelo);
        }

        if (op == 2) {
            string placa;
            cout << "Placa a eliminar: ";
            cin >> placa;
            lista.eliminar(placa);
        }

        if (op == 3) {
            string placa;
            cout << "Placa a buscar: ";
            cin >> placa;

            Vehiculo* v = lista.buscar(placa);

            if (v != NULL) {
                cout << "Encontrado: " << v->getModelo() << endl;
            } else {
                cout << "No existe\n";
            }
        }

        if (op == 4) {
            lista.imprimir();
        }

        if (op == 5) {
            string placa, cedula, nombre;

            cout << "Placa: ";
            cin >> placa;
            cout << "Cedula: ";
            cin >> cedula;
            cout << "Nombre: ";
            cin >> nombre;

            Persona p(cedula, nombre);
            lista.reservarVehiculo(placa, p);
        }

    } while (op != 0);

    return 0;
}