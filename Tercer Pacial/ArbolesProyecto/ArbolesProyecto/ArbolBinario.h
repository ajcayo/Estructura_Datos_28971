#pragma once
#include "ComunWin.h"
#include <vector>

class ArbolBinario {
public:
    ArbolBinario() : raiz(nullptr) {}

    void insertar(int valor);
    std::vector<int> preOrden();
    std::vector<int> inOrden();
    std::vector<int> postOrden();
    void dibujar(HDC hdc, int x, int y, int offsetX);

private:
    struct Nodo {
        int valor;
        Nodo* izq;
        Nodo* der;
        Nodo(int v) : valor(v), izq(nullptr), der(nullptr) {}
    };

    Nodo* raiz;

    Nodo* insertar(Nodo* nodo, int valor);
    void preOrden(Nodo* nodo, std::vector<int>& out);
    void inOrden(Nodo* nodo, std::vector<int>& out);
    void postOrden(Nodo* nodo, std::vector<int>& out);
    void dibujar(HDC hdc, Nodo* nodo, int x, int y, int offsetX);
};
