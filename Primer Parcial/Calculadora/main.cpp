#include <iostream>
#include "Calculadora.h"

using namespace std;

int main() {

    Calculadora<int> calcInt;

    cout << "INT:" << endl;
    cout << "Suma: " << calcInt.suma(5, 3) << endl;
    cout << "Resta: " << calcInt.resta(5, 3) << endl;
    cout << "Multiplicacion: " << calcInt.multiplicacion(5, 3) << endl;
    cout << "Division: " << calcInt.division(6, 3) << endl;

    cout << endl;


    Calculadora<float> calcFloat;

    cout << "FLOAT:" << endl;
    cout << "Suma: " << calcFloat.suma(5.5, 2.2) << endl;
    cout << "Resta: " << calcFloat.resta(5.5, 2.2) << endl;
    cout << "Multiplicacion: " << calcFloat.multiplicacion(5.5, 2.2) << endl;
    cout << "Division: " << calcFloat.division(5.5, 2.2) << endl;

    return 0;
}