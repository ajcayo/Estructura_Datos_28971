#define _USE_MATH_DEFINES  // necesario en Windows/MinGW para que M_PI exista
#include "Grafo.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <queue>
#include <cmath>
#include <limits>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static const double INF = std::numeric_limits<double>::infinity();

Grafo::Grafo(int n) : n(n) {
    lugares.resize(n);
    matriz.assign(n, std::vector<double>(n, INF));
    for (int i = 0; i < n; i++) matriz[i][i] = 0.0;
    lista.resize(n);
}

void Grafo::agregarLugar(int id, const std::string& nombre, double lat, double lng) {
    lugares[id] = Lugar{id, nombre, lat, lng};
}

double Grafo::haversine(double lat1, double lon1, double lat2, double lon2) {
    const double R = 6371000.0; // radio de la Tierra en metros
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;
    double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
               std::cos(lat1 * M_PI / 180.0) * std::cos(lat2 * M_PI / 180.0) *
               std::sin(dLon / 2) * std::sin(dLon / 2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    return R * c;
}

void Grafo::agregarArista(int u, int v) {
    double d = haversine(lugares[u].lat, lugares[u].lng, lugares[v].lat, lugares[v].lng);
    matriz[u][v] = d;
    matriz[v][u] = d;
    lista[u].push_back({v, d});
    lista[v].push_back({u, d});
    aristas.push_back({u, v, d});
}

std::vector<int> Grafo::dijkstra(int origen, int destino, double& distanciaTotal) const {
    std::vector<double> dist(n, INF);
    std::vector<int> previo(n, -1);
    std::vector<bool> visitado(n, false);

    using Par = std::pair<double, int>; // (distancia, nodo)
    std::priority_queue<Par, std::vector<Par>, std::greater<Par>> pq;

    dist[origen] = 0.0;
    pq.push({0.0, origen});

    while (!pq.empty()) {
        double d = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        (void)d;
        if (visitado[u]) continue;
        visitado[u] = true;

        if (u == destino) break;

        for (std::list<std::pair<int, double>>::const_iterator it = lista[u].begin();
             it != lista[u].end(); ++it) {
            int v = it->first;
            double peso = it->second;
            if (!visitado[v] && dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
                previo[v] = u;
                pq.push(std::make_pair(dist[v], v));
            }
        }
    }

    distanciaTotal = dist[destino];
    std::vector<int> camino;
    if (dist[destino] == INF) return camino; // no hay ruta

    for (int at = destino; at != -1; at = previo[at]) {
        camino.push_back(at);
    }
    std::reverse(camino.begin(), camino.end());
    return camino;
}

void Grafo::imprimirLugares() const {
    std::cout << "\n===== LUGARES DEL CAMPUS ESPE - MATRIZ SANGOLQUI =====\n";
    for (const auto& l : lugares) {
        std::cout << std::setw(2) << l.id << ". " << l.nombre
                   << "  (lat=" << l.lat << ", lng=" << l.lng << ")\n";
    }
}

void Grafo::imprimirMatriz() const {
    std::cout << "\n===== MATRIZ DE ADYACENCIA (metros) =====\n";
    std::cout << std::fixed << std::setprecision(0);
    std::cout << std::setw(5) << " ";
    for (int j = 0; j < n; j++) std::cout << std::setw(7) << j;
    std::cout << "\n";
    for (int i = 0; i < n; i++) {
        std::cout << std::setw(5) << i;
        for (int j = 0; j < n; j++) {
            if (matriz[i][j] == INF) std::cout << std::setw(7) << "-";
            else std::cout << std::setw(7) << matriz[i][j];
        }
        std::cout << "\n";
    }
}

void Grafo::imprimirListas() const {
    std::cout << "\n===== LISTAS DE ADYACENCIA =====\n";
    std::cout << std::fixed << std::setprecision(1);
    for (int i = 0; i < n; i++) {
        std::cout << i << " (" << lugares[i].nombre << ") -> ";
        bool primero = true;
        for (std::list<std::pair<int, double>>::const_iterator it = lista[i].begin();
             it != lista[i].end(); ++it) {
            if (!primero) std::cout << ", ";
            std::cout << it->first << "[" << it->second << "m]";
            primero = false;
        }
        std::cout << "\n";
    }
}

void Grafo::generarMapaHTML(const std::string& rutaArchivo,
                             int origen,
                             int destino,
                             const std::vector<int>& camino,
                             double distanciaTotal) const {
    std::ostringstream nodosJS;
    for (int i = 0; i < n; i++) {
        nodosJS << "{id:" << i << ",nombre:\"" << lugares[i].nombre << "\",lat:"
                << std::fixed << std::setprecision(6) << lugares[i].lat
                << ",lng:" << lugares[i].lng << "},\n";
    }

    std::ostringstream aristasJS;
    for (const auto& a : aristas) {
        aristasJS << "[[" << lugares[a.u].lat << "," << lugares[a.u].lng << "],["
                  << lugares[a.v].lat << "," << lugares[a.v].lng << "]],\n";
    }

    std::ostringstream caminoJS;
    for (size_t i = 0; i < camino.size(); i++) {
        int id = camino[i];
        caminoJS << "[" << lugares[id].lat << "," << lugares[id].lng << "]";
        if (i + 1 < camino.size()) caminoJS << ",\n";
    }

    bool hayRuta = !camino.empty();

    std::ofstream out(rutaArchivo);
    out << R"HTML(<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<title>Ruta optima - Campus ESPE Matriz Sangolqui</title>
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<link rel="stylesheet" href="https://unpkg.com/leaflet@1.9.4/dist/leaflet.css" />
<style>
  html, body { margin:0; padding:0; height:100%; font-family: Arial, Helvetica, sans-serif; }
  #mapa { height: 100vh; width: 100%; }
  .panel {
    position: absolute; top: 10px; left: 50px; z-index: 1000;
    background: white; padding: 14px 18px; border-radius: 10px;
    box-shadow: 0 2px 10px rgba(0,0,0,0.3); max-width: 320px;
  }
  .panel h2 { margin: 0 0 8px 0; font-size: 16px; color:#0b3d91; }
  .panel p { margin: 4px 0; font-size: 13px; }
  .badge { display:inline-block; padding:2px 8px; border-radius:12px; font-size:11px; color:white; }
  .ok { background:#2e7d32; }
  .no { background:#c62828; }
</style>
</head>
<body>
<div class="panel">
  <h2>Ruta optima - Dijkstra</h2>
  <p><b>Origen:</b> )HTML" << lugares[origen].nombre << R"HTML(</p>
  <p><b>Destino:</b> )HTML" << lugares[destino].nombre << R"HTML(</p>
)HTML";

    if (hayRuta) {
        out << "  <p><b>Distancia total:</b> " << std::fixed << std::setprecision(1)
            << distanciaTotal << " m</p>\n";
        out << "  <p><span class=\"badge ok\">Ruta encontrada</span></p>\n";
    } else {
        out << "  <p><span class=\"badge no\">No existe ruta</span></p>\n";
    }

    out << R"HTML(  <p style="color:#555">Linea gris: todas las rutas del grafo (no dirigido).<br>Linea azul: mejor ruta calculada.</p>
</div>
<div id="mapa"></div>
<script src="https://unpkg.com/leaflet@1.9.4/dist/leaflet.js"></script>
<script>
  var lugares = [
)HTML" << nodosJS.str() << R"HTML(  ];

  var aristas = [
)HTML" << aristasJS.str() << R"HTML(  ];

  var camino = [
)HTML" << caminoJS.str() << R"HTML(
  ];

  var origenId = )HTML" << origen << R"HTML(;
  var destinoId = )HTML" << destino << R"HTML(;

  var centro = [lugares[0].lat, lugares[0].lng];
  var mapa = L.map('mapa').setView(centro, 17);

  L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
      maxZoom: 20,
      attribution: '&copy; OpenStreetMap contributors'
  }).addTo(mapa);

  // Dibuja todas las aristas del grafo (rutas posibles) en gris
  aristas.forEach(function(a) {
      L.polyline(a, {color: '#9e9e9e', weight: 3, opacity: 0.6}).addTo(mapa);
  });

  // Dibuja todos los lugares (nodos)
  lugares.forEach(function(l) {
      var color = '#1565c0';
      if (l.id === origenId) color = '#2e7d32';
      if (l.id === destinoId) color = '#c62828';
      var marker = L.circleMarker([l.lat, l.lng], {
          radius: 8, color: '#ffffff', weight: 2,
          fillColor: color, fillOpacity: 1
      }).addTo(mapa);
      marker.bindPopup('<b>' + l.id + '. ' + l.nombre + '</b>');
  });

  // Dibuja la mejor ruta (resaltada) encima de todo
  if (camino.length > 1) {
      L.polyline(camino, {color: '#1976d2', weight: 6, opacity: 0.9}).addTo(mapa);
  }
</script>
</body>
</html>
)HTML";
    out.close();

    std::cout << "\nMapa generado en: " << rutaArchivo
               << "  (abrelo con doble clic o 'Live Server' en VS Code)\n";
}
