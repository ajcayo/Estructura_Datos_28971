#ifndef MATRIZ_H
#define MATRIZ_H

#include <iostream>
using namespace std;

template <typename T>
class Matriz {
private:
    T*** datos;
    int filas, columnas, profundidad;

public:
    Matriz(int f, int c, int p);
    ~Matriz();

    void ingresar();
    void mostrar() const;

    Matriz<T> operator-(const Matriz<T>& m) const;
};

#endif