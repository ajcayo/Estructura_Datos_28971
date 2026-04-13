/***********************************************************************
 * Module:  Operacion.h
 * Author:  Janine
 * Modified: jueves, 9 de abril de 2026 07:09:19 p. m.
 * Purpose: Declaration of the class Quebrado
 ***********************************************************************/
#ifndef OPERACION_H
#define OPERACION_H

#include "Quebrado.h"

template <typename T>
class Operacion {
public:
    Quebrado<T> sumar(Quebrado<T> q1, Quebrado<T> q2);
    void mostrar(Quebrado<T> q);
};

#include "Operacion.cpp"

#endif