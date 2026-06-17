#include "ListaDoble.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

ListaDoble::ListaDoble() { cabeza = nullptr; cola = nullptr; }
ListaDoble::~ListaDoble() {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        Nodo* aBorrar = actual;
        actual = actual->getSiguiente();
        delete aBorrar;
    }
}
void ListaDoble::agregarReserva(Reserva* r) {
    Nodo* nuevoNodo = new Nodo(r);
    if (cabeza == nullptr) { cabeza = nuevoNodo; cola = nuevoNodo; }
    else { cola->setSiguiente(nuevoNodo); nuevoNodo->setAnterior(cola); cola = nuevoNodo; }
}
void ListaDoble::mostrarReservas() const {
    Nodo* actual = cabeza;
    if (!actual) { cout << "  No hay turnos registrados.\n"; return; }
    cout << "\n--- TURNOS REGISTRADOS ---\n";
    while (actual != nullptr) {
        Reserva* res = actual->getReserva();
        tm f = res->getFechaAsignada();
        cout << "  Propietario: " << res->getUsuario()->getNombre()
             << " " << res->getUsuario()->getApellido()
             << " | CI: " << res->getUsuario()->getCedula() << "\n"
             << "  Vehiculo: " << res->getVehiculo()->getPlaca() << "\n"
             << "  Fecha turno: " << f.tm_mday << "/"
             << (f.tm_mon + 1) << "/" << (f.tm_year + 1900)
             << " a las " << res->getHoraAsignada() << ":00\n"
             << "  ----------------------------\n";
        actual = actual->getSiguiente();
    }
}
Reserva* ListaDoble::buscarPorPlaca(string placa) const {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        if (actual->getReserva()->getVehiculo()->getPlaca() == placa)
            return actual->getReserva();
        actual = actual->getSiguiente();
    }
    return nullptr;
}
bool ListaDoble::eliminarReserva(string placa) {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        if (actual->getReserva()->getVehiculo()->getPlaca() == placa) {
            if (actual->getAnterior()) actual->getAnterior()->setSiguiente(actual->getSiguiente());
            else cabeza = actual->getSiguiente();
            if (actual->getSiguiente()) actual->getSiguiente()->setAnterior(actual->getAnterior());
            else cola = actual->getAnterior();
            delete actual;
            return true;
        }
        actual = actual->getSiguiente();
    }
    return false;
}
void ListaDoble::eliminarCascadaUsuario(string cedula) {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        Nodo* siguiente = actual->getSiguiente();
        if (actual->getReserva()->getUsuario()->getCedula() == cedula)
            eliminarReserva(actual->getReserva()->getVehiculo()->getPlaca());
        actual = siguiente;
    }
}
void ListaDoble::eliminarCascadaVehiculo(string placa) {
    eliminarReserva(placa);
}
Nodo* ListaDoble::getCabeza() const { return cabeza; }

// ============================================================
//  IMPRESION INTERNA para la animacion
// ============================================================
void ListaDoble::imprimir() const {
    Nodo* actual = cabeza;
    int pos = 1;
    while (actual != nullptr) {
        Reserva* res = actual->getReserva();
        tm f = res->getFechaAsignada();
        cout << "  [" << pos << "] "
             << res->getUsuario()->getApellido() << ", "
             << res->getUsuario()->getNombre()
             << " | Placa: " << res->getVehiculo()->getPlaca()
             << " | Fecha: " << f.tm_mday << "/" << (f.tm_mon+1) << "/" << (f.tm_year+1900)
             << " | Hora: " << res->getHoraAsignada() << ":00\n";
        actual = actual->getSiguiente();
        pos++;
    }
}

// ============================================================
//  INTERCAMBIO BIT A BIT usando XOR sobre cada caracter
// ============================================================
static void intercambiarStringsR(string& a, string& b) {
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

// Convierte tm a entero comparable YYYYMMDD
static int fechaAEntero(const tm& f) {
    return ((f.tm_year + 1900) * 10000) + ((f.tm_mon + 1) * 100) + f.tm_mday;
}

// Intercambia todos los datos entre dos nodos de reserva (bit a bit para strings)
static void intercambiarReservas(Nodo* nA, Nodo* nB) {
    Reserva* rA = nA->getReserva();
    Reserva* rB = nB->getReserva();

    // --- Strings: intercambio XOR bit a bit ---
    string cedulaA   = rA->getUsuario()->getCedula();
    string nombreA   = rA->getUsuario()->getNombre();
    string apellidoA = rA->getUsuario()->getApellido();
    string placaA    = rA->getVehiculo()->getPlaca();

    string cedulaB   = rB->getUsuario()->getCedula();
    string nombreB   = rB->getUsuario()->getNombre();
    string apellidoB = rB->getUsuario()->getApellido();
    string placaB    = rB->getVehiculo()->getPlaca();

    intercambiarStringsR(cedulaA,   cedulaB);
    intercambiarStringsR(nombreA,   nombreB);
    intercambiarStringsR(apellidoA, apellidoB);
    intercambiarStringsR(placaA,    placaB);

    rA->getUsuario()->setCedula(cedulaB);
    rA->getUsuario()->setNombre(nombreB);
    rA->getUsuario()->setApellido(apellidoB);
    rA->getVehiculo()->setPlaca(placaB);

    rB->getUsuario()->setCedula(cedulaA);
    rB->getUsuario()->setNombre(nombreA);
    rB->getUsuario()->setApellido(apellidoA);
    rB->getVehiculo()->setPlaca(placaA);

    // --- Fecha: intercambio XOR bit a bit sobre los bytes de tm ---
    tm fechaA = rA->getFechaAsignada();
    tm fechaB = rB->getFechaAsignada();

    unsigned char* pA = reinterpret_cast<unsigned char*>(&fechaA);
    unsigned char* pB = reinterpret_cast<unsigned char*>(&fechaB);
    for (int i = 0; i < (int)sizeof(tm); i++) {
        pA[i] = pA[i] ^ pB[i];
        pB[i] = pA[i] ^ pB[i];
        pA[i] = pA[i] ^ pB[i];
    }
    rA->setFechaAsignada(fechaB);
    rB->setFechaAsignada(fechaA);

    // --- Hora: intercambio XOR bit a bit sobre int ---
    int horaA = rA->getHoraAsignada();
    int horaB = rB->getHoraAsignada();
    horaA = horaA ^ horaB;
    horaB = horaA ^ horaB;
    horaA = horaA ^ horaB;
    rA->setHoraAsignada(horaB);
    rB->setHoraAsignada(horaA);
}

// ============================================================
//  ORDENAMIENTO POR INTERCAMBIO - Reservas
//  criterio: 0 = por fecha (YYYYMMDD), 1 = por hora
// ============================================================
void ListaDoble::ordenamientoIntercambio(int criterio) {
    if (cabeza == nullptr || cabeza->getSiguiente() == nullptr) {
        cout << "  [!] Lista vacia o con un solo elemento, no hay que ordenar.\n";
        return;
    }

    string criterioNombre = (criterio == 1) ? "HORA" : "FECHA";

    bool intercambiado;
    Nodo* actual;
    Nodo* ultimoLeido = nullptr;
    int paso = 1;

    do {
        intercambiado = false;
        actual = cabeza;

        while (actual->getSiguiente() != ultimoLeido) {
            Reserva* rA = actual->getReserva();
            Reserva* rB = actual->getSiguiente()->getReserva();

            bool hayQueCambiar = false;
            if (criterio == 1) {
                // Comparar por hora
                hayQueCambiar = (rA->getHoraAsignada() > rB->getHoraAsignada());
            } else {
                // Comparar por fecha
                hayQueCambiar = (fechaAEntero(rA->getFechaAsignada()) >
                                 fechaAEntero(rB->getFechaAsignada()));
            }

            if (hayQueCambiar) {
                intercambiarReservas(actual, actual->getSiguiente());
                intercambiado = true;

                system("cls");
                cout << "==========================================================================" << endl;
                cout << "   ANIMACION EN VIVO - ORDENAR RESERVAS POR " << criterioNombre << " - PASO #" << paso << endl;
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
    cout << "         RESERVAS ORDENADAS POR " << criterioNombre << " - COMPLETADO               " << endl;
    cout << "==========================================================================" << endl;
    imprimir();
}
