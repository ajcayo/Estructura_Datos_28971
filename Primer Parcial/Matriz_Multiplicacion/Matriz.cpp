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
void Matriz<T>::ingresar() {
    for(int i=0;i<filas;i++){
        for(int j=0;j<columnas;j++){
            for(int k=0;k<profundidad;k++){
                cout<<"["<<i<<"]["<<j<<"]["<<k<<"]: ";
                cin >> *(*(*(datos+i)+j)+k);
            }
        }
    }
}

template <typename T>
void Matriz<T>::mostrar() const {
    for(int i=0;i<filas;i++){
        for(int j=0;j<columnas;j++){
            for(int k=0;k<profundidad;k++){
                cout << *(*(*(datos+i)+j)+k) << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}

template <typename T>
Matriz<T> Matriz<T>::operator*(const Matriz<T>& m) const {

    // resultado: filas de A x columnas de B
    Matriz<T> r(filas, m.columnas, profundidad);

    for(int k=0;k<profundidad;k++){           // capa
        for(int i=0;i<filas;i++){             // filas A
            for(int j=0;j<m.columnas;j++){    // columnas B

                *(*(*(r.datos+i)+j)+k) = 0;

                for(int x=0;x<columnas;x++){  // suma
                    *(*(*(r.datos+i)+j)+k) +=
                    (*(*(*(datos+i)+x)+k)) *
                    (*(*(*(m.datos+x)+j)+k));
                }
            }
        }
    }
    return r;
}

template class Matriz<int>;
template class Matriz<float>;