#include <iostream>
#include <limits>
#include "Grafo.h"

// Construye el grafo con lugares reales (aproximados) del
// Campus Matriz de la Universidad de las Fuerzas Armadas - ESPE, Sangolqui.
// Direccion base: Av. General Ruminahui s/n y Ambato, Sangolqui - Ecuador.
Grafo construirCampusESPE() {
    const int NUM_LUGARES = 17;
    Grafo g(NUM_LUGARES);

    g.agregarLugar(0,  "Puerta Principal (Av. Gral. Ruminahui)", -0.317000, -78.444200);
    g.agregarLugar(1,  "Edificio de Rectorado",                  -0.316500, -78.443800);
    g.agregarLugar(2,  "Biblioteca Alejandro Segovia",            -0.316200, -78.443200);
    g.agregarLugar(3,  "Coliseo Miguel G. Iturralde Jaramillo",   -0.317800, -78.442500);
    g.agregarLugar(4,  "Facultad de Ciencias de la Computacion",  -0.315500, -78.442800);
    g.agregarLugar(5,  "Facultad de Ciencias de la Energia y Mecanica", -0.315000, -78.442000);
    g.agregarLugar(6,  "Facultad de Ciencias Economicas Admin. y Comercio", -0.316800, -78.442000);
    g.agregarLugar(7,  "Facultad Electrica, Electronica y Telecom.", -0.315800, -78.441500);
    g.agregarLugar(8,  "Facultad de Ciencias Exactas",            -0.317200, -78.441200);
    g.agregarLugar(9,  "Facultad de Ciencias de la Tierra y Construccion", -0.314500, -78.443200);
    g.agregarLugar(10, "Facultad de Ciencias Humanas y Sociales", -0.318000, -78.443500);
    g.agregarLugar(11, "Departamento de Ciencias Medicas",        -0.318500, -78.441800);
    g.agregarLugar(12, "Bienestar Estudiantil",                   -0.316000, -78.444000);
    g.agregarLugar(13, "Canchas / Areas Deportivas",               -0.318300, -78.443000);
    g.agregarLugar(14, "Parqueadero Central",                     -0.317500, -78.444500);
    g.agregarLugar(15, "Auditorio Fuerzas Armadas",                -0.316300, -78.442500);
    g.agregarLugar(16, "Comedor Universitario",                    -0.316800, -78.443000);

    // Conexiones (caminos/veredas reales del campus) - grafo NO DIRIGIDO
    int conexiones[][2] = {
        {0,14}, {0,12}, {14,1}, {12,1}, {12,16},
        {1,2}, {1,6}, {2,15}, {2,4}, {6,16},
        {6,7}, {16,15}, {15,4}, {15,10}, {4,9},
        {4,5}, {5,7}, {7,8}, {9,5}, {10,13},
        {10,11}, {13,11}, {13,3}, {3,8}, {8,11},
        {6,10}, {16,13}, {2,9}
    };
    for (auto& c : conexiones) g.agregarArista(c[0], c[1]);

    return g;
}

void pausar() {
    std::cout << "\nPresiona ENTER para continuar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

int main() {
    Grafo campus = construirCampusESPE();
    int opcion = -1;

    do {
        std::cout << "\n==========================================================\n";
        std::cout << "  RUTA OPTIMA - CAMPUS ESPE MATRIZ SANGOLQUI (Dijkstra)\n";
        std::cout << "==========================================================\n";
        std::cout << "1. Ver lista de lugares (nodos)\n";
        std::cout << "2. Ver matriz de adyacencia\n";
        std::cout << "3. Ver listas de adyacencia\n";
        std::cout << "4. Calcular mejor ruta entre dos lugares (genera mapa.html)\n";
        std::cout << "5. Salir\n";
        std::cout << "Opcion: ";
        std::cin >> opcion;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrada invalida.\n";
            continue;
        }

        switch (opcion) {
            case 1:
                campus.imprimirLugares();
                pausar();
                break;
            case 2:
                campus.imprimirMatriz();
                pausar();
                break;
            case 3:
                campus.imprimirListas();
                pausar();
                break;
            case 4: {
                campus.imprimirLugares();
                int origen, destino;
                std::cout << "\nID de lugar de ORIGEN: ";
                std::cin >> origen;
                std::cout << "ID de lugar de DESTINO: ";
                std::cin >> destino;

                if (origen < 0 || origen >= campus.totalLugares() ||
                    destino < 0 || destino >= campus.totalLugares()) {
                    std::cout << "ID invalido.\n";
                    break;
                }

                double distancia = 0.0;
                std::vector<int> camino = campus.dijkstra(origen, destino, distancia);

                if (camino.empty()) {
                    std::cout << "\nNo existe una ruta entre esos dos lugares.\n";
                } else {
                    std::cout << "\n--- MEJOR RUTA ENCONTRADA (Dijkstra) ---\n";
                    for (size_t i = 0; i < camino.size(); i++) {
                        std::cout << camino[i] << ". " << campus.lugar(camino[i]).nombre;
                        if (i + 1 < camino.size()) std::cout << "  ->  ";
                    }
                    std::cout << "\nDistancia total aproximada: " << distancia << " metros\n";
                }

                campus.generarMapaHTML("mapa.html", origen, destino, camino, distancia);
                pausar();
                break;
            }
            case 5:
                std::cout << "Hasta luego.\n";
                break;
            default:
                std::cout << "Opcion no valida.\n";
        }
    } while (opcion != 5);

    return 0;
}
