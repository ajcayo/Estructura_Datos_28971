#ifndef ARBOLEXPRESION_H
#define ARBOLEXPRESION_H

#include "NodoArbol.h"
#include <windows.h>
#include <string>
#include <vector>
using namespace std;

class ArbolExpresion {
private:
    NodoArbol* raiz;

    bool esOperador(string token);
    void liberar(NodoArbol* nodo);

    void preordenRec(NodoArbol* nodo);
    void inordenRec(NodoArbol* nodo);
    void postordenRec(NodoArbol* nodo);

    void dibujarNodo(HDC hdc, NodoArbol* nodo, int x, int y, int desplazamiento, int nivel);

public:
    ArbolExpresion();
    ~ArbolExpresion();

    void construirDesdePostfija(vector<string> tokens);

    void preorden();
    void inorden();
    void postorden();

    NodoArbol* obtenerRaiz();
    void dibujarArbol(HDC hdc, int anchoVentana);
};

#endif
