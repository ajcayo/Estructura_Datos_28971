#ifndef PROCESO_H
#define PROCESO_H

#include "Matriz.h"

template <typename T>
class Proceso {
public:
    Matriz<T> sumar(Matriz<T>& m1, Matriz<T>& m2) {
        return m1 + m2;
    }

    void imprimir(Matriz<T>& m) {
        m.imprimir();
    }
};

#endif


