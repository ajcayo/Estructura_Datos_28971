#include <iostream>
#include "Lista.h"
using namespace std;

int main() {
    Lista lista;
    int op;
    string c, n, ref;

    lista.cargarDatos("datos.txt");
    string* provincias = lista.cargarProvincias("provincias.txt");

    //LAMBDA DE VALIDACION
    auto validar = [&](string c, string n) {
        return lista.esCedulaValida(c) &&
               !lista.existeCedula(c) &&
               lista.esNombreValido(n);
    };

    do {
        cout << "\n--- MENU ---\n";
        cout << "1 Insertar inicio\n";
        cout << "2 Insertar final\n";
        cout << "3 Insertar entre\n";
        cout << "4 Buscar\n";
        cout << "5 Eliminar\n";
        cout << "6 Imprimir\n";
        cout << "7 Guardar\n";
        cout << "8 Reporte provincias\n";
        cout << "0 Salir\n";
        cin >> op;

        switch(op) {

        case 1:
            cin >> c >> n;
            if (validar(c,n))
                lista.insertarCabeza(c,n);
            else
                cout << "Error\n";
            break;

        case 2:
            cin >> c >> n;
            lista.insertarFinal(c,n);
            break;

        case 3:
            cin >> ref >> c >> n;
            lista.insertarEntre(ref,c,n);
            break;

        case 4: {
            cin >> c;
            Nodo* x = lista.buscar(c);
            if (x) {
                cout << x->getNombre() << endl;
                cout << "Vocales: " << lista.contarVocales(x->getNombre()) << endl;
                cout << "Letras: " << lista.contarLetras(x->getNombre()) << endl;
            } else cout << "No existe\n";
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
            lista.reporteProvincias(provincias);
            break;
        }

    } while (op != 0);

    return 0;
}