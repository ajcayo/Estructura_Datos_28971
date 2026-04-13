/***********************************************************************
 * Module:  Quebrado.cpp
 * Author:  Janine
 * Modified: jueves, 9 de abril de 2026 07:09:19 p. m.
 * Purpose: Declaration of the class Quebrado
 ***********************************************************************/

#include "Quebrado.h"

template <typename T>
Quebrado<T>::Quebrado() {
    numerador = 0;
    denominador = 1;
}

template <typename T>
Quebrado<T>::Quebrado(T n, T d) {
    numerador = n;
    denominador = d;
}

template <typename T>
void Quebrado<T>::setNumerador(T n) {
    numerador = n;
}

template <typename T>
void Quebrado<T>::setDenominador(T d) {
    denominador = d;
}

template <typename T>
T Quebrado<T>::getNumerador() {
    return numerador;
}

template <typename T>
T Quebrado<T>::getDenominador() {
    return denominador;
}