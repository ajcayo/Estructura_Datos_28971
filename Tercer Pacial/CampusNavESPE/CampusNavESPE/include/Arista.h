#ifndef ARISTA_H
#define ARISTA_H

#include "Nodo.h"
#include <string>

// Representa una conexion entre dos Nodo (arista del grafo).
// No es dueña de los Nodo* que referencia (esos los posee Grafo).
class Arista {
private:
    Nodo* origen;
    Nodo* destino;
    double distancia;        // metros, calculada con formula de Haversine
    std::string tipoCamino;  // "peatonal", "vehicular", "escalera", "rampa"...
    bool accesible;
    bool abierta;

public:
    Arista(Nodo* origen, Nodo* destino, double distancia,
           const std::string& tipoCamino, bool accesible, bool abierta = true);

    Nodo* getOrigen() const;
    Nodo* getDestino() const;
    double getDistancia() const;
    std::string getTipoCamino() const;
    bool esAccesible() const;
    bool estaAbierta() const;

    void cerrar();
    void abrir();

    // Dado el id de uno de los extremos, devuelve el Nodo* del otro extremo.
    // Permite reutilizar una unica Arista para representar ambos sentidos
    // en un grafo no dirigido, sin duplicar el objeto.
    Nodo* obtenerVecino(int idNodoActual) const;

    std::string toString() const;

    ~Arista() = default;
};

#endif // ARISTA_H
