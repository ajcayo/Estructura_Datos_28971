#include "Matriz.h"

template <typename T>
Matriz<T>::Matriz(int f, int c) {
    filas = f;
    columnas = c;
    datos = new T*[filas];
    for(int i = 0; i < filas; i++) {
        datos[i] = new T[columnas];
    }
}

template <typename T>
Matriz<T>::~Matriz() {
    for(int i = 0; i < filas; i++) {
        delete[] datos[i];
    }
    delete[] datos;
}

template <typename T>
Matriz<T>::Matriz(const Matriz<T>& other) {
    filas = other.filas;
    columnas = other.columnas;
    datos = new T*[filas];
    for(int i = 0; i < filas; i++) {
        datos[i] = new T[columnas];
        for(int j = 0; j < columnas; j++) {
            datos[i][j] = other.datos[i][j];
        }
    }
}

template <typename T>
Matriz<T>& Matriz<T>::operator=(const Matriz<T>& other) {
    if (this == &other) return *this;
    for(int i = 0; i < filas; i++) delete[] datos[i];
    delete[] datos;

    filas = other.filas;
    columnas = other.columnas;
    datos = new T*[filas];
    for(int i = 0; i < filas; i++) {
        datos[i] = new T[columnas];
        for(int j = 0; j < columnas; j++) {
            datos[i][j] = other.datos[i][j];
        }
    }
    return *this;
}

template <typename T>
void Matriz<T>::ingresar() {
    cout << "Ingrese datos:" << endl;
    for(int i = 0; i < filas; i++) {
        for(int j = 0; j < columnas; j++) {
            cout << "[" << i << "][" << j << "]: ";
            cin >> datos[i][j];
        }
    }
}

template <typename T>
void Matriz<T>::mostrar() const {
    for(int i = 0; i < filas; i++) {
        for(int j = 0; j < columnas; j++) {
            cout << datos[i][j] << " ";
        }
        cout << endl;
    }
}

template <typename T>
Matriz<T> Matriz<T>::operator + (const Matriz<T>& m) const { 
    Matriz<T> resultado(filas, columnas);
    for(int i = 0; i < filas; i++) {
        for(int j = 0; j < columnas; j++) {
            resultado.datos[i][j] = datos[i][j] + m.datos[i][j];
        }
    }
    return resultado;
}

template class Matriz<int>;
template class Matriz<float>;


