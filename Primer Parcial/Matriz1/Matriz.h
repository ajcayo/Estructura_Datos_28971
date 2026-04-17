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
    Matriz(int f, int c) {
        filas = f;
        columnas = c;
        datos = new T*[filas];
        for(int i = 0; i < filas; i++) {
            datos[i] = new T[columnas];
        }
    }

    // --- AGREGADO: Constructor de Copias para evitar errores de memoria ---
    Matriz(const Matriz<T>& otra) {
        filas = otra.filas;
        columnas = otra.columnas;
        datos = new T*[filas];
        for(int i = 0; i < filas; i++) {
            datos[i] = new T[columnas];
            for(int j = 0; j < columnas; j++) {
                datos[i][j] = otra.datos[i][j];
            }
        }
    }

    ~Matriz() {
        for(int i = 0; i < filas; i++) {
            delete[] datos[i];
        }
        delete[] datos;
    }

    void ingresar() {
        cout << "Ingrese datos:" << endl;
        for(int i = 0; i < filas; i++) {
            for(int j = 0; j < columnas; j++) {
                cout << "[" << i << "][" << j << "]: ";
                cin >> datos[i][j];
            }
        }
    }

    void mostrar() const {
        for(int i = 0; i < filas; i++) {
            for(int j = 0; j < columnas; j++) {
                cout << datos[i][j] << " ";
            }
            cout << endl;
        }
    }

    // Corregido: se añade 'const' para que Proceso pueda usarlo con referencias constantes
    Matriz<T> operator + (const Matriz<T>& m) const {
        Matriz<T> resultado(filas, columnas);
        for(int i = 0; i < filas; i++) {
            for(int j = 0; j < columnas; j++) {
                resultado.datos[i][j] = this->datos[i][j] + m.datos[i][j];
            }
        }
        return resultado;
    }
};

#endif

