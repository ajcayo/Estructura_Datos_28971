#ifndef INTERFAZ_H
#define INTERFAZ_H

#include "Grafo.h"

// Responsable UNICAMENTE del menu y la interaccion con el usuario por consola.
// No conoce Dijkstra directamente en su interior mas alla de invocarlo como
// servicio; no conoce detalles internos del Grafo (usa su API publica).
class Interfaz {
private:
    Grafo* grafo; // el grafo activo de la sesion (memoria dinamica, gestionada aqui)

    void mostrarMenu() const;
    void opcionVerListaAdyacencia() const;
    void opcionVerMatriz() const;
    void opcionVerMapa() const;
    void opcionCalcularRuta() const;
    void opcionListarNodos() const;

public:
    Interfaz();
    ~Interfaz();

    // Sin copia: posee un Grafo* dinamico.
    Interfaz(const Interfaz&) = delete;
    Interfaz& operator=(const Interfaz&) = delete;

    void iniciar();
};

#endif // INTERFAZ_H
