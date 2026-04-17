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

    Quebrado<float> q1(1, 2);
    Quebrado<float> q2(3, 4);

    Operacion<float> op;


    Quebrado<float> r0 = q1 + q2;


    Quebrado<float> r2;
    op.sumar(q1, q2, r2);


    Quebrado<float> r3;
    op.sumar(q1, q2, &r3);

    cout << "Operador +: ";
    op.mostrar(r0);

    cout << "\nReferencia: ";
    op.mostrar(r2);

    cout << "\nPuntero: ";
    op.mostrar(r3);

    cout << endl;

    return 0;
}