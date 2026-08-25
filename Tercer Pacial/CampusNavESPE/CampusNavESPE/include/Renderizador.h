#ifndef RENDERIZADOR_H
#define RENDERIZADOR_H

#include "Grafo.h"
#include "Dijkstra.h"

// Responsable UNICAMENTE de mostrar informacion visual en consola.
// No calcula rutas ni conoce Dijkstra internamente: recibe resultados ya
// calculados y solo los dibuja. Esto respeta la separacion entre
// logica del grafo y renderizado pedida en los requisitos del proyecto.
class Renderizador {
public:
    static void dibujarMapa(const Grafo& grafo);
    static void dibujarRuta(const Grafo& grafo, const ResultadoRuta& ruta);
};

#endif // RENDERIZADOR_H
