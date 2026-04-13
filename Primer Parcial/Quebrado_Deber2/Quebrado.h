/***********************************************************************
 * Module:  Quebrado.h
 * Author:  Janine
 * Modified: jueves, 9 de abril de 2026 07:09:19 p. m.
 * Purpose: Declaration of the class Quebrado
 ***********************************************************************/
 
#ifndef QUEBRADO_H
#define QUEBRADO_H

template <typename T>
class Quebrado {
private:
    T numerador;
    T denominador;

public:
    Quebrado();
    Quebrado(T n, T d);

    void setNumerador(T n);
    void setDenominador(T d);

    T getNumerador();
    T getDenominador();
};

#include "Quebrado.cpp" 

#endif