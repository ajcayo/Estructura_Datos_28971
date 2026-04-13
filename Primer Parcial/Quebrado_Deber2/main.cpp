/***********************************************************************
 * Module:  main.cpp
 * Author:  Janine
 * Modified: jueves, 9 de abril de 2026 07:09:19 p. m.
 * Purpose: Declaration of the class Quebrado
 ***********************************************************************/
#include <iostream>
#include "Quebrado.h"
#include "Operacion.h"

using namespace std;

int main() {
    float n, d;

    Quebrado<float> q1, q2, resultado;
    Operacion<float> operacion;

    cout << "--- Ingrese el primer quebrado ---" << endl;
    cout << "Numerador: ";
    cin >> n;
    q1.setNumerador(n);

    cout << "Denominador: ";
    cin >> d;
    q1.setDenominador(d);

    cout << "\n--- Ingrese el segundo quebrado ---" << endl;
    cout << "Numerador: ";
    cin >> n;
    q2.setNumerador(n);

    cout << "Denominador: ";
    cin >> d;
    q2.setDenominador(d);

    resultado = operacion.sumar(q1, q2);

    cout << "\nResultado: ";
    operacion.mostrar(q1);
    cout << " + ";
    operacion.mostrar(q2);
    cout << " = ";
    operacion.mostrar(resultado);

    cout << endl;

    return 0;
}