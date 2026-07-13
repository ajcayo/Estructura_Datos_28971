#include "ArbolBinario.h"
#include "GraficoUtils.h"
#include <string>
#include <algorithm>
using namespace std;

void ArbolBinario::insertar(int valor){
    raiz = insertar(raiz, valor);
}
ArbolBinario::Nodo* ArbolBinario::insertar(Nodo* nodo, int valor){
    if(!nodo) return new Nodo(valor);
    if(valor < nodo->valor) nodo->izq = insertar(nodo->izq, valor);
    else if(valor > nodo->valor) nodo->der = insertar(nodo->der, valor);
    return nodo;
}

vector<int> ArbolBinario::preOrden(){ vector<int> out; preOrden(raiz, out); return out; }
void ArbolBinario::preOrden(Nodo* nodo, vector<int>& out){
    if(!nodo) return;
    out.push_back(nodo->valor);
    preOrden(nodo->izq, out);
    preOrden(nodo->der, out);
}

vector<int> ArbolBinario::inOrden(){ vector<int> out; inOrden(raiz, out); return out; }
void ArbolBinario::inOrden(Nodo* nodo, vector<int>& out){
    if(!nodo) return;
    inOrden(nodo->izq, out);
    out.push_back(nodo->valor);
    inOrden(nodo->der, out);
}

vector<int> ArbolBinario::postOrden(){ vector<int> out; postOrden(raiz, out); return out; }
void ArbolBinario::postOrden(Nodo* nodo, vector<int>& out){
    if(!nodo) return;
    postOrden(nodo->izq, out);
    postOrden(nodo->der, out);
    out.push_back(nodo->valor);
}

void ArbolBinario::dibujar(HDC hdc, int x, int y, int offsetX){
    dibujar(hdc, raiz, x, y, offsetX);
}
void ArbolBinario::dibujar(HDC hdc, Nodo* nodo, int x, int y, int offsetX){
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
    Ellipse(hdc, x-RADIO, y-RADIO, x+RADIO, y+RADIO);
    string s = to_string(nodo->valor);
    RECT r = {x-RADIO, y-RADIO, x+RADIO, y+RADIO};
    DrawTextA(hdc, s.c_str(), -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
