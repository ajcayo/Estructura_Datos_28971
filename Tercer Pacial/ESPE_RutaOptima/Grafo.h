#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <list>
#include <string>

// Representa un lugar (nodo) real dentro del campus ESPE Matriz - Sangolqui
struct Lugar {
    int id;
    std::string nombre;
    double lat;   // latitud real (grados)
    double lng;   // longitud real (grados)
};

// Arista simple para reportes / dibujo del mapa
struct Arista {
    int u;
    int v;
    double peso; // distancia en metros
};

class Grafo {
public:
    explicit Grafo(int n);

    // Agrega los datos de un lugar (nodo) del campus
    void agregarLugar(int id, const std::string& nombre, double lat, double lng);

    // Crea una arista NO DIRIGIDA entre u y v.
    // El peso (distancia en metros) se calcula automaticamente con Haversine
    // a partir de las coordenadas reales de los dos lugares.
    void agregarArista(int u, int v);

    // Dijkstra: calcula la ruta mas corta entre origen y destino.
    // Devuelve la secuencia de nodos del camino y deja distanciaTotal (metros).
    std::vector<int> dijkstra(int origen, int destino, double& distanciaTotal) const;

    // Utilidades de impresion (requisito: mostrar matriz y listas)
    void imprimirLugares() const;
    void imprimirMatriz() const;
    void imprimirListas() const;

    // Genera un archivo HTML (Leaflet + OpenStreetMap) con:
    //  - todos los lugares como marcadores
    //  - todas las aristas (rutas posibles) en gris
    //  - la ruta calculada resaltada, con inicio y destino marcados
    void generarMapaHTML(const std::string& rutaArchivo,
                          int origen,
                          int destino,
                          const std::vector<int>& camino,
                          double distanciaTotal) const;

    int totalLugares() const { return n; }
    const Lugar& lugar(int id) const { return lugares[id]; }

private:
    int n;
    std::vector<Lugar> lugares;
    std::vector<std::vector<double>> matriz;             // matriz de adyacencia (INF = sin conexion)
    std::vector<std::list<std::pair<int, double>>> lista; // lista de adyacencia
    std::vector<Arista> aristas;                          // registro plano para dibujar el mapa

    static double haversine(double lat1, double lon1, double lat2, double lon2);
};

#endif
