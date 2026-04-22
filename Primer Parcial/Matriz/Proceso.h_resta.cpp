#ifndef PROCESO_H
#define PROCESO_H

#include "Matriz.h"

template <typename T>
class Proceso {
public:
    Matriz<T> restar(const Matriz<T>& m1, const Matriz<T>& m2);
    void mostrar(const Matriz<T>& m);
};

#endif