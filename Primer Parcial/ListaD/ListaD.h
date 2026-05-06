#ifndef LISTA_H
#define LISTA_H

#include "NodoD.h"

class Lista {
private:
    Nodo* cabeza;
    Nodo* cola;

public:
    Lista();
    ~Lista();

    void insertarCabeza(string, string);
    void insertarCola(string, string);

    void imprimir();
    void imprimirReversa();

    Nodo* buscar(string);
    void eliminar(string);
};

#endif