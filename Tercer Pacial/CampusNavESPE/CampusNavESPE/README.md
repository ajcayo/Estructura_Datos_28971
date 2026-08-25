# Sistema Inteligente de Navegación - Campus ESPE Sangolquí

Proyecto universitario en C++17. Modela el campus de la ESPE (Sangolquí) como un
grafo no dirigido y ponderado, y calcula la ruta más corta entre dos puntos con
el algoritmo de Dijkstra.

## Compilar

### Linux / macOS (g++)
```
g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o CampusNavESPE
./CampusNavESPE
```

### Windows con MinGW (PowerShell) - mismo esquema que usas normalmente
```
g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o CampusNavESPE.exe
.\CampusNavESPE.exe
```

### Con CMake (multiplataforma)
```
mkdir build
cd build
cmake ..
cmake --build .
```

## Cómo funciona

1. Al iniciar, el programa te pregunta cuántos de los 14 puntos del catálogo
   del campus quieres cargar (mínimo 1, máximo 14).
2. Con eso construye el grafo dinámicamente: nodos, aristas, lista de
   adyacencia y matriz de adyacencia, todo con memoria dinámica (`new`/`delete`).
3. Desde el menú puedes:
   - Listar los nodos cargados
   - Ver la lista de adyacencia
   - Ver la matriz de adyacencia
   - Ver un mapa ASCII aproximado del campus
   - Calcular la ruta más corta entre dos puntos (Dijkstra) y verla dibujada
     sobre el mapa

## Estructura

```
include/   Headers (.h) de cada clase
src/       Implementaciones (.cpp) + main.cpp
docs/      Notas de diseño / UML
CMakeLists.txt
```

## Notas de diseño (resumen)

- **Nodo**: representa un punto físico del campus (edificio, garita, entrada,
  cruce, escalera, rampa, parqueadero, cafetería, biblioteca, laboratorio,
  auditorio). Inmutable una vez creado.
- **Arista**: conexión entre dos nodos, con distancia (calculada con la
  fórmula de Haversine a partir de coordenadas reales aproximadas),
  tipo de camino, accesibilidad y estado (abierta/cerrada).
- **ListaAdyacencia**: lista enlazada dinámica propia (sin `std::vector`),
  una por nodo.
- **Grafo**: dueño real de toda la memoria dinámica (nodos, aristas, listas
  de adyacencia y matriz). Sin variables globales, sin arreglos de tamaño
  fijo en tiempo de compilación.
- **Dijkstra**: O(V²), arreglos dimensionados en tiempo de ejecución según
  la cantidad real de nodos cargados. Reconstruye el camino como una lista
  enlazada dinámica.
- **CargadorDatos**: contiene el catálogo de puntos reales del campus (14
  ubicaciones con coordenadas aproximadas) y las conexiones entre ellos.
- **Renderizador**: solo dibuja (mapa ASCII y ruta), no calcula nada.
- **Interfaz**: solo maneja el menú de consola.

Verificado con `valgrind --leak-check=full`: 0 fugas de memoria.

## Próximos pasos sugeridos

- Implementar A* como alternativa a Dijkstra.
- Cargar el catálogo desde un archivo externo en `data/` en vez de tenerlo
  embebido en el código.
- Integrar Google Maps solo para visualización/coordenadas (nunca para
  calcular rutas internas).
- Reemplazar el mapa ASCII por una interfaz gráfica (Win32 GDI, tal como
  trabajas normalmente tus proyectos de consola/gráficos).
