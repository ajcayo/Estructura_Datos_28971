#include "Interfaz.h"
#include "CargadorDatos.h"
#include "Dijkstra.h"
#include "Renderizador.h"
#include <iostream>
#include <limits>

Interfaz::Interfaz() : grafo(nullptr) {
}

Interfaz::~Interfaz() {
    delete grafo; // libera todo el arbol de memoria dinamica del grafo
}

void Interfaz::mostrarMenu() const {
    std::cout << "\n================ CAMPUS NAV ESPE ================\n";
    std::cout << "1. Listar nodos cargados\n";
    std::cout << "2. Ver lista de adyacencia\n";
    std::cout << "3. Ver matriz de adyacencia\n";
    std::cout << "4. Ver mapa (ASCII)\n";
    std::cout << "5. Calcular ruta mas corta (Dijkstra)\n";
    std::cout << "6. Salir\n";
    std::cout << "===================================================\n";
    std::cout << "Opcion: ";
}

void Interfaz::opcionListarNodos() const {
    if (!grafo) return;
    std::cout << "\n--- NODOS CARGADOS ---\n";
    for (int i = 0; i < grafo->getCantidadNodos(); ++i) {
        std::cout << grafo->getNodoEnPosicion(i)->toString() << "\n";
    }
}

void Interfaz::opcionVerListaAdyacencia() const {
    if (!grafo) return;
    grafo->imprimirListaAdyacencia();
}

void Interfaz::opcionVerMatriz() const {
    if (!grafo) return;
    grafo->imprimirMatrizAdyacencia();
}

void Interfaz::opcionVerMapa() const {
    if (!grafo) return;
    Renderizador::dibujarMapa(*grafo);
}

void Interfaz::opcionCalcularRuta() const {
    if (!grafo) return;
    opcionListarNodos();

    int idOrigen, idDestino;
    std::cout << "\nId del nodo de ORIGEN: ";
    std::cin >> idOrigen;
    std::cout << "Id del nodo de DESTINO: ";
    std::cin >> idDestino;

    ResultadoRuta resultado = Dijkstra::calcularRuta(*grafo, idOrigen, idDestino);
    Dijkstra::imprimirRuta(resultado);
    if (resultado.encontrada) {
        Renderizador::dibujarRuta(*grafo, resultado);
    }
    Dijkstra::liberarRuta(resultado); // liberamos la lista enlazada del resultado
}

void Interfaz::iniciar() {
    std::cout << "Bienvenido al Sistema Inteligente de Navegacion - Campus ESPE Sangolqui\n";

    int totalDisponible = CargadorDatos::totalNodosDisponibles();
    int cantidad = 0;
    std::cout << "Hay " << totalDisponible << " puntos disponibles en el catalogo del campus.\n";
    std::cout << "Cuantos deseas cargar en el grafo? (1-" << totalDisponible << "): ";
    std::cin >> cantidad;

    if (cantidad < 1) cantidad = 1;
    if (cantidad > totalDisponible) cantidad = totalDisponible;

    grafo = CargadorDatos::construirGrafoESPE(cantidad);
    std::cout << "Grafo construido con " << grafo->getCantidadNodos() << " nodos.\n";

    int opcion = 0;
    do {
        mostrarMenu();
        std::cin >> opcion;

        switch (opcion) {
            case 1: opcionListarNodos(); break;
            case 2: opcionVerListaAdyacencia(); break;
            case 3: opcionVerMatriz(); break;
            case 4: opcionVerMapa(); break;
            case 5: opcionCalcularRuta(); break;
            case 6: std::cout << "Hasta luego.\n"; break;
            default: std::cout << "Opcion invalida.\n"; break;
        }
    } while (opcion != 6);
}
