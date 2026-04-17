#ifndef MATRIZ_H
#define MATRIZ_H

#include <iostream>
using namespace std;

template <typename T>
class Matriz {
private:
    T** datos;
    int filas;
    int columnas;

public:
    Matriz(int f, int c);
    ~Matriz();
    Matriz(const Matriz<T>& other);
    Matriz<T>& operator=(const Matriz<T>& other);

    void ingresar();
    void mostrar() const; // Agregado const

    Matriz<T> operator + (const Matriz<T>& m) const; // Agregado const
};

#endif


