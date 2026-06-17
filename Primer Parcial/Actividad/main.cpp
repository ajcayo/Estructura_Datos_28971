#include <iostream>
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
        cout << "0 Salir\n";
        cin >> op;

        switch(op) {

        case 1:
        	cout << "Ingrese datos" << endl;
            cin >> c >> n >> a;
            if (lista.esCedulaValida(c) && !lista.existeCedula(c))
                lista.insertarCabeza(c,n,a);
            else
                cout << "Error\n";
            break;

        case 2:
            cin >> c >> n >> a;
            lista.insertarFinal(c,n,a);
            break;

        case 3:
            cin >> ref >> c >> n >> a;
            lista.insertarEntre(ref,c,n,a);
            break;

        case 4: {
            cin >> c;
            Nodo* x = lista.buscar(c);
            if (x)
                cout << x->getNombre() << " " << x->getApellido() << endl;
            else
                cout << "No existe\n";
            break;
        }

        case 5:
            cin >> c;
            lista.eliminar(c);
            break;

        case 6:
            lista.imprimir();
            break;

        case 7:
            lista.guardarDatos("datos.txt");
            break;

        case 8:
            lista.generarCorreos();
            break;
        }

    } while (op != 0);

    return 0;
}