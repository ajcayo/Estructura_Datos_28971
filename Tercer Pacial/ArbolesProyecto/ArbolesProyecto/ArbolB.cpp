#include "ArbolB.h"
#include "GraficoUtils.h"
#include <algorithm>
using namespace std;

const int ArbolB::T;

void ArbolB::dividirHijo(Nodo* x, int i){
    Nodo* y = x->hijos[i];
    Nodo* z = new Nodo(y->hoja);
    int mid = T - 1;
    for(int j = mid+1; j < (int)y->claves.size(); j++) z->claves.push_back(y->claves[j]);
    if(!y->hoja){
        for(int j = mid+1; j < (int)y->hijos.size(); j++) z->hijos.push_back(y->hijos[j]);
        y->hijos.resize(mid+1);
    }
    int claveMedia = y->claves[mid];
    y->claves.resize(mid);
    x->hijos.insert(x->hijos.begin()+i+1, z);
    x->claves.insert(x->claves.begin()+i, claveMedia);
}
void ArbolB::insertarNoLleno(Nodo* x, int clave){
    int i = (int)x->claves.size() - 1;
    if(x->hoja){
        x->claves.push_back(0);
        while(i >= 0 && x->claves[i] > clave){ x->claves[i+1] = x->claves[i]; i--; }
        x->claves[i+1] = clave;
    } else {
        while(i >= 0 && x->claves[i] > clave) i--;
        i++;
        if((int)x->hijos[i]->claves.size() == 2*T - 1){
            dividirHijo(x, i);
            if(clave > x->claves[i]) i++;
        }
        insertarNoLleno(x->hijos[i], clave);
    }
}
void ArbolB::insertar(int clave){
    if(!raiz){
        raiz = new Nodo(true);
        raiz->claves.push_back(clave);
        return;
    }
    if((int)raiz->claves.size() == 2*T - 1){
        Nodo* s = new Nodo(false);
        s->hijos.push_back(raiz);
        dividirHijo(s, 0);
        raiz = s;
    }
    insertarNoLleno(raiz, clave);
}

vector<int> ArbolB::preOrden(){ vector<int> out; preOrden(raiz, out); return out; }
void ArbolB::preOrden(Nodo* n, vector<int>& out){
    if(!n) return;
    for(int k : n->claves) out.push_back(k);
    for(auto c : n->hijos) preOrden(c, out);
}
vector<int> ArbolB::inOrden(){ vector<int> out; inOrden(raiz, out); return out; }
void ArbolB::inOrden(Nodo* n, vector<int>& out){
    if(!n) return;
    if(n->hoja){ for(int k : n->claves) out.push_back(k); return; }
    for(size_t i = 0; i < n->claves.size(); i++){
        inOrden(n->hijos[i], out);
        out.push_back(n->claves[i]);
    }
    inOrden(n->hijos.back(), out);
}
vector<int> ArbolB::postOrden(){ vector<int> out; postOrden(raiz, out); return out; }
void ArbolB::postOrden(Nodo* n, vector<int>& out){
    if(!n) return;
    for(auto c : n->hijos) postOrden(c, out);
    for(int k : n->claves) out.push_back(k);
}
int ArbolB::contarHojas(Nodo* n){
    if(!n) return 1;
    if(n->hijos.empty()) return 1;
    int total = 0;
    for(auto c : n->hijos) total += contarHojas(c);
    return total;
}

void ArbolB::dibujar(HDC hdc, int x, int y, int anchoDisponible){
    dibujar(hdc, raiz, x, y, anchoDisponible);
}
void ArbolB::dibujar(HDC hdc, Nodo* nodo, int x, int y, int anchoDisponible){
    if(!nodo) return;
    dibujarNodoMultivia(hdc, nodo->claves, x, y);
    if(nodo->hoja) return;
    int n = (int)nodo->hijos.size();
    vector<int> hojasPorHijo(n);
    int totalHojas = 0;
    for(int i = 0; i < n; i++){ hojasPorHijo[i] = contarHojas(nodo->hijos[i]); totalHojas += hojasPorHijo[i]; }
    int startX = x - anchoDisponible/2;
    int acumulado = 0;
    for(int i = 0; i < n; i++){
        int anchoHijo = anchoDisponible * hojasPorHijo[i] / max(1, totalHojas);
        int cx = startX + acumulado + anchoHijo/2;
        int cy = y + NIVEL_ALTO;
        dibujarLinea(hdc, x, y+15, cx, cy-15);
        dibujar(hdc, nodo->hijos[i], cx, cy, max(anchoHijo, 60));
        acumulado += anchoHijo;
    }
}
