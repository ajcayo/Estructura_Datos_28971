#include <iostream>
#include "DetectorCiclos.h"
using namespace std;


bool DetectorCiclos::detectar(Nodo* cabeza)
{
    // si la lista esta vacia no hay ciclo
    if (cabeza == NULL)
        return false;

    Nodo* lento  = cabeza; // tortuga: 1 paso
    Nodo* rapido = cabeza; // liebre:  2 pasos

    while (rapido != NULL && rapido->siguiente != NULL)
    {
        lento  = lento->siguiente;             // avanza 1
        rapido = rapido->siguiente->siguiente; // avanza 2

      
        if (lento == rapido)
        {
            return true; 
        }
    }

   
    return false; 
}


void DetectorCiclos::detectarConPasos(Nodo* cabeza)
{
    if (cabeza == NULL)
    {
        cout << "La lista esta vacia." << endl;
        return;
    }

    Nodo* lento  = cabeza;
    Nodo* rapido = cabeza;
    int paso = 0;

    cout << "  [Inicio] Lento y Rapido apuntan al nodo: "
         << cabeza->valor << endl;

    while (rapido != NULL && rapido->siguiente != NULL)
    {
        lento  = lento->siguiente;
        rapido = rapido->siguiente->siguiente;
        paso++;

        cout << "  [Paso " << paso << "]"
             << "  Lento -> " << lento->valor
             << "  |  Rapido -> " << rapido->valor;

        if (lento == rapido)
        {
            cout << "  <-- SE ENCONTRARON! Hay ciclo." << endl;
            return;
        }

        cout << endl;
    }

    cout << "  El puntero rapido llego al final. No hay ciclo." << endl;
}
