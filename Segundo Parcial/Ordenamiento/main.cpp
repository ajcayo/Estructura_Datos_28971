#include <iostream>
#include <limits>
#include "Lista.h"
using namespace std;

int main() {
    Lista lista;
    int op;
    string c, n, a, ref;

    lista.cargarDatos("datos.txt");

    do {
        cout << "\n--- MENU ---\n";
        cout << "1 Insertar inicio\n";
        cout << "2 Insertar final\n";
        cout << "3 Insertar entre\n";
        cout << "4 Buscar\n";
        cout << "5 Eliminar\n";
        cout << "6 Imprimir\n";
        cout << "7 Guardar\n";
        cout << "8 Generar correos\n";
        cout << "9 Ordenar burbuja (animado)\n";
        cout << "0 Salir\n";
        cout << "Opcion: ";
        cin >> op;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch(op) {

        case 1:
            cout << "Cedula: ";
            cin >> c;
            cin.ignore();

            cout << "Nombre: ";
            getline(cin, n);

            cout << "Apellido: ";
            getline(cin, a);

            if (lista.esCedulaValida(c) && !lista.existeCedula(c))
                lista.insertarCabeza(c, n, a);
            else
                cout << "Error\n";
            break;

        case 2:
            cout << "Cedula: ";
            cin >> c;
            cin.ignore();

            cout << "Nombre: ";
            getline(cin, n);

            cout << "Apellido: ";
            getline(cin, a);

            lista.insertarFinal(c, n, a);
            break;

        case 3:
            cout << "Cedula referencia: ";
            cin >> ref;

            cout << "Nueva cedula: ";
            cin >> c;
            cin.ignore();

            cout << "Nombre: ";
            getline(cin, n);

            cout << "Apellido: ";
            getline(cin, a);

            lista.insertarEntre(ref, c, n, a);
            break;

        case 4:
            cout << "Cedula a buscar: ";
            cin >> c;
            {
                Nodo* x = lista.buscar(c);
                if (x)
                    cout << x->getNombre() << " " << x->getApellido() << endl;
                else
                    cout << "No existe\n";
            }
            break;

        case 5:
            cout << "Cedula a eliminar: ";
            cin >> c;
            lista.eliminar(c);
            break;

        case 6:
            lista.imprimir();
            break;

        case 7:
            lista.guardarDatos("datos.txt");
            cout << "Datos guardados\n";
            break;

        case 8:
            lista.generarCorreos();
            break;

        case 9:
            lista.ordenamientoburbuja();
            break;
        }

    } while (op != 0);

    return 0;
}
