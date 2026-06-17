#include <iostream>
#include <fstream>
#include <regex>
#include <cctype>
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

// INSERTAR INICIO
void Lista::insertarCabeza(string cedula, string nombre, string apellido) {
    Nodo* nuevo = new Nodo(cedula, nombre, apellido);

    if (cabeza == nullptr) {
        cabeza = nuevo;
        cola = nuevo;
    } else {
        nuevo->setSiguiente(cabeza);
        cabeza = nuevo;
    }
}

// INSERTAR FINAL
void Lista::insertarFinal(string cedula, string nombre, string apellido) {
    Nodo* nuevo = new Nodo(cedula, nombre, apellido);

    if (cabeza == nullptr) {
        cabeza = nuevo;
        cola = nuevo;
    } else {
        cola->setSiguiente(nuevo);
        cola = nuevo;
    }
}

// INSERTAR ENTRE
void Lista::insertarEntre(string ref, string cedula, string nombre, string apellido) {
    Nodo* actual = cabeza;

    while (actual != nullptr && actual->getCedula() != ref) {
        actual = actual->getSiguiente();
    }

    if (actual != nullptr) {
        Nodo* nuevo = new Nodo(cedula, nombre, apellido);

        nuevo->setSiguiente(actual->getSiguiente());
        actual->setSiguiente(nuevo);

        if (nuevo->getSiguiente() == nullptr) {
            cola = nuevo;
        }

        cout << "Insertado entre nodos\n";
    } else {
        cout << "No se encontro referencia\n";
    }
}

// IMPRIMIR
void Lista::imprimir() {
    Nodo* aux = cabeza;

    if (aux == nullptr) {
        cout << "Lista vacia\n";
        return;
    }

    while (aux != nullptr) {
        cout << aux->getCedula() << " - "
             << aux->getNombre() << " "
             << aux->getApellido() << endl;
        aux = aux->getSiguiente();
    }
}

// BUSCAR
Nodo* Lista::buscar(string cedula) {
    Nodo* aux = cabeza;

    while (aux != nullptr) {
        if (aux->getCedula() == cedula) return aux;
        aux = aux->getSiguiente();
    }
    return nullptr;
}

// ELIMINAR
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

// GUARDAR
void Lista::guardarDatos(string archivo) {
    ofstream file(archivo);

    Nodo* aux = cabeza;
    while (aux != nullptr) {
        file << aux->getCedula() << " "
             << aux->getNombre() << " "
             << aux->getApellido() << endl;
        aux = aux->getSiguiente();
    }

    file.close();
}

// CARGAR
void Lista::cargarDatos(string archivo) {
    ifstream file(archivo);

    string c, n, a;
    while (file >> c >> n >> a) {
        insertarFinal(c, n, a);
    }

    file.close();
}

// VALIDACIONES
bool Lista::existeCedula(string c) {
    return buscar(c) != nullptr;
}

bool Lista::esNombreValido(string nombre) {
    regex patron("^[a-zA-ZáéíóúÁÉÍÓÚñÑ ]+$");
    return regex_match(nombre, patron);
}
// Generar Correo 
void Lista::generarCorreos() {
    ofstream file("correos.txt");

    map<string, int> contador;

    function<void(Nodo*)> recorrer = [&](Nodo* nodo) {

        if (nodo == nullptr) return;

        string nombre = nodo->getNombre();
        string apellido = nodo->getApellido();

        for (auto &c : nombre) c = tolower(c);
        for (auto &c : apellido) c = tolower(c);

        string correo = string(1, nombre[0]) + apellido;

        contador[correo]++;
        if (contador[correo] > 1) {
            correo += to_string(contador[correo]);
        }

        correo += "@turnos.ecuador.com";

        file << nodo->getCedula() << " -> " << correo << endl;

        recorrer(nodo->getSiguiente());
    };

    recorrer(cabeza);

    file.close();

    cout << "Correos generados en correos.txt\n";
}