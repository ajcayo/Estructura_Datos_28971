#ifndef VECTOR_H
#define VECTOR_H

template <typename T>
class Vector {
private:
    T* datos;
    int capacidad;
    int cantidad;

    void redimensionar();

public:
    Vector();
    ~Vector();

    void agregar(T dato);
    void eliminar();
    T obtener(int index);
    int tamanio();
};

#include "Vector.cpp"

#endif