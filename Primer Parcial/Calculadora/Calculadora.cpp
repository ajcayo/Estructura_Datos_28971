#include "Calculadora.h"

template <typename T>
T Calculadora<T>::suma(T a, T b) {
    return a + b;
}


template <typename T>
T Calculadora<T>::resta(T a, T b) {
    return a - b;
}


template <typename T>
T Calculadora<T>::multiplicacion(T a, T b) {
    return a * b;
}


template <typename T>
T Calculadora<T>::division(T a, T b) {
    return a / b;
}