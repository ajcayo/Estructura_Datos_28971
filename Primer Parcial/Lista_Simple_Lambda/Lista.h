#ifndef LISTA_H
#define LISTA_H

#include "Nodo.h"
#include <fstream>
#include <regex>
#include <functional>

using namespace std;

class Lista {
private:
    Nodo* cabeza;
    Nodo* cola;

public:
    Lista();
    ~Lista();

    void insertarCabeza(string, string);
    void insertarFinal(string, string);
    void insertarEntre(string, string, string);

    void imprimir();
    Nodo* buscar(string);
    void eliminar(string);

    void cargarDatos(string);
    void guardarDatos(string);

    string* cargarProvincias(string);
    void reporteProvincias(string*);

    bool esCedulaValida(string);
    bool existeCedula(string);
    bool esNombreValido(string);

    int contarVocales(string);
    int contarLetras(string);

    // LAMBDA BASE
    void recorrer(function<void(Nodo*)>);
};

#endif