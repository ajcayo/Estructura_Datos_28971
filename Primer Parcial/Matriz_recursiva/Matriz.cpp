#include "Matriz.h"
#include <stdlib.h>

template <typename T>
Matriz<T>::Matriz(int f, int c, int p) {
    filas = f;
    columnas = c;
    profundidad = p;

    datos = (T***)malloc(sizeof(T**) * filas);
    for(int i = 0; i < filas; i++){
        *(datos + i) = (T**)malloc(sizeof(T*) * columnas);
        for(int j = 0; j < columnas; j++){
            *(*(datos + i) + j) = (T*)malloc(sizeof(T) * profundidad);
        }
    }
}

template <typename T>
Matriz<T>::~Matriz() {
    for(int i = 0; i < filas; i++){
        for(int j = 0; j < columnas; j++){
            free(*(*(datos + i) + j));
        }
        free(*(datos + i));
    }
    free(datos);
}


template <typename T>
void Matriz<T>::ingresarRec(int i, int j, int k) {
    if (i == filas) return;

    cout << "[" << i << "][" << j << "][" << k << "]: ";
    cin >> *(*(*(datos+i)+j)+k);

    if (k + 1 < profundidad) ingresarRec(i, j, k+1);
    else if (j + 1 < columnas) ingresarRec(i, j+1, 0);
    else ingresarRec(i+1, 0, 0);
}

template <typename T>
void Matriz<T>::ingresar() {
    ingresarRec(0,0,0);
}


template <typename T>
void Matriz<T>::mostrarRec(int i, int j, int k) const {
    if (i == filas) return;

    cout << *(*(*(datos+i)+j)+k) << " ";

    if (k + 1 < profundidad) mostrarRec(i, j, k+1);
    else if (j + 1 < columnas) {
        cout << endl;
        mostrarRec(i, j+1, 0);
    }
    else {
        cout << endl << endl;
        mostrarRec(i+1, 0, 0);
    }
}

template <typename T>
void Matriz<T>::mostrar() const {
    mostrarRec(0,0,0);
}


template <typename T>
void multiplicarRec(const Matriz<T>& A, const Matriz<T>& B, Matriz<T>& R,
                    int i, int j, int k, int x) {
    if (i == A.filas) return;

    if (j == B.columnas) {
        multiplicarRec(A, B, R, i+1, 0, k, 0);
        return;
    }

    if (k == A.profundidad) {
        multiplicarRec(A, B, R, i, j+1, 0, 0);
        return;
    }

    if (x == A.columnas) {
        multiplicarRec(A, B, R, i, j, k+1, 0);
        return;
    }

    if (x == 0) *(*(*(R.datos+i)+j)+k) = 0;

    *(*(*(R.datos+i)+j)+k) +=
        (*(*(*(A.datos+i)+x)+k)) *
        (*(*(*(B.datos+x)+j)+k));

    multiplicarRec(A, B, R, i, j, k, x+1);
}

template <typename T>
Matriz<T> Matriz<T>::operator*(const Matriz<T>& m) const {
    Matriz<T> r(filas, m.columnas, profundidad);
    multiplicarRec(*this, m, r, 0, 0, 0, 0);
    return r;
}

template class Matriz<int>;
template class Matriz<float>;
