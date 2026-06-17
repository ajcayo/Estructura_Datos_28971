#include "ListaVehiculos.h"
#include <iostream>

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
void ListaVehiculos::reportar() const {
    NodoVehiculo* temp = cabeza;
    if (!temp) { std::cout << "  No hay vehiculos registrados.\n"; return; }
    std::cout << "\n--- VEHICULOS REGISTRADOS ---\n";
    while (temp != nullptr) {
        std::cout << "  Placa: " << temp->getVehiculo()->getPlaca() << "\n";
        temp = temp->getSiguiente();
    }
}
Vehiculo* ListaVehiculos::buscar(std::string placa) const {
    NodoVehiculo* temp = cabeza;
    while (temp != nullptr) {
        if (temp->getVehiculo()->getPlaca() == placa) return temp->getVehiculo();
        temp = temp->getSiguiente();
    }
    return nullptr;
}
bool ListaVehiculos::actualizar(std::string placa, std::string nuevaPlaca) {
    Vehiculo* v = buscar(placa);
    if (v) { v->setPlaca(nuevaPlaca); return true; }
    return false;
}
bool ListaVehiculos::eliminar(std::string placa) {
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