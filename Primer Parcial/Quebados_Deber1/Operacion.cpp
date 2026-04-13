/***********************************************************************
 * Module:  Operacion.cpp
 * Author:  Janine
 * Modified: jueves, 9 de abril de 2026 07:09:19 p. m.
 * Purpose: Declaration of the class Quebrado
 ***********************************************************************/
 #include "Operacion.h"
#include <iostream>
using namespace std;

Quebrado Operacion::sumar(Quebrado q1, Quebrado q2) {
    
    float num = q1.getNumerador() * q2.getDenominador() + 
                q2.getNumerador() * q1.getDenominador();

    float den = q1.getDenominador() * q2.getDenominador();

    Quebrado resultado;
    resultado.setNumerador(num);
    resultado.setDenominador(den);

    return resultado;
}

void Operacion::mostrar(Quebrado q) {
    cout << q.getNumerador() << "/" << q.getDenominador() << endl;
}