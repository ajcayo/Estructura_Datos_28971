#ifndef MATRIZ_H
#define MATRIZ_H

#include <iostream>
using namespace std;

template <typename T>
class Matriz {
private:
    T*** datos;
    int filas, columnas, profundidad;


    void ingresarRec(int i, int j, int k);
    void mostrarRec(int i, int j, int k) const;

public:
    Matriz(int f, int c, int p);
    ~Matriz();

    void ingresar();
    void mostrar() const;

    Matriz<T> operator*(const Matriz<T>& m) const;


    template <typename U>
    friend void multiplicarRec(const Matriz<U>& A, const Matriz<U>& B, Matriz<U>& R,
                               int i, int j, int k, int x);
};

#endif
