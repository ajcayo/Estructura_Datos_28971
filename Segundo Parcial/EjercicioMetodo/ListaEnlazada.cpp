#include <iostream>
#include "ListaEnlazada.h"
using namespace std;


ListaEnlazada::ListaEnlazada()
{
    cabeza = NULL; 
}


void ListaEnlazada::agregar(int valor)
{
    Nodo* nuevo = new Nodo(valor);


    if (cabeza == NULL)
    {
        cabeza = nuevo;
        return;
    }


    Nodo* temp = cabeza;
    while (temp->siguiente != NULL)
    {
        temp = temp->siguiente;
    }


    temp->siguiente = nuevo;
}


void ListaEnlazada::crearCiclo(int posicion)
{
    if (cabeza == NULL)
        return;


    Nodo* destino = cabeza;
    for (int i = 0; i < posicion; i++)
    {
        if (destino->siguiente == NULL)
            return; 
        destino = destino->siguiente;
    }


    Nodo* ultimo = cabeza;
    while (ultimo->siguiente != NULL)
    {
        ultimo = ultimo->siguiente;
    }


    ultimo->siguiente = destino;
}


void ListaEnlazada::mostrar()
{
    Nodo* temp = cabeza;
    while (temp != NULL)
    {
        cout << temp->valor;
        if (temp->siguiente != NULL)
            cout << " -> ";
        temp = temp->siguiente;
    }
    cout << " -> NULL" << endl;
}

Nodo* ListaEnlazada::getCabeza()
{
    return cabeza;
}
