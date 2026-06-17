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

void Lista::imprimir() {
    Nodo* aux = cabeza;

    if (aux == nullptr) {
        cout << "Lista vacia\n";
        return;
    }

    while (aux != nullptr) {
        cout << aux->getCedula() << " - " << aux->getNombre() << endl;
        aux = aux->getSiguiente();
    }
}

Nodo* Lista::buscar(string cedula) {
    Nodo* aux = cabeza;

    while (aux != nullptr) {
        if (aux->getCedula() == cedula) return aux;
        aux = aux->getSiguiente();
    }
    return nullptr;
}

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

void Lista::guardarDatos(string archivo) {
    ofstream file(archivo);

    Nodo* aux = cabeza;
    while (aux != nullptr) {
        file << aux->getCedula() << " " << aux->getNombre() << endl;
        aux = aux->getSiguiente();
    }

    file.close();
}

void Lista::cargarDatos(string archivo) {
    ifstream file(archivo);

    string c, n;
    while (file >> c >> n) {
        insertarFinal(c, n);
    }

    file.close();
}

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
    int* cont = new int[31]();

    Nodo* aux = cabeza;

    while (aux != nullptr) {
        string c = aux->getCedula();

        int prov = ((c[0]-'0') << 3) + ((c[0]-'0') << 1) + (c[1]-'0'); // x10 con bits

        if ((prov >= 1 && prov <= 24) || prov == 30)
            (*(cont + prov))++;

        aux = aux->getSiguiente();
    }

    cout << "\n--- REPORTE ---\n";
    for (int i = 1; i <= 30; i++) {
        if (*(cont + i) > 0)
            cout << *(nombres + i) << " -> " << *(cont + i) << endl;
    }

    delete[] cont;
}

// VALIDACIONES BIT A BIT
bool Lista::existeCedula(string c) {
    return buscar(c) != nullptr;
}

bool Lista::esNombreValido(string nombre) {
    regex patron("^[a-zA-ZáéíóúÁÉÍÓÚñÑ ]+$");
    return regex_match(nombre, patron);
}

bool Lista::esCedulaValida(string cedula) {
    if (cedula.length() != 10) return false;

    for (char c : cedula)
        if (!isdigit(c)) return false;

    int suma = 0;

    for (int i = 0; i < 9; i++) {
        int d = cedula[i] - '0';

        if ((i & 1) == 0) {   // BIT A BIT
            d <<= 1;          // multiplicar por 2
            if (d > 9) d -= 9;
        }

        suma += d;
    }

    int ver = (suma % 10 == 0) ? 0 : 10 - (suma % 10);

    return ver == (cedula[9] - '0');
}

// VOCALES BIT A BIT
int Lista::contarVocales(string nombre) {
    int c = 0;
    const char* ptr = nombre.c_str();

    while (*ptr) {
        char l = (*ptr | 32); // convierte a minúscula con bits

        if (l=='a'||l=='e'||l=='i'||l=='o'||l=='u')
            c++;

        ptr++;
    }
    return c;
}

int Lista::contarLetras(string nombre) {
    int c = 0;
    const char* ptr = nombre.c_str();

    while (*ptr) {
        if (*ptr != ' ') c++;
        ptr++;
    }
    return c;
}
void Lista::verPares() {

    Nodo* nodoActual = cabeza; 
    // puntero que apunta al primer nodo de la lista

    cout << "\nPares encontrados:\n"; 
    // mensaje en pantalla

    while (nodoActual != nullptr) {
    // recorre toda la lista hasta el final

        auto obtenerPares = [](string texto) {
        // función lambda: recibe la cedula como string

            string salida = "";
            // aqui se guardan los numeros pares

            const char* puntero = texto.c_str();
            // convierte el string a puntero de caracteres

            while (*puntero != '\0') {
            // recorre caracter por caracter hasta el final

                int digito = *puntero - '0';
                // convierte el caracter en numero

                if (digito % 2 == 0 && digito != 0) {
                    // si es par y no es 0
                    salida += *puntero;
                    // lo agrega al resultado
                }
                else if (digito == 0) {
                    // caso especial del 0 (tambien es par)
                    salida += '0';
                }

                puntero++;
                // avanza al siguiente caracter
            }

            return salida;
            // devuelve solo los numeros pares
        };

        string resultado = obtenerPares(nodoActual->getCedula());
        // aplica la funcion a la cedula del nodo actual

        cout << "Cedula: " << nodoActual->getCedula()
             << " -> " << resultado << endl;
        // imprime la cedula original y su version filtrada

        nodoActual = nodoActual->getSiguiente();
        // pasa al siguiente nodo de la lista
    }
}