#ifndef NODOARBOL_H
#define NODOARBOL_H
#include <string>
using namespace std;

class NodoArbol {
public:
    string valor;
    NodoArbol* izquierdo;
    NodoArbol* derecho;

    NodoArbol(string val) {
        valor = val;
        izquierdo = nullptr;
        derecho = nullptr;
    }
};

#endif
