#include "Usuario.h"

Usuario::Usuario(std::string nom, std::string ape, std::string ced) {
    nombre = nom;
    apellido = ape;
    cedula = ced;
}

std::string Usuario::getNombre() const {
     return nombre;
     }
std::string Usuario::getApellido() const {
     return apellido;
     }
std::string Usuario::getCedula() const {
     return cedula; 
    }

void Usuario::setNombre(std::string& nom) { 
    nombre = nom;
 }
void Usuario::setApellido(std::string& ape) { 
    apellido = ape; 
 }
void Usuario::setCedula(std::string& ced) { 
    cedula = ced; 
 }