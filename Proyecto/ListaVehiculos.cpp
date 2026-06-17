#include "ListaVehiculos.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

NodoVehiculo::NodoVehiculo(Vehiculo* v) { vehiculo = v; siguiente = nullptr; }
NodoVehiculo::~NodoVehiculo() { delete vehiculo; }
Vehiculo* NodoVehiculo::getVehiculo() { return vehiculo; }
NodoVehiculo* NodoVehiculo::getSiguiente() { return siguiente; }
void NodoVehiculo::setSiguiente(NodoVehiculo* sig) { siguiente = sig; }

ListaVehiculos::ListaVehiculos() { cabeza = nullptr; }
ListaVehiculos::~ListaVehiculos() {
    NodoVehiculo* actual = cabeza;
    while (actual != nullptr) {
        NodoVehiculo* borrar = actual;
        actual = actual->getSiguiente();
        delete borrar;
    }
}
void ListaVehiculos::crear(Vehiculo* v) {
    NodoVehiculo* nuevo = new NodoVehiculo(v);
    nuevo->setSiguiente(cabeza);
    cabeza = nuevo;
}

void ListaVehiculos::imprimir() const {
    NodoVehiculo* temp = cabeza;
    int pos = 1;
    while (temp != nullptr) {
        cout << "  [" << pos << "] Placa: " << temp->getVehiculo()->getPlaca() << "\n";
        temp = temp->getSiguiente();
        pos++;
    }
}

void ListaVehiculos::reportar() const {
    NodoVehiculo* temp = cabeza;
    if (!temp) { cout << "  No hay vehiculos registrados.\n"; return; }
    cout << "\n--- VEHICULOS REGISTRADOS ---\n";
    while (temp != nullptr) {
        cout << "  Placa: " << temp->getVehiculo()->getPlaca() << "\n";
        temp = temp->getSiguiente();
    }
}
Vehiculo* ListaVehiculos::buscar(string placa) const {
    NodoVehiculo* temp = cabeza;
    while (temp != nullptr) {
        if (temp->getVehiculo()->getPlaca() == placa) return temp->getVehiculo();
        temp = temp->getSiguiente();
    }
    return nullptr;
}
bool ListaVehiculos::actualizar(string placa, string nuevaPlaca) {
    Vehiculo* v = buscar(placa);
    if (v) { v->setPlaca(nuevaPlaca); return true; }
    return false;
}
bool ListaVehiculos::eliminar(string placa) {
    NodoVehiculo* actual = cabeza;
    NodoVehiculo* anterior = nullptr;
    while (actual != nullptr) {
        if (actual->getVehiculo()->getPlaca() == placa) {
            if (anterior == nullptr) cabeza = actual->getSiguiente();
            else anterior->setSiguiente(actual->getSiguiente());
            delete actual;
            return true;
        }
        anterior = actual;
        actual = actual->getSiguiente();
    }
    return false;
}
NodoVehiculo* ListaVehiculos::getCabeza() const { return cabeza; }

// ============================================================
//  INTERCAMBIO BIT A BIT usando XOR sobre cada caracter
// ============================================================
static void intercambiarStringsV(string& a, string& b) {
    while (a.size() < b.size()) a += '\0';
    while (b.size() < a.size()) b += '\0';

    string::iterator itA = a.begin();
    string::iterator itB = b.begin();
    while (itA != a.end()) {
        *itA = *itA ^ *itB;
        *itB = *itA ^ *itB;
        *itA = *itA ^ *itB;
        ++itA;
        ++itB;
    }
    while (!a.empty() && a.back() == '\0') a.pop_back();
    while (!b.empty() && b.back() == '\0') b.pop_back();
}

// ============================================================
//  ORDENAMIENTO POR INTERCAMBIO - Vehiculos por Placa
//  criterio: 0 = por placa (unico disponible)
// ============================================================
void ListaVehiculos::ordenamientoIntercambio(int criterio) {
    (void)criterio; // unico criterio: placa
    if (cabeza == nullptr || cabeza->getSiguiente() == nullptr) {
        cout << "  [!] Lista vacia o con un solo elemento, no hay que ordenar.\n";
        return;
    }

    bool intercambiado;
    NodoVehiculo* actual;
    NodoVehiculo* ultimoLeido = nullptr;
    int paso = 1;

    do {
        intercambiado = false;
        actual = cabeza;

        while (actual->getSiguiente() != ultimoLeido) {
            Vehiculo* vA = actual->getVehiculo();
            Vehiculo* vB = actual->getSiguiente()->getVehiculo();

            if (vA->getPlaca() > vB->getPlaca()) {

                // ── Intercambio bit a bit con XOR ──
                string pA = vA->getPlaca();
                string pB = vB->getPlaca();
                intercambiarStringsV(pA, pB);
                vA->setPlaca(pB);
                vB->setPlaca(pA);

                intercambiado = true;

                system("cls");
                cout << "==========================================================================" << endl;
                cout << "      ANIMACION EN VIVO - ORDENAR VEHICULOS POR PLACA - PASO #" << paso << endl;
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
    cout << "         VEHICULOS ORDENADOS POR PLACA - COMPLETADO                       " << endl;
    cout << "==========================================================================" << endl;
    imprimir();
}
