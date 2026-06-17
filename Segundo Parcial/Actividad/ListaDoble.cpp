#include "ListaDoble.h"
#include <iostream>

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
    if (!actual) { std::cout << "  No hay turnos registrados.\n"; return; }
    std::cout << "\n--- TURNOS REGISTRADOS ---\n";
    while (actual != nullptr) {
        Reserva* res = actual->getReserva();
        std::tm f = res->getFechaAsignada();
        std::cout << "  Propietario: " << res->getUsuario()->getNombre()
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
Reserva* ListaDoble::buscarPorPlaca(std::string placa) const {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        if (actual->getReserva()->getVehiculo()->getPlaca() == placa)
            return actual->getReserva();
        actual = actual->getSiguiente();
    }
    return nullptr;
}
bool ListaDoble::eliminarReserva(std::string placa) {
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
void ListaDoble::eliminarCascadaUsuario(std::string cedula) {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        Nodo* siguiente = actual->getSiguiente();
        if (actual->getReserva()->getUsuario()->getCedula() == cedula)
            eliminarReserva(actual->getReserva()->getVehiculo()->getPlaca());
        actual = siguiente;
    }
}
void ListaDoble::eliminarCascadaVehiculo(std::string placa) {
    eliminarReserva(placa);
}
Nodo* ListaDoble::getCabeza() const { return cabeza; }