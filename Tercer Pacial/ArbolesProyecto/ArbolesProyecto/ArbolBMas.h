#pragma once
#include "ComunWin.h"
#include <vector>

class ArbolBMas {
public:
    ArbolBMas() : raiz(nullptr) {}

    void insertar(int clave);
    std::vector<int> preOrden();
    std::vector<int> inOrden();
    std::vector<int> postOrden();
    void dibujar(HDC hdc, int x, int y, int anchoDisponible);

private:
    static const int ORDEN = 4; // maximo de hijos por nodo interno

    struct Nodo {
        bool hoja;
        std::vector<int> claves;
        std::vector<Nodo*> hijos;   // solo internos
        Nodo* siguiente;            // solo hojas (enlace entre hojas)
        Nodo(bool esHoja) : hoja(esHoja), siguiente(nullptr) {}
    };

    struct ResultadoDivision {
        bool dividio;
        int clave;
        Nodo* nuevo;
    };

    Nodo* raiz;

    ResultadoDivision insertarRec(Nodo* nodo, int clave);
    void preOrden(Nodo* n, std::vector<int>& out);
    void inOrden(Nodo* n, std::vector<int>& out);
    void postOrden(Nodo* n, std::vector<int>& out);
    int contarHojas(Nodo* n);
    void dibujar(HDC hdc, Nodo* nodo, int x, int y, int anchoDisponible);
};
