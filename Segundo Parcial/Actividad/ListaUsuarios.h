#pragma once
#include "Usuario.h"

class NodoUsuario {
private:
    Usuario* usuario;
    NodoUsuario* siguiente;
public:
    NodoUsuario(Usuario* u);
    ~NodoUsuario();
    Usuario* getUsuario();
    NodoUsuario* getSiguiente();
    void setSiguiente(NodoUsuario* sig);
};

class ListaUsuarios {
private:
    NodoUsuario* cabeza;
public:
    ListaUsuarios();
    ~ListaUsuarios();
    void crear(Usuario* u);
    void reportar() const;
    Usuario* buscar(std::string cedula) const;
    bool actualizar(std::string cedula, std::string nuevoNom, std::string nuevoApe);
    bool eliminar(std::string cedula);


    NodoUsuario* getCabeza() const;
};