#include "ListaUsuarios.h"
#include <iostream>

NodoUsuario::NodoUsuario(Usuario* u) { usuario = u; siguiente = nullptr; }
NodoUsuario::~NodoUsuario() { delete usuario; }
Usuario* NodoUsuario::getUsuario() { return usuario; }
NodoUsuario* NodoUsuario::getSiguiente() { return siguiente; }
void NodoUsuario::setSiguiente(NodoUsuario* sig) { siguiente = sig; }

ListaUsuarios::ListaUsuarios() { cabeza = nullptr; }
ListaUsuarios::~ListaUsuarios() {
    NodoUsuario* actual = cabeza;
    while (actual != nullptr) {
        NodoUsuario* borrar = actual;
        actual = actual->getSiguiente();
        delete borrar;
    }
}
void ListaUsuarios::crear(Usuario* u) {
    NodoUsuario* nuevo = new NodoUsuario(u);
    nuevo->setSiguiente(cabeza);
    cabeza = nuevo;
}
void ListaUsuarios::reportar() const {
    NodoUsuario* temp = cabeza;
    if (!temp) { std::cout << "  No hay usuarios registrados.\n"; return; }
    std::cout << "\n--- USUARIOS REGISTRADOS ---\n";
    while (temp != nullptr) {
        std::cout << "  CI: " << temp->getUsuario()->getCedula()
                  << " | " << temp->getUsuario()->getNombre()
                  << " " << temp->getUsuario()->getApellido() << "\n";
        temp = temp->getSiguiente();
    }
}
Usuario* ListaUsuarios::buscar(std::string cedula) const {
    NodoUsuario* temp = cabeza;
    while (temp != nullptr) {
        if (temp->getUsuario()->getCedula() == cedula) return temp->getUsuario();
        temp = temp->getSiguiente();
    }
    return nullptr;
}
bool ListaUsuarios::actualizar(std::string cedula, std::string nuevoNom, std::string nuevoApe) {
    Usuario* u = buscar(cedula);
    if (u) { u->setNombre(nuevoNom); u->setApellido(nuevoApe); return true; }
    return false;
}
bool ListaUsuarios::eliminar(std::string cedula) {
    NodoUsuario* actual = cabeza;
    NodoUsuario* anterior = nullptr;
    while (actual != nullptr) {
        if (actual->getUsuario()->getCedula() == cedula) {
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
NodoUsuario* ListaUsuarios::getCabeza() const { return cabeza; }