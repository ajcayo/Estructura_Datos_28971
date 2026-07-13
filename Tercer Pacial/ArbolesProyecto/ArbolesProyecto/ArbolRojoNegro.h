#pragma once
#include "ComunWin.h"
#include <vector>

class ArbolRojoNegro {
public:
    ArbolRojoNegro() : raiz(nullptr) {}

    void insertar(int valor);
    std::vector<int> preOrden();
    std::vector<int> inOrden();
    std::vector<int> postOrden();
    void dibujar(HDC hdc, int x, int y, int offsetX);

private:
    enum Color { ROJO, NEGRO };
    struct Nodo {
        int valor;
        Color color;
        Nodo *izq, *der, *padre;
        Nodo(int v) : valor(v), color(ROJO), izq(nullptr), der(nullptr), padre(nullptr) {}
    };

    Nodo* raiz;

    void rotarIzq(Nodo* x);
    void rotarDer(Nodo* x);
    void arreglarInsercion(Nodo* z);
    void preOrden(Nodo* nodo, std::vector<int>& out);
    void inOrden(Nodo* nodo, std::vector<int>& out);
    void postOrden(Nodo* nodo, std::vector<int>& out);
    void dibujar(HDC hdc, Nodo* nodo, int x, int y, int offsetX);
};
