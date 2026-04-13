/***********************************************************************
 * Module:  Quebrado.cpp
 * Author:  Janine
 * Modified: jueves, 9 de abril de 2026 07:09:19 p. m.
 * Purpose: Declaration of the class Quebrado
 ***********************************************************************/
 
#include "Quebrado.h"

Quebrado::Quebrado() {
    num = 0;
    den = 1;
}

Quebrado::Quebrado(float n, float d) {
    num = n;
    den = d;
}

void Quebrado::setNumerador(float n) {
    num = n;
}

void Quebrado::setDenominador(float d) {
    den = d;
}

float Quebrado::getNumerador() {
    return num;
}

float Quebrado::getDenominador() {
    return den;
}