#include "Nodo.h"
#include <sstream>

std::string tipoNodoToString(TipoNodo tipo) {
    switch (tipo) {
        case TipoNodo::Edificio:    return "Edificio";
        case TipoNodo::Garita:      return "Garita";
        case TipoNodo::Entrada:     return "Entrada";
        case TipoNodo::Cruce:       return "Cruce";
        case TipoNodo::Escalera:    return "Escalera";
        case TipoNodo::Rampa:       return "Rampa";
        case TipoNodo::Parqueadero: return "Parqueadero";
        case TipoNodo::Cafeteria:   return "Cafeteria";
        case TipoNodo::Biblioteca:  return "Biblioteca";
        case TipoNodo::Laboratorio: return "Laboratorio";
        case TipoNodo::Auditorio:   return "Auditorio";
    }
    return "Desconocido";
}

Nodo::Nodo(int id, const std::string& nombre, TipoNodo tipo,
           double latitud, double longitud, bool accesible)
    : id(id), nombre(nombre), tipo(tipo),
      latitud(latitud), longitud(longitud), accesible(accesible) {
}

int Nodo::getId() const { return id; }
std::string Nodo::getNombre() const { return nombre; }
TipoNodo Nodo::getTipo() const { return tipo; }
double Nodo::getLatitud() const { return latitud; }
double Nodo::getLongitud() const { return longitud; }
bool Nodo::esAccesible() const { return accesible; }

std::string Nodo::toString() const {
    std::ostringstream oss;
    oss << "[" << id << "] " << nombre << " (" << tipoNodoToString(tipo) << ")"
        << " lat:" << latitud << " lon:" << longitud
        << " accesible:" << (accesible ? "si" : "no");
    return oss.str();
}
