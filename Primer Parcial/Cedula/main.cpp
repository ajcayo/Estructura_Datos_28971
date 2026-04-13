#include <iostream>
#include "ValidadorCedula.h"
using namespace std;

int main() {
    string ced;

    cout << "Ingrese la cedula: ";
    cin >> ced;

    ValidadorCedula<string> validador(ced);

    validador.mostrarResultado();

    return 0;
} 