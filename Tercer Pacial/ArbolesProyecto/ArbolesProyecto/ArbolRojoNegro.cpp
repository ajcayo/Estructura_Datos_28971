#include "ArbolRojoNegro.h"
#include "GraficoUtils.h"
#include <string>
#include <algorithm>
using namespace std;

void ArbolRojoNegro::rotarIzq(Nodo* x){
    Nodo* y = x->der;
    x->der = y->izq;
    if(y->izq) y->izq->padre = x;
    y->padre = x->padre;
    if(!x->padre) raiz = y;
    else if(x == x->padre->izq) x->padre->izq = y;
    else x->padre->der = y;
    y->izq = x;
    x->padre = y;
}
void ArbolRojoNegro::rotarDer(Nodo* x){
    Nodo* y = x->izq;
    x->izq = y->der;
    if(y->der) y->der->padre = x;
    y->padre = x->padre;
    if(!x->padre) raiz = y;
    else if(x == x->padre->der) x->padre->der = y;
    else x->padre->izq = y;
    y->der = x;
    x->padre = y;
}
void ArbolRojoNegro::arreglarInsercion(Nodo* z){
    while(z->padre && z->padre->color == ROJO){
        Nodo* abuelo = z->padre->padre;
        if(!abuelo) break;
        if(z->padre == abuelo->izq){
            Nodo* tio = abuelo->der;
            if(tio && tio->color == ROJO){
                z->padre->color = NEGRO; tio->color = NEGRO; abuelo->color = ROJO; z = abuelo;
            } else {
                if(z == z->padre->der){ z = z->padre; rotarIzq(z); }
                z->padre->color = NEGRO; abuelo->color = ROJO; rotarDer(abuelo);
            }
        } else {
            Nodo* tio = abuelo->izq;
            if(tio && tio->color == ROJO){
                z->padre->color = NEGRO; tio->color = NEGRO; abuelo->color = ROJO; z = abuelo;
            } else {
                if(z == z->padre->izq){ z = z->padre; rotarDer(z); }
                z->padre->color = NEGRO; abuelo->color = ROJO; rotarIzq(abuelo);
            }
        }
    }
    raiz->color = NEGRO;
}
void ArbolRojoNegro::insertar(int valor){
    Nodo* z = new Nodo(valor);
    Nodo* y = nullptr;
    Nodo* x = raiz;
    while(x){
        y = x;
        if(z->valor < x->valor) x = x->izq; else x = x->der;
    }
    z->padre = y;
    if(!y) raiz = z;
    else if(z->valor < y->valor) y->izq = z; else y->der = z;
    arreglarInsercion(z);
}

vector<int> ArbolRojoNegro::preOrden(){ vector<int> out; preOrden(raiz, out); return out; }
void ArbolRojoNegro::preOrden(Nodo* nodo, vector<int>& out){
    if(!nodo) return;
    out.push_back(nodo->valor);
    preOrden(nodo->izq, out);
    preOrden(nodo->der, out);
}
vector<int> ArbolRojoNegro::inOrden(){ vector<int> out; inOrden(raiz, out); return out; }
void ArbolRojoNegro::inOrden(Nodo* nodo, vector<int>& out){
    if(!nodo) return;
    inOrden(nodo->izq, out);
    out.push_back(nodo->valor);
    inOrden(nodo->der, out);
}
vector<int> ArbolRojoNegro::postOrden(){ vector<int> out; postOrden(raiz, out); return out; }
void ArbolRojoNegro::postOrden(Nodo* nodo, vector<int>& out){
    if(!nodo) return;
    postOrden(nodo->izq, out);
    postOrden(nodo->der, out);
    out.push_back(nodo->valor);
}

void ArbolRojoNegro::dibujar(HDC hdc, int x, int y, int offsetX){
    dibujar(hdc, raiz, x, y, offsetX);
}
void ArbolRojoNegro::dibujar(HDC hdc, Nodo* nodo, int x, int y, int offsetX){
    if(!nodo) return;
    if(nodo->izq){
        int nx = x - offsetX, ny = y + NIVEL_ALTO;
        dibujarLinea(hdc, x, y, nx, ny);
        dibujar(hdc, nodo->izq, nx, ny, max(offsetX/2, 30));
    }
    if(nodo->der){
        int nx = x + offsetX, ny = y + NIVEL_ALTO;
        dibujarLinea(hdc, x, y, nx, ny);
        dibujar(hdc, nodo->der, nx, ny, max(offsetX/2, 30));
    }
    HBRUSH pincel = CreateSolidBrush(nodo->color == ROJO ? RGB(220,40,40) : RGB(30,30,30));
    HBRUSH viejo = (HBRUSH)SelectObject(hdc, pincel);
    Ellipse(hdc, x-RADIO, y-RADIO, x+RADIO, y+RADIO);
    SelectObject(hdc, viejo);
    DeleteObject(pincel);
    SetTextColor(hdc, RGB(255,255,255));
    string s = to_string(nodo->valor);
    RECT r = {x-RADIO, y-RADIO, x+RADIO, y+RADIO};
    DrawTextA(hdc, s.c_str(), -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    SetTextColor(hdc, RGB(0,0,0));
}
