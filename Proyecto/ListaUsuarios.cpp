#include "ListaUsuarios.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;


NodoUsuario::NodoUsuario(Usuario* u) { usuario = u; siguiente = nullptr; }
NodoUsuario::~NodoUsuario()          { delete usuario; }
Usuario*     NodoUsuario::getUsuario()              { return usuario; }
NodoUsuario* NodoUsuario::getSiguiente()            { return siguiente; }
void         NodoUsuario::setSiguiente(NodoUsuario* s){ siguiente = s; }


ListaUsuarios::ListaUsuarios() { cabeza = nullptr; }
ListaUsuarios::~ListaUsuarios() {
    NodoUsuario* a = cabeza;
    while (a) { NodoUsuario* b = a; a = a->getSiguiente(); delete b; }
}

void ListaUsuarios::crear(Usuario* u) {
    NodoUsuario* nuevo = new NodoUsuario(u);
    nuevo->setSiguiente(cabeza);
    cabeza = nuevo;
}


void ListaUsuarios::imprimir() const {
    NodoUsuario* tmp = cabeza;
    int pos = 1;
    while (tmp != nullptr) {
        cout << "  [" << pos << "] "
             << tmp->getUsuario()->getApellido()
             << ", "  << tmp->getUsuario()->getNombre()
             << "  (CI: " << tmp->getUsuario()->getCedula() << ")\n";
        tmp = tmp->getSiguiente();
        pos++;
    }
}

void ListaUsuarios::reportar() const {
    NodoUsuario* tmp = cabeza;
    if (!tmp) { cout << "  No hay usuarios registrados.\n"; return; }
    cout << "\n--- USUARIOS REGISTRADOS ---\n";
    imprimir();
}


Usuario* ListaUsuarios::buscar(string cedula) const {
    NodoUsuario* tmp = cabeza;
    while (tmp) {
        if (tmp->getUsuario()->getCedula() == cedula) return tmp->getUsuario();
        tmp = tmp->getSiguiente();
    }
    return nullptr;
}


Usuario* ListaUsuarios::buscarConExcepcion(string cedula) const {
    try {
        NodoUsuario* tmp = cabeza;
        while (tmp) {
            if (tmp->getUsuario()->getCedula() == cedula)
                return tmp->getUsuario();
            tmp = tmp->getSiguiente();
        }
        throw runtime_error("Usuario con CI [" + cedula + "] no encontrado en el sistema.");
    } catch (const runtime_error& e) {
        cout << "\n  [EXCEPCION CAPTURADA] " << e.what() << "\n";
        return nullptr;
    }
}

bool ListaUsuarios::actualizar(string cedula, string nuevoNom, string nuevoApe) {
    Usuario* u = buscar(cedula);
    if (u) {
        u->setNombre(nuevoNom);
        u->setApellido(nuevoApe);
        return true;
    }
    return false;
}

bool ListaUsuarios::eliminar(string cedula) {
    NodoUsuario* actual   = cabeza;
    NodoUsuario* anterior = nullptr;
    while (actual) {
        if (actual->getUsuario()->getCedula() == cedula) {
            if (!anterior) cabeza = actual->getSiguiente();
            else anterior->setSiguiente(actual->getSiguiente());
            delete actual;
            return true;
        }
        anterior = actual;
        actual   = actual->getSiguiente();
    }
    return false;
}

NodoUsuario* ListaUsuarios::getCabeza() const { return cabeza; }

// ============================================================
//  INTERCAMBIO BIT A BIT usando XOR sobre cada caracter
//  Requisito: una sola funcion, sin vector, sin corchetes,
//             sin variable temporal de string -> usa XOR char a char
// ============================================================
static void intercambiarStrings(string& a, string& b) {
    // Igualar longitudes con padding \0
    while (a.size() < b.size()) a += '\0';
    while (b.size() < a.size()) b += '\0';

    // XOR bit a bit caracter por caracter (sin corchetes: iteradores)
    string::iterator itA = a.begin();
    string::iterator itB = b.begin();
    while (itA != a.end()) {
        *itA = *itA ^ *itB;
        *itB = *itA ^ *itB;
        *itA = *itA ^ *itB;
        ++itA;
        ++itB;
    }
    // Eliminar padding \0 al final
    while (!a.empty() && a.back() == '\0') a.pop_back();
    while (!b.empty() && b.back() == '\0') b.pop_back();
}

// ============================================================
//  ORDENAMIENTO POR INTERCAMBIO con animacion en vivo
//  criterio: 0 = por apellido, 1 = por nombre
// ============================================================
void ListaUsuarios::ordenamientoIntercambio(int criterio) {
    if (cabeza == nullptr || cabeza->getSiguiente() == nullptr) {
        cout << "  [!] Lista vacia o con un solo elemento, no hay que ordenar.\n";
        return;
    }

    string criterioNombre = (criterio == 1) ? "NOMBRE" : "APELLIDO";

    bool intercambiado;
    NodoUsuario* actual;
    NodoUsuario* ultimoLeido = nullptr;
    int paso = 1;

    do {
        intercambiado = false;
        actual = cabeza;

        while (actual->getSiguiente() != ultimoLeido) {
            Usuario* uA = actual->getUsuario();
            Usuario* uB = actual->getSiguiente()->getUsuario();

            // Obtener clave de comparacion segun criterio (sin corchetes)
            string claveA = (criterio == 1) ? uA->getNombre() : uA->getApellido();
            string claveB = (criterio == 1) ? uB->getNombre() : uB->getApellido();

            if (claveA > claveB) {

                // ── Intercambio bit a bit con XOR (sin variable temporal) ──
                string cedA = uA->getCedula();
                string nomA = uA->getNombre();
                string apeA = uA->getApellido();
                string cedB = uB->getCedula();
                string nomB = uB->getNombre();
                string apeB = uB->getApellido();

                intercambiarStrings(cedA, cedB);
                intercambiarStrings(nomA, nomB);
                intercambiarStrings(apeA, apeB);

                uA->setCedula(cedB);
                uA->setNombre(nomB);
                uA->setApellido(apeB);
                uB->setCedula(cedA);
                uB->setNombre(nomA);
                uB->setApellido(apeA);

                intercambiado = true;

                system("cls");
                cout << "==========================================================================" << endl;
                cout << "      ANIMACION EN VIVO - ORDENAR POR " << criterioNombre << " - PASO #" << paso << endl;
                cout << "==========================================================================" << endl;
                imprimir();
                paso++;
                this_thread::sleep_for(chrono::milliseconds(800));
            }

            actual = actual->getSiguiente();
        }
        ultimoLeido = actual;

    } while (intercambiado);

    system("cls");
    cout << "==========================================================================" << endl;
    cout << "         USUARIOS ORDENADOS POR " << criterioNombre << " - COMPLETADO          " << endl;
    cout << "==========================================================================" << endl;
    imprimir();
}
