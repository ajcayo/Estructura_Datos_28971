#include "Proceso.h"

template <typename T>
Matriz<T> Proceso<T>::restar(const Matriz<T>& m1, const Matriz<T>& m2) {
    return m1 - m2;
}

template <typename T>
void Proceso<T>::mostrar(const Matriz<T>& m) {
    m.mostrar();
}

template class Proceso<int>;
template class Proceso<float>;