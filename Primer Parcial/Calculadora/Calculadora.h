#ifndef CALCULADORA_H
#define CALCULADORA_H

template <typename T>
class Calculadora {
public:
    T suma(T a, T b);
    T resta(T a, T b);
    T multiplicacion(T a, T b);
    T division(T a, T b);
};

#include "Calculadora.cpp" 

#endif