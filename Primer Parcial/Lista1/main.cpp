#include <iostream>
#include "Lista.h"
using namespace std;

int main() {
    Lista lista;

    lista.insertarCabeza("0101", "Juan");
    lista.insertarCabeza("0202", "Maria");
    lista.insertarCabeza("0303", "Pedro");

    cout << "Lista:" << endl;
    lista.imprimir();

    return 0;
}