#include "Vector.h"
#include <iostream>
using namespace std;


template <typename T>
Vector<T>::Vector() {
    capacidad = 2;
    cantidad = 0;
    datos = new T[capacidad];
}


template <typename T>
Vector<T>::~Vector() {
    delete[] datos;
}


template <typename T>
void Vector<T>::redimensionar() {
    capacidad *= 2;
    T* nuevo = new T[capacidad];

    for (int i = 0; i < cantidad; i++) {
        nuevo[i] = datos[i];
    }

    delete[] datos;
    datos = nuevo;
}

template <typename T>
void Vector<T>::agregar(T dato) {
    if (cantidad == capacidad) {
        redimensionar();
    }
    datos[cantidad++] = dato;
}

template <typename T>
void Vector<T>::eliminar() {
    if (cantidad > 0) {
        cantidad--;
    } else {
        cout << "Vector vacio" << endl;
    }
}


template <typename T>
T Vector<T>::obtener(int index) {
    if (index >= 0 && index < cantidad) {
        return datos[index];
    } else {
        cout << "Indice fuera de rango" << endl;
        return T(); 
    }
}

template <typename T>
int Vector<T>::tamanio() {
    return cantidad;
}