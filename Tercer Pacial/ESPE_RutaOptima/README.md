# Ruta Optima - Campus ESPE Matriz Sangolqui (Grafos + Dijkstra)

Programa en C++ que modela el **Campus Matriz de la Universidad de las Fuerzas
Armadas - ESPE (Sangolqui)** como un **grafo no dirigido**, con 17 lugares
reales (Rectorado, Biblioteca Alejandro Segovia, Coliseo, facultades, etc.)
conectados por caminos internos. Calcula la **ruta mas corta** entre dos
lugares usando **Dijkstra** y genera un **mapa interactivo tipo Google Maps**
(usando Leaflet + OpenStreetMap) donde se ven todas las rutas posibles y la
ruta recomendada resaltada.

## Requisitos

- Visual Studio Code
- Extension "C/C++" (ms-vscode.cpptools)
- Compilador `g++` (MinGW en Windows, o build-essential en Linux, o Xcode
  Command Line Tools en Mac)
- Navegador web (Chrome, Edge, Firefox...) para ver `mapa.html`

## Estructura del proyecto

```
ESPE_RutaOptima/
├── main.cpp          -> menu, datos reales del campus (nodos y conexiones)
├── Grafo.h            -> definicion de la clase Grafo
├── Grafo.cpp           -> matriz de adyacencia, lista de adyacencia, Dijkstra,
│                           generador del mapa HTML (Leaflet)
├── .vscode/
│   ├── tasks.json      -> tarea de compilacion (Ctrl+Shift+B)
│   └── launch.json     -> configuracion para depurar (F5)
└── README.md
```

## Como compilar y ejecutar en VS Code

1. Abre la carpeta `ESPE_RutaOptima` en VS Code (`File > Open Folder`).
2. Presiona `Ctrl+Shift+B` (o `Terminal > Run Task > Compilar Ruta ESPE`).
   Esto genera el ejecutable `rutaESPE` (o `rutaESPE.exe` en Windows).
3. En la terminal integrada, ejecuta:
   - Windows: `.\rutaESPE.exe`
   - Linux/Mac: `./rutaESPE`

   O compila manualmente desde la terminal:
   ```bash
   g++ -std=c++17 main.cpp Grafo.cpp -o rutaESPE
   ./rutaESPE
   ```

4. En el menu:
   - Opcion **1**: ver la lista de lugares (nodos) con sus coordenadas reales.
   - Opcion **2**: ver la **matriz de adyacencia** (distancias en metros).
   - Opcion **3**: ver las **listas de adyacencia**.
   - Opcion **4**: ingresa el ID de origen y destino; el programa calcula la
     mejor ruta con **Dijkstra** y genera `mapa.html` en la carpeta del proyecto.
5. Abre `mapa.html` haciendo doble clic, o clic derecho > "Open with Live
   Server" en VS Code. Veras el mapa real de Sangolqui (tiles de
   OpenStreetMap) con:
   - Puntos azules: todos los lugares del campus.
   - Punto verde: lugar de origen.
   - Punto rojo: lugar de destino.
   - Lineas grises: todas las conexiones del grafo (rutas posibles).
   - Linea azul gruesa: la **mejor ruta** calculada.

## Notas tecnicas

- El grafo es **no dirigido**: cada conexion se agrega en ambos sentidos
  (`u -> v` y `v -> u`) tanto en la matriz como en la lista de adyacencia.
- El peso de cada arista es la **distancia real en metros** entre las dos
  coordenadas GPS, calculada con la formula de **Haversine** (no esta
  "inventado a mano").
- Dijkstra usa una **cola de prioridad (min-heap)** sobre la lista de
  adyacencia, con complejidad O((V + E) log V).
- Las coordenadas de los lugares son aproximaciones reales alrededor de
  Av. General Ruminahui s/n y Ambato, Sangolqui - Ecuador (ubicacion real
  del Campus Matriz ESPE). Si tienes coordenadas GPS mas exactas de cada
  edificio, puedes reemplazarlas directamente en `main.cpp`
  (funcion `construirCampusESPE`).
- Para agregar o quitar lugares/conexiones solo edita el arreglo
  `conexiones` y las llamadas `agregarLugar` en `main.cpp`.

## Como se ve

`mapa.html` abre un mapa real (como Google Maps, pero con tiles libres de
OpenStreetMap) centrado en el campus ESPE, con el grafo dibujado encima.
