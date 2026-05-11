#include <iostream>
#include <fstream>
#include <regex>
#include <cctype>
#include <functional>
#include "Lista.h"
using namespace std;

Lista::Lista() {
    cabeza = nullptr;
    cola = nullptr;
}

Lista::~Lista() {
    Nodo* aux = cabeza;
    while (aux != nullptr) {
        Nodo* temp = aux;
        aux = aux->getSiguiente();
        delete temp;
    }
}

// 🔥 RECORRIDO GENERAL CON LAMBDA
void Lista::recorrer(function<void(Nodo*)> accion) {
    Nodo* aux = cabeza;
    while (aux != nullptr) {
        accion(aux);
        aux = aux->getSiguiente();
    }
}

// INSERTAR
void Lista::insertarCabeza(string cedula, string nombre) {
    Nodo* nuevo = new Nodo(cedula, nombre);

    if (cabeza == nullptr) {
        cabeza = nuevo;
        cola = nuevo;
    } else {
        nuevo->setSiguiente(cabeza);
        cabeza = nuevo;
    }
}

void Lista::insertarFinal(string cedula, string nombre) {
    Nodo* nuevo = new Nodo(cedula, nombre);

    if (cabeza == nullptr) {
        cabeza = nuevo;
        cola = nuevo;
    } else {
        cola->setSiguiente(nuevo);
        cola = nuevo;
    }
}

void Lista::insertarEntre(string ref, string cedula, string nombre) {
    Nodo* actual = cabeza;

    while (actual != nullptr && actual->getCedula() != ref) {
        actual = actual->getSiguiente();
    }

    if (actual != nullptr) {
        Nodo* nuevo = new Nodo(cedula, nombre);
        nuevo->setSiguiente(actual->getSiguiente());
        actual->setSiguiente(nuevo);

        if (nuevo->getSiguiente() == nullptr)
            cola = nuevo;

        cout << "Insertado entre nodos\n";
    } else {
        cout << "No se encontro referencia\n";
    }
}

//IMPRIMIR CON LAMBDA
void Lista::imprimir() {
    if (cabeza == nullptr) {
        cout << "Lista vacia\n";
        return;
    }

    recorrer([](Nodo* n) {
        cout << n->getCedula() << " - " << n->getNombre() << endl;
    });
}

//BUSCAR CON LAMBDA
Nodo* Lista::buscar(string cedula) {
    Nodo* encontrado = nullptr;

    recorrer([&](Nodo* n) {
        if (n->getCedula() == cedula)
            encontrado = n;
    });

    return encontrado;
}

// ELIMINAR (se mantiene normal por lógica compleja)
void Lista::eliminar(string cedula) {
    Nodo* aux = cabeza;
    Nodo* ant = nullptr;

    while (aux != nullptr) {
        if (aux->getCedula() == cedula) {

            if (ant == nullptr) {
                cabeza = aux->getSiguiente();
                if (cabeza == nullptr) cola = nullptr;
            } else {
                ant->setSiguiente(aux->getSiguiente());
                if (aux == cola) cola = ant;
            }

            delete aux;
            cout << "Eliminado\n";
            return;
        }

        ant = aux;
        aux = aux->getSiguiente();
    }

    cout << "No encontrado\n";
}

//GUARDAR CON LAMBDA
void Lista::guardarDatos(string archivo) {
    ofstream file(archivo);

    recorrer([&](Nodo* n) {
        file << n->getCedula() << " " << n->getNombre() << endl;
    });

    file.close();
}

// CARGAR
void Lista::cargarDatos(string archivo) {
    ifstream file(archivo);
    string c, n;

    while (file >> c >> n)
        insertarFinal(c, n);

    file.close();
}

//PROVINCIAS CON LAMBDA
string* Lista::cargarProvincias(string archivo) {
    string* nombres = new string[31];

    ifstream file(archivo);
    int cod;
    string nom;

    while (file >> cod >> nom) {
        *(nombres + cod) = nom;
    }

    file.close();
    return nombres;
}

void Lista::reporteProvincias(string* nombres) {
    int cont[31] = {0};

    recorrer([&](Nodo* n) {
        string c = n->getCedula();
        int prov = (c[0]-'0')*10 + (c[1]-'0');

        if ((prov >= 1 && prov <= 24) || prov == 30)
            cont[prov]++;
    });

    cout << "\n--- REPORTE ---\n";

    for (int i = 1; i <= 30; i++) {
        if (cont[i] > 0) {
            cout << nombres[i] << " -> " << cont[i] << endl;
        }
    }
}

//VALIDAR CEDULA CON LAMBDA INTERNA
bool Lista::esCedulaValida(string cedula) {

    auto esNumero = [](char c) {
        return isdigit(c);
    };

    if (cedula.length() != 10) return false;

    for (char c : cedula)
        if (!esNumero(c)) return false;

    int suma = 0;

    for (int i = 0; i < 9; i++) {
        int d = cedula[i] - '0';

        if (i % 2 == 0) {
            d *= 2;
            if (d > 9) d -= 9;
        }

        suma += d;
    }

    int ver = (suma % 10 == 0) ? 0 : 10 - (suma % 10);

    return ver == (cedula[9] - '0');
}

bool Lista::existeCedula(string c) {
    return buscar(c) != nullptr;
}

bool Lista::esNombreValido(string nombre) {
    regex patron("^[a-zA-ZáéíóúÁÉÍÓÚñÑ ]+$");
    return regex_match(nombre, patron);
}

// VOCALES CON LAMBDA
int Lista::contarVocales(string nombre) {

    auto esVocal = [](char l) {
        return (l=='a'||l=='e'||l=='i'||l=='o'||l=='u'||
                l=='A'||l=='E'||l=='I'||l=='O'||l=='U');
    };

    int cont = 0;

    for (char c : nombre)
        if (esVocal(c)) cont++;

    return cont;
}

// LETRAS CON LAMBDA
int Lista::contarLetras(string nombre) {

    auto esLetra = [](char c) {
        return c != ' ';
    };

    int cont = 0;

    for (char c : nombre)
        if (esLetra(c)) cont++;

    return cont;
}