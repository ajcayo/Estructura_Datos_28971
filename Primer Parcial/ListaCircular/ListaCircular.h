#ifndef LISTACIRCULAR_H
#define LISTACIRCULAR_H

#include "Nodo.h"

class ListaCircular {
private:
    Nodo* cabeza;

public:
    ListaCircular();
    ~ListaCircular();

    void insertar(string, string);
    void imprimir();
    Nodo* buscar(string);
    void eliminar(string);
};

#endif