/***********************************************************************
 * Module:  Operacion.h
 * Author:  Janine
 * Modified: jueves, 9 de abril de 2026 07:09:19 p. m.
 * Purpose: Declaration of the class Quebrado
 ***********************************************************************/
#ifndef OPERACION_H
#define OPERACION_H

#include "Quebrado.h"

class Operacion {
public:
    Quebrado sumar(Quebrado q1, Quebrado q2);
    void mostrar(Quebrado q);
};

#endif