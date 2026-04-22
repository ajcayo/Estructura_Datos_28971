#ifndef MATRIZ_H
#define MATRIZ_H

#include <iostream>
#include <stdlib.h>
using namespace std;

template <typename T>
class Matriz {
private:
    T** datos;
    int filas;
    int columnas;

public:

    Matriz(int f, int c) {
        filas = f;
        columnas = c;

        datos = (T**)malloc(sizeof(T*) * filas);
        for(int i = 0; i < filas; i++) {
            *(datos + i) = (T*)malloc(sizeof(T) * columnas);
        }
    }


    T** getDatos() {
        return datos;
    }

    int getFilas() {
        return filas;
    }

    int getColumnas() {
        return columnas;
    }


    void setDato(int i, int j, T valor) {
        *(*(datos + i) + j) = valor;
    }

    T getDato(int i, int j) {
        return *(*(datos + i) + j);
    }


    void encerar() {
        for(int i = 0; i < filas; i++) {
            for(int j = 0; j < columnas; j++) {
                *(*(datos + i) + j) = 0;
            }
        }
    }


    void ingresar() {
        for(int i = 0; i < filas; i++) {
            for(int j = 0; j < columnas; j++) {
                cout << "Ingrese [" << i << "][" << j << "]: ";
                cin >> *(*(datos + i) + j);
            }
        }
    }


    void imprimir() {
        for(int i = 0; i < filas; i++) {
            for(int j = 0; j < columnas; j++) {
                cout << *(*(datos + i) + j) << " ";
            }
            cout << endl;
        }
    }


    Matriz<T> operator+(Matriz<T>& m) {
        Matriz<T> r(filas, columnas);

        for(int i = 0; i < filas; i++) {
            for(int j = 0; j < columnas; j++) {
                *(*(r.datos + i) + j) =
                    *(*(datos + i) + j) + *(*(m.datos + i) + j);
            }
        }
        return r;
    }
};

#endif

