#include "ArbolBMas.h"
#include "GraficoUtils.h"
#include <algorithm>
using namespace std;

const int ArbolBMas::ORDEN;

ArbolBMas::ResultadoDivision ArbolBMas::insertarRec(Nodo* nodo, int clave){
    if(nodo->hoja){
        int i = (int)nodo->claves.size() - 1;
        nodo->claves.push_back(0);
        while(i >= 0 && nodo->claves[i] > clave){ nodo->claves[i+1] = nodo->claves[i]; i--; }
        nodo->claves[i+1] = clave;
        if((int)nodo->claves.size() < ORDEN) return {false, 0, nullptr};

        Nodo* nuevo = new Nodo(true);
        int mid = (int)nodo->claves.size() / 2;
        for(int j = mid; j < (int)nodo->claves.size(); j++) nuevo->claves.push_back(nodo->claves[j]);
        nodo->claves.resize(mid);
        nuevo->siguiente = nodo->siguiente;
        nodo->siguiente = nuevo;
        return {true, nuevo->claves[0], nuevo};
    } else {
        int i = 0;
        while(i < (int)nodo->claves.size() && clave >= nodo->claves[i]) i++;
        ResultadoDivision r = insertarRec(nodo->hijos[i], clave);
        if(!r.dividio) return {false, 0, nullptr};

        nodo->claves.insert(nodo->claves.begin()+i, r.clave);
        nodo->hijos.insert(nodo->hijos.begin()+i+1, r.nuevo);
        if((int)nodo->hijos.size() <= ORDEN) return {false, 0, nullptr};

        Nodo* nuevo = new Nodo(false);
        int mid = (int)nodo->claves.size() / 2;
        int subeClave = nodo->claves[mid];
        for(int j = mid+1; j < (int)nodo->claves.size(); j++) nuevo->claves.push_back(nodo->claves[j]);
        for(int j = mid+1; j < (int)nodo->hijos.size(); j++) nuevo->hijos.push_back(nodo->hijos[j]);
        nodo->claves.resize(mid);
        nodo->hijos.resize(mid+1);
        return {true, subeClave, nuevo};
    }
}
void ArbolBMas::insertar(int clave){
    if(!raiz){
        raiz = new Nodo(true);
        raiz->claves.push_back(clave);
        return;
    }
    ResultadoDivision r = insertarRec(raiz, clave);
    if(r.dividio){
        Nodo* nuevaRaiz = new Nodo(false);
        nuevaRaiz->claves.push_back(r.clave);
        nuevaRaiz->hijos.push_back(raiz);
        nuevaRaiz->hijos.push_back(r.nuevo);
        raiz = nuevaRaiz;
    }
}

vector<int> ArbolBMas::preOrden(){ vector<int> out; preOrden(raiz, out); return out; }
void ArbolBMas::preOrden(Nodo* n, vector<int>& out){
    if(!n) return;
    for(int k : n->claves) out.push_back(k);
    if(!n->hoja) for(auto c : n->hijos) preOrden(c, out);
}
vector<int> ArbolBMas::inOrden(){ vector<int> out; inOrden(raiz, out); return out; }
void ArbolBMas::inOrden(Nodo* n, vector<int>& out){
    if(!n) return;
    if(n->hoja){ for(int k : n->claves) out.push_back(k); return; }
    for(auto c : n->hijos) inOrden(c, out);
}
vector<int> ArbolBMas::postOrden(){ vector<int> out; postOrden(raiz, out); return out; }
void ArbolBMas::postOrden(Nodo* n, vector<int>& out){
    if(!n) return;
    if(!n->hoja) for(auto c : n->hijos) postOrden(c, out);
    for(int k : n->claves) out.push_back(k);
}
int ArbolBMas::contarHojas(Nodo* n){
    if(!n) return 1;
    if(n->hijos.empty()) return 1;
    int total = 0;
    for(auto c : n->hijos) total += contarHojas(c);
    return total;
}

void ArbolBMas::dibujar(HDC hdc, int x, int y, int anchoDisponible){
    dibujar(hdc, raiz, x, y, anchoDisponible);
}
void ArbolBMas::dibujar(HDC hdc, Nodo* nodo, int x, int y, int anchoDisponible){
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
