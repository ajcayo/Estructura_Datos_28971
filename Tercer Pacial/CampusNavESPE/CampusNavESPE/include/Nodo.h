#ifndef NODO_H
#define NODO_H

#include <string>

// Clasificacion de los puntos del campus.
enum class TipoNodo {
    Edificio,
    Garita,
    Entrada,
    Cruce,
    Escalera,
    Rampa,
    Parqueadero,
    Cafeteria,
    Biblioteca,
    Laboratorio,
    Auditorio
};

std::string tipoNodoToString(TipoNodo tipo);

// Representa un punto fisico del campus (vertice del grafo).
// No gestiona memoria dinamica propia: solo encapsula datos y su acceso.
class Nodo {
private:
    int id;
    std::string nombre;
    TipoNodo tipo;
    double latitud;
    double longitud;
    bool accesible;

public:
    Nodo(int id, const std::string& nombre, TipoNodo tipo,
         double latitud, double longitud, bool accesible);

    int getId() const;
    std::string getNombre() const;
    TipoNodo getTipo() const;
    double getLatitud() const;
    double getLongitud() const;
    bool esAccesible() const;

    std::string toString() const;

    ~Nodo() = default;
};

#endif // NODO_H
