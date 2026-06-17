// ============================================
//  Archivo: main.cpp
//  Proyecto: Algoritmo de Floyd
//            Punteros Rapido y Lento
//  Descripcion: Programa principal que prueba
//  el detector de ciclos con 3 casos distintos
// ============================================

#include <iostream>
#include "ListaEnlazada.h"
#include "DetectorCiclos.h"
using namespace std;

// funcion auxiliar para imprimir separadores
void separador()
{
    cout << "----------------------------------------" << endl;
}

int main()
{
    DetectorCiclos detector;

    cout << "========================================" << endl;
    cout << "   ALGORITMO DE FLOYD" << endl;
    cout << "   Deteccion de Ciclos en Listas" << endl;
    cout << "========================================" << endl;
    cout << endl;

    // ==========================================
    // CASO 1: Lista normal SIN ciclo
    // 10 -> 20 -> 30 -> 40 -> 50 -> NULL
    // ==========================================
    separador();
    cout << "CASO 1: Lista SIN ciclo" << endl;
    separador();

    ListaEnlazada lista1;
    lista1.agregar(10);
    lista1.agregar(20);
    lista1.agregar(30);
    lista1.agregar(40);
    lista1.agregar(50);

    cout << "Lista: ";
    lista1.mostrar();
    cout << endl;

    cout << "Ejecutando algoritmo de Floyd..." << endl;
    detector.detectarConPasos(lista1.getCabeza());
    cout << endl;

    if (detector.detectar(lista1.getCabeza()))
        cout << ">> RESULTADO: HAY un ciclo." << endl;
    else
        cout << ">> RESULTADO: NO hay ciclo." << endl;

    cout << endl;

    // ==========================================
    // CASO 2: Lista CON ciclo al principio
    // 1 -> 2 -> 3 -> 4 -> 5
    //           ^              |
    //           |______________|
    //  (el nodo 5 apunta de regreso al nodo 3)
    // ==========================================
    separador();
    cout << "CASO 2: Lista CON ciclo (hacia el nodo 3)" << endl;
    separador();

    ListaEnlazada lista2;
    lista2.agregar(1);
    lista2.agregar(2);
    lista2.agregar(3);
    lista2.agregar(4);
    lista2.agregar(5);

    // posicion 2 = tercer nodo (el que vale 3)
    lista2.crearCiclo(2);

    // NO llamamos lista2.mostrar() porque entraria en loop infinito
    cout << "Lista: 1 -> 2 -> 3 -> 4 -> 5 -> (ciclo al nodo 3)" << endl;
    cout << endl;

    cout << "Ejecutando algoritmo de Floyd..." << endl;
    detector.detectarConPasos(lista2.getCabeza());
    cout << endl;

    if (detector.detectar(lista2.getCabeza()))
        cout << ">> RESULTADO: HAY un ciclo." << endl;
    else
        cout << ">> RESULTADO: NO hay ciclo." << endl;

    cout << endl;

    // ==========================================
    // CASO 3: Lista CON ciclo en el mismo ultimo nodo
    // 7 -> 14 -> 21 -> 28
    //                   ^   |
    //                   |___|
    //  (el ultimo nodo apunta a si mismo)
    // ==========================================
    separador();
    cout << "CASO 3: Lista CON ciclo (el ultimo nodo apunta a si mismo)" << endl;
    separador();

    ListaEnlazada lista3;
    lista3.agregar(7);
    lista3.agregar(14);
    lista3.agregar(21);
    lista3.agregar(28);

    // posicion 3 = cuarto nodo (el ultimo, el que vale 28)
    lista3.crearCiclo(3);

    cout << "Lista: 7 -> 14 -> 21 -> 28 -> (ciclo a si mismo)" << endl;
    cout << endl;

    cout << "Ejecutando algoritmo de Floyd..." << endl;
    detector.detectarConPasos(lista3.getCabeza());
    cout << endl;

    if (detector.detectar(lista3.getCabeza()))
        cout << ">> RESULTADO: HAY un ciclo." << endl;
    else
        cout << ">> RESULTADO: NO hay ciclo." << endl;

    cout << endl;
    separador();
    cout << "Fin del programa." << endl;
    separador();

    cout << "\nPresiona Enter para salir...";
    cin.get();

    return 0;
}