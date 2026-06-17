#include <iostream>
#include <fstream>
#include <regex>
#include <cctype>
#include <thread>
#include <chrono>
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

bool Lista::esCedulaValida(string c) {
    if (c.length() != 10) return false;
    for (char ch : c) if (!isdigit(ch)) return false;
    return true;
}

int Lista::contarVocales(string s) {
    int cnt = 0;
    for (char c : s) if (string("aeiouAEIOU").find(c) != string::npos) cnt++;
    return cnt;
}

int Lista::contarLetras(string s) {
    int cnt = 0;
    for (char c : s) if (isalpha(c)) cnt++;
    return cnt;
}

void Lista::verPares() { cout << "verPares: no implementado\n"; }
string* Lista::cargarProvincias(string) { return nullptr; }
void Lista::reporteProvincias(string*) { cout << "reporteProvincias: no implementado\n"; }

// GENERAR CORREO
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

// ORDENAMIENTO BURBUJA (intercambio) con animacion
void Lista::ordenamientoburbuja() {
    if (cabeza == nullptr || cabeza->getSiguiente() == nullptr) {
        return;
    }

    bool intercambiado;
    Nodo* actual;
    Nodo* ultimoLeido = nullptr;
    int paso = 1;

    do {
        intercambiado = false;
        actual = cabeza;

        while (actual->getSiguiente() != ultimoLeido) {

            if (actual->getApellido() > actual->getSiguiente()->getApellido()) {

                // Intercambio de datos
                string tempCedula   = actual->getCedula();
                string tempNombre   = actual->getNombre();
                string tempApellido = actual->getApellido();

                actual->setCedula(actual->getSiguiente()->getCedula());
                actual->setNombre(actual->getSiguiente()->getNombre());
                actual->setApellido(actual->getSiguiente()->getApellido());

                actual->getSiguiente()->setCedula(tempCedula);
                actual->getSiguiente()->setNombre(tempNombre);
                actual->getSiguiente()->setApellido(tempApellido);

                intercambiado = true;

                system("cls");
                cout << "==========================================" << endl;
                cout << "  ANIMACION EN VIVO: PASO #" << paso << endl;
                cout << "==========================================" << endl;
                imprimir();
                paso++;
                this_thread::sleep_for(chrono::milliseconds(800));
            }

            actual = actual->getSiguiente();
        }

        ultimoLeido = actual;

    } while (intercambiado);

    system("cls");
    cout << "==========================================" << endl;
    cout << "      LISTA ORDENADA COMPLETAMENTE        " << endl;
    cout << "==========================================" << endl;
    imprimir();
}
