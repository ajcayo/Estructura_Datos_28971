#include "ArbolExpresion.h"
#include <iostream>
#include <stack>
#include <algorithm>
using namespace std;

ArbolExpresion::ArbolExpresion() {
    raiz = nullptr;
}

ArbolExpresion::~ArbolExpresion() {
    liberar(raiz);
}

void ArbolExpresion::liberar(NodoArbol* nodo) {
    if (nodo == nullptr) return;
    liberar(nodo->izquierdo);
    liberar(nodo->derecho);
    delete nodo;
}

bool ArbolExpresion::esOperador(string token) {
    return (token == "+" || token == "-" || token == "*" || token == "/" || token == "^");
}

// Construye el arbol de expresion a partir de tokens en notacion POSTFIJA (polaca inversa)
// Ejemplo: "3 4 + 2 *"  ->  (3 + 4) * 2
void ArbolExpresion::construirDesdePostfija(vector<string> tokens) {
    stack<NodoArbol*> pila;

    for (int i = 0; i < (int)tokens.size(); i++) {
        string token = tokens[i];

        if (esOperador(token)) {
            if (pila.size() < 2) {
                cout << "Error: la expresion postfija no es valida." << endl;
                return;
            }
            NodoArbol* derecho = pila.top(); pila.pop();
            NodoArbol* izquierdo = pila.top(); pila.pop();

            NodoArbol* nuevo = new NodoArbol(token);
            nuevo->izquierdo = izquierdo;
            nuevo->derecho = derecho;

            pila.push(nuevo);
        } else {
            NodoArbol* hoja = new NodoArbol(token);
            pila.push(hoja);
        }
    }

    if (pila.size() != 1) {
        cout << "Error: la expresion postfija no es valida." << endl;
        return;
    }

    raiz = pila.top();
}

void ArbolExpresion::preordenRec(NodoArbol* nodo) {
    if (nodo == nullptr) return;
    cout << nodo->valor << " ";
    preordenRec(nodo->izquierdo);
    preordenRec(nodo->derecho);
}

void ArbolExpresion::inordenRec(NodoArbol* nodo) {
    if (nodo == nullptr) return;
    bool esOp = esOperador(nodo->valor);
    if (esOp) cout << "(";
    inordenRec(nodo->izquierdo);
    cout << nodo->valor;
    inordenRec(nodo->derecho);
    if (esOp) cout << ")";
}

void ArbolExpresion::postordenRec(NodoArbol* nodo) {
    if (nodo == nullptr) return;
    postordenRec(nodo->izquierdo);
    postordenRec(nodo->derecho);
    cout << nodo->valor << " ";
}

void ArbolExpresion::preorden() {
    cout << "Preorden  : ";
    preordenRec(raiz);
    cout << endl;
}

void ArbolExpresion::inorden() {
    cout << "Inorden   : ";
    inordenRec(raiz);
    cout << endl;
}

void ArbolExpresion::postorden() {
    cout << "Postorden : ";
    postordenRec(raiz);
    cout << endl;
}

NodoArbol* ArbolExpresion::obtenerRaiz() {
    return raiz;
}

void ArbolExpresion::dibujarNodo(HDC hdc, NodoArbol* nodo, int x, int y, int desplazamiento, int nivel) {
    if (nodo == nullptr) return;

    int radio = 20;
    int siguienteY = y + 90;

    if (nodo->izquierdo != nullptr) {
        int xIzq = x - desplazamiento;
        MoveToEx(hdc, x, y + radio, NULL);
        LineTo(hdc, xIzq, siguienteY - radio);
        dibujarNodo(hdc, nodo->izquierdo, xIzq, siguienteY, desplazamiento / 2, nivel + 1);
    }

    if (nodo->derecho != nullptr) {
        int xDer = x + desplazamiento;
        MoveToEx(hdc, x, y + radio, NULL);
        LineTo(hdc, xDer, siguienteY - radio);
        dibujarNodo(hdc, nodo->derecho, xDer, siguienteY, desplazamiento / 2, nivel + 1);
    }

    HBRUSH pincel = CreateSolidBrush(RGB(173, 216, 230));
    HBRUSH pincelAnterior = (HBRUSH)SelectObject(hdc, pincel);
    Ellipse(hdc, x - radio, y - radio, x + radio, y + radio);
    SelectObject(hdc, pincelAnterior);
    DeleteObject(pincel);

    SetBkMode(hdc, TRANSPARENT);
    RECT rect = { x - radio, y - radio, x + radio, y + radio };
    DrawText(hdc, nodo->valor.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void ArbolExpresion::dibujarArbol(HDC hdc, int anchoVentana) {
    if (raiz == nullptr) return;
    int desplazamientoInicial = anchoVentana / 4;
    dibujarNodo(hdc, raiz, anchoVentana / 2, 60, desplazamientoInicial, 0);
}
