#include "Arista.h"
#include <sstream>

Arista::Arista(Nodo* origen, Nodo* destino, double distancia,
               const std::string& tipoCamino, bool accesible, bool abierta)
    : origen(origen), destino(destino), distancia(distancia),
      tipoCamino(tipoCamino), accesible(accesible), abierta(abierta) {
}

Nodo* Arista::getOrigen() const { return origen; }
Nodo* Arista::getDestino() const { return destino; }
double Arista::getDistancia() const { return distancia; }
std::string Arista::getTipoCamino() const { return tipoCamino; }
bool Arista::esAccesible() const { return accesible; }
bool Arista::estaAbierta() const { return abierta; }

void Arista::cerrar() { abierta = false; }
void Arista::abrir() { abierta = true; }

Nodo* Arista::obtenerVecino(int idNodoActual) const {
    if (origen->getId() == idNodoActual) return destino;
    if (destino->getId() == idNodoActual) return origen;
    return nullptr; // el nodo actual no es extremo de esta arista
}

std::string Arista::toString() const {
    std::ostringstream oss;
    oss << origen->getNombre() << " <-> " << destino->getNombre()
        << " (" << distancia << " m, " << tipoCamino
        << ", accesible:" << (accesible ? "si" : "no")
        << ", estado:" << (abierta ? "abierta" : "cerrada") << ")";
    return oss.str();
}
