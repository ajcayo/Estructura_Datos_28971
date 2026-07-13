#pragma once
#include "ComunWin.h"
#include <vector>

class ArbolB {
public:
    ArbolB() : raiz(nullptr) {}

    void insertar(int clave);
    std::vector<int> preOrden();
    std::vector<int> inOrden();
    std::vector<int> postOrden();
    void dibujar(HDC hdc, int x, int y, int anchoDisponible);

private:
    static const int T = 3; // grado minimo (maximo de claves = 2T-1 = 5)

    struct Nodo {
        std::vector<int> claves;
        std::vector<Nodo*> hijos;
        bool hoja;
        Nodo(bool esHoja) : hoja(esHoja) {}
    };

    Nodo* raiz;

    void dividirHijo(Nodo* x, int i);
    void insertarNoLleno(Nodo* x, int clave);
    void preOrden(Nodo* n, std::vector<int>& out);
    void inOrden(Nodo* n, std::vector<int>& out);
    void postOrden(Nodo* n, std::vector<int>& out);
    int contarHojas(Nodo* n);
    void dibujar(HDC hdc, Nodo* nodo, int x, int y, int anchoDisponible);
};
