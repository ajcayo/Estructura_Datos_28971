#ifndef LISTAENLAZADA_H
#define LISTAENLAZADA_H


#include "Nodo.h"

class ListaEnlazada
{
    private:
        Nodo* cabeza; 

    public:
     
        ListaEnlazada();

   
        void agregar(int valor);


        void crearCiclo(int posicion);


        void mostrar();


        Nodo* getCabeza();
};

#endif
