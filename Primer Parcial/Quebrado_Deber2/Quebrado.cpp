/***********************************************************************
 * Module:  Quebrado.cpp
 * Author:  Janine
 * Modified: jueves, 9 de abril de 2026 07:09:19 p. m.
 * Purpose: Declaration of the class Quebrado
 ***********************************************************************/
#ifndef OPERACION_H
#define OPERACION_H

#include <iostream>
#include "Quebrado.h"
using namespace std;

template <typename T>
class Operacion {
public:


    void sumar(const Quebrado<T>& q1, const Quebrado<T>& q2, Quebrado<T>& resultado);

    void sumar(const Quebrado<T>& q1, const Quebrado<T>& q2, Quebrado<T>* resultado);

    void mostrar(const Quebrado<T>& q);
};


template <typename T>
void Operacion<T>::sumar(const Quebrado<T>& q1, const Quebrado<T>& q2, Quebrado<T>& resultado) {

    resultado = q1 + q2;   
}


template <typename T>
void Operacion<T>::sumar(const Quebrado<T>& q1, const Quebrado<T>& q2, Quebrado<T>* resultado) {

    *resultado = q1 + q2;   
}


template <typename T>
void Operacion<T>::mostrar(const Quebrado<T>& q) {
    cout << q.getNumerador() << "/" << q.getDenominador();
}

#endif