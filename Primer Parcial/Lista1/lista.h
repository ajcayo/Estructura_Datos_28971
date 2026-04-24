#ifndef LISTA_H
#define LISTA_H
#include "Nodo.h"

class Lista {
private:
    Nodo* cabeza;
    Nodo* cola;

public:
    Lista();
    ~Lista();

    void insertarCabeza(string, string);
    void imprimir();
};

#endif