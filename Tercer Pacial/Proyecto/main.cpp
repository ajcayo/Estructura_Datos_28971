#define UNICODE
#define _UNICODE
#include <windows.h>
#include <iostream>
#include <cmath>

#define INF 999999.0
#define EARTH_RADIUS_KM 6371.0

// ==========================================
// ESTRUCTURAS DE DATOS (ARITMÉTICA DE PUNTEROS)
// ==========================================

struct CoordenadaGPS {
    double latitud;
    double longitud;
};

struct Nodo {
    int id;
    wchar_t nombre[50];
    CoordenadaGPS gps;
    int xVentana; // Coordenadas en píxeles para la ventana Win32
    int yVentana;
};

struct ElementoLista {
    int destino;
    double peso;
    ElementoLista* siguiente;
};

// Auxiliar para copiar cadenas anchas usando punteros (Sin corchetes)
void copiarCadenaW(wchar_t* destino, const wchar_t* origen) {
    while (*origen != L'\0') {
        *destino = *origen;
        destino++;
        origen++;
    }
    *destino = L'\0';
}

// ==========================================
// CÁLCULO DE DISTANCIA (HAVERSINE)
// ==========================================
double gradosARadianes(double grados) {
    return grados * (3.14159265358979323846 / 180.0);
}

double calcularDistanciaHaversine(CoordenadaGPS p1, CoordenadaGPS p2) {
    double dLat = gradosARadianes(p2.latitud - p1.latitud);
    double dLon = gradosARadianes(p2.longitud - p1.longitud);

    double a = sin(dLat / 2.0) * sin(dLat / 2.0) +
               cos(gradosARadianes(p1.latitud)) * cos(gradosARadianes(p2.latitud)) *
               sin(dLon / 2.0) * sin(dLon / 2.0);

    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
    return EARTH_RADIUS_KM * c * 1000.0;
}

// ==========================================
// VARIABLES GLOBALES PARA LA VENTANA WIN32
// ==========================================
class GrafoESPE;
GrafoESPE* g_mapaGlobal = NULL;
int* g_caminoGlobal = NULL;
int g_tamCaminoGlobal = 0;
int g_origenGlobal = -1;
int g_destinoGlobal = -1;
int g_pasoAnimacion = 0;

// ==========================================
// CLASE GRAFO ESPE SANGOLQUÍ
// ==========================================
class GrafoESPE {
private:
    int totalNodos;
    Nodo* nodos;
    double** matrizAdyacencia;
    ElementoLista** listaAdyacencia;

public:
    GrafoESPE(int n) {
        totalNodos = n;
        nodos = (Nodo*)malloc(totalNodos * sizeof(Nodo));

        matrizAdyacencia = (double**)malloc(totalNodos * sizeof(double*));
        for (int i = 0; i < totalNodos; i++) {
            *(matrizAdyacencia + i) = (double*)malloc(totalNodos * sizeof(double));
            for (int j = 0; j < totalNodos; j++) {
                if (i == j) {
                    *(*(matrizAdyacencia + i) + j) = 0.0;
                } else {
                    *(*(matrizAdyacencia + i) + j) = INF;
                }
            }
        }

        listaAdyacencia = (ElementoLista**)malloc(totalNodos * sizeof(ElementoLista*));
        for (int i = 0; i < totalNodos; i++) {
            *(listaAdyacencia + i) = NULL;
        }
    }

    ~GrafoESPE() {
        for (int i = 0; i < totalNodos; i++) {
            free(*(matrizAdyacencia + i));
            ElementoLista* actual = *(listaAdyacencia + i);
            while (actual != NULL) {
                ElementoLista* temp = actual;
                actual = actual->siguiente;
                free(temp);
            }
        }
        free(matrizAdyacencia);
        free(listaAdyacencia);
        free(nodos);
    }

    int getTotalNodos() { return totalNodos; }

    Nodo* getNodo(int idx) {
        return nodos + idx;
    }

    void registrarNodo(int idx, const wchar_t* nombre, double lat, double lon, int x, int y) {
        Nodo* ptrNodo = nodos + idx;
        ptrNodo->id = idx;
        copiarCadenaW(ptrNodo->nombre, nombre);
        ptrNodo->gps.latitud = lat;
        ptrNodo->gps.longitud = lon;
        ptrNodo->xVentana = x;
        ptrNodo->yVentana = y;
    }

    void agregarArista(int origen, int destino) {
        Nodo* n1 = nodos + origen;
        Nodo* n2 = nodos + destino;

        double peso = calcularDistanciaHaversine(n1->gps, n2->gps);

        *(*(matrizAdyacencia + origen) + destino) = peso;
        *(*(matrizAdyacencia + destino) + origen) = peso;

        ElementoLista* nuevo1 = (ElementoLista*)malloc(sizeof(ElementoLista));
        nuevo1->destino = destino;
        nuevo1->peso = peso;
        nuevo1->siguiente = *(listaAdyacencia + origen);
        *(listaAdyacencia + origen) = nuevo1;

        ElementoLista* nuevo2 = (ElementoLista*)malloc(sizeof(ElementoLista));
        nuevo2->destino = origen;
        nuevo2->peso = peso;
        nuevo2->siguiente = *(listaAdyacencia + destino);
        *(listaAdyacencia + destino) = nuevo2;
    }

    void listarNodosConsola() {
        for (int i = 0; i < totalNodos; i++) {
            Nodo* n = nodos + i;
            std::wcout << L"  [" << i + 1 << L"] " << n->nombre << L"\n";
        }
    }

    void mostrarMatriz() {
        std::wcout << L"\n=======================================================\n";
        std::wcout << L"        MATRIZ DE ADYACENCIA (DISTANCIAS EN METROS)     \n";
        std::wcout << L"=======================================================\n\t";
        for (int i = 0; i < totalNodos; i++) {
            std::wcout << L"N" << i + 1 << L"\t";
        }
        std::wcout << L"\n";

        for (int i = 0; i < totalNodos; i++) {
            std::wcout << L"N" << i + 1 << L"\t";
            for (int j = 0; j < totalNodos; j++) {
                double val = *(*(matrizAdyacencia + i) + j);
                if (val >= INF) {
                    std::wcout << L"INF\t";
                } else {
                    std::wcout << (int)val << L"m\t";
                }
            }
            std::wcout << L"\n";
        }
    }

    void mostrarLista() {
        std::wcout << L"\n=======================================================\n";
        std::wcout << L"                 LISTA DE ADYACENCIA                  \n";
        std::wcout << L"=======================================================\n";
        for (int i = 0; i < totalNodos; i++) {
            Nodo* n = nodos + i;
            std::wcout << L"[" << i + 1 << L"] " << n->nombre << L" -> ";
            ElementoLista* temp = *(listaAdyacencia + i);
            while (temp != NULL) {
                Nodo* dest = nodos + temp->destino;
                std::wcout << L"(" << dest->nombre << L" | " << (int)temp->peso << L"m) -> ";
                temp = temp->siguiente;
            }
            std::wcout << L"NULL\n";
        }
    }

    // ==========================================
    // ALGORITMO DIJKSTRA (SIN CORCHETES)
    // ==========================================
    void dijkstra(int origenIdx, int destinoIdx) {
        double* distancias = (double*)malloc(totalNodos * sizeof(double));
        bool* visitados = (bool*)malloc(totalNodos * sizeof(bool));
        int* previos = (int*)malloc(totalNodos * sizeof(int));

        for (int i = 0; i < totalNodos; i++) {
            *(distancias + i) = INF;
            *(visitados + i) = false;
            *(previos + i) = -1;
        }

        *(distancias + origenIdx) = 0.0;

        for (int count = 0; count < totalNodos - 1; count++) {
            double min = INF;
            int u = -1;

            for (int v = 0; v < totalNodos; v++) {
                if (!(*(visitados + v)) && *(distancias + v) <= min) {
                    min = *(distancias + v);
                    u = v;
                }
            }

            if (u == -1) break;
            *(visitados + u) = true;

            for (int v = 0; v < totalNodos; v++) {
                double peso = *(*(matrizAdyacencia + u) + v);
                if (!(*(visitados + v)) && peso != INF && *(distancias + u) != INF &&
                    (*(distancias + u) + peso < *(distancias + v))) {
                    *(distancias + v) = *(distancias + u) + peso;
                    *(previos + v) = u;
                }
            }
        }

        if (g_caminoGlobal != NULL) free(g_caminoGlobal);
        g_caminoGlobal = (int*)malloc(totalNodos * sizeof(int));
        g_tamCaminoGlobal = 0;

        int actual = destinoIdx;
        while (actual != -1) {
            *(g_caminoGlobal + g_tamCaminoGlobal) = actual;
            g_tamCaminoGlobal++;
            actual = *(previos + actual);
        }

        g_origenGlobal = origenIdx;
        g_destinoGlobal = destinoIdx;

        std::wcout << L"\n=======================================================\n";
        std::wcout << L"              RUTA CALCULADA CON EXITO                 \n";
        std::wcout << L"=======================================================\n";
        std::wcout << L" Origen:  " << (nodos + origenIdx)->nombre << L"\n";
        std::wcout << L" Destino: " << (nodos + destinoIdx)->nombre << L"\n";
        std::wcout << L" Distancia Estimada: " << *(distancias + destinoIdx) << L" metros\n";
        std::wcout << L"-------------------------------------------------------\n";

        free(distancias);
        free(visitados);
        free(previos);
    }

    ElementoLista** getListaAdyacencia() { return listaAdyacencia; }
};

// ==========================================
// PROCEDIMIENTO DE DIBUJO DE LA VENTANA WIN32
// ==========================================
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_TIMER:
        if (g_pasoAnimacion < g_tamCaminoGlobal) {
            g_pasoAnimacion++;
            InvalidateRect(hwnd, NULL, TRUE); // Redibujar pantalla
        }
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Fondo oscuro tipo mapa
        HBRUSH hFondo = CreateSolidBrush(RGB(30, 30, 30));
        RECT rectVentana;
        GetClientRect(hwnd, &rectVentana);
        FillRect(hdc, &rectVentana, hFondo);
        DeleteObject(hFondo);

        SetBkMode(hdc, TRANSPARENT);

        if (g_mapaGlobal != NULL) {
            // 1. Dibujar todas las aristas (Caminos en gris)
            HPEN penGris = CreatePen(PS_DOT, 2, RGB(100, 100, 100));
            SelectObject(hdc, penGris);

            for (int i = 0; i < g_mapaGlobal->getTotalNodos(); i++) {
                Nodo* origen = g_mapaGlobal->getNodo(i);
                ElementoLista* temp = *(g_mapaGlobal->getListaAdyacencia() + i);

                while (temp != NULL) {
                    Nodo* dest = g_mapaGlobal->getNodo(temp->destino);
                    MoveToEx(hdc, origen->xVentana, origen->yVentana, NULL);
                    LineTo(hdc, dest->xVentana, dest->yVentana);
                    temp = temp->siguiente;
                }
            }
            DeleteObject(penGris);

            // 2. Dibujar la ruta óptima resaltada (Amarillo Neón)
            HPEN penRuta = CreatePen(PS_SOLID, 6, RGB(255, 215, 0));
            SelectObject(hdc, penRuta);

            for (int i = g_tamCaminoGlobal - 1; i > 0; i--) {
                int id1 = *(g_caminoGlobal + i);
                int id2 = *(g_caminoGlobal + (i - 1));
                Nodo* n1 = g_mapaGlobal->getNodo(id1);
                Nodo* n2 = g_mapaGlobal->getNodo(id2);

                MoveToEx(hdc, n1->xVentana, n1->yVentana, NULL);
                LineTo(hdc, n2->xVentana, n2->yVentana);
            }
            DeleteObject(penRuta);

            // 3. Dibujar Nodos/Bloques
            for (int i = 0; i < g_mapaGlobal->getTotalNodos(); i++) {
                Nodo* n = g_mapaGlobal->getNodo(i);

                HBRUSH brushNodo = CreateSolidBrush(RGB(70, 130, 180)); // Azul por defecto
                if (i == g_origenGlobal) brushNodo = CreateSolidBrush(RGB(50, 205, 50));  // Verde (Origen)
                if (i == g_destinoGlobal) brushNodo = CreateSolidBrush(RGB(220, 20, 60)); // Rojo (Destino)

                SelectObject(hdc, brushNodo);
                Ellipse(hdc, n->xVentana - 15, n->yVentana - 15, n->xVentana + 15, n->yVentana + 15);
                DeleteObject(brushNodo);

                // Texto del Bloque
                SetTextColor(hdc, RGB(255, 255, 255));
                TextOutW(hdc, n->xVentana - 35, n->yVentana + 18, n->nombre, wcslen(n->nombre));
            }

            // 4. Puntero GPS animado siguiendo el recorrido (Punto Rojo)
            if (g_pasoAnimacion > 0) {
                int idActual = *(g_caminoGlobal + (g_tamCaminoGlobal - g_pasoAnimacion));
                Nodo* nActual = g_mapaGlobal->getNodo(idActual);

                HBRUSH brushGPS = CreateSolidBrush(RGB(255, 0, 0));
                SelectObject(hdc, brushGPS);
                Ellipse(hdc, nActual->xVentana - 8, nActual->yVentana - 8, nActual->xVentana + 8, nActual->yVentana + 8);
                DeleteObject(brushGPS);
            }
        }

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// ==========================================
// FUNCIÓN PARA CREAR Y ABRIR LA VENTANA GRAFICA
// ==========================================
void abrirVentanaMapa() {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    const wchar_t CLASS_NAME[] = L"ClaseMapaESPE";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
        0, CLASS_NAME, L"ESPE MAPS - Navegador GPS del Campus Sangolqui",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 900, 650,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return;

    g_pasoAnimacion = 1;
    SetTimer(hwnd, 1, 800, NULL); // Animación cada 800 milisegundos

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// ==========================================
// MAIN Y MENÚ DE SELECCIÓN
// ==========================================
int main() {
    int totalPuntos = 7;
    GrafoESPE mapaESPE(totalPuntos);
    g_mapaGlobal = &mapaESPE;

    // Registro de coordenadas en pantalla (X, Y en píxeles) para la ventana Win32
    mapaESPE.registrarNodo(0, L"Entrada_Principal", -0.314200, -78.445800, 100, 100);
    mapaESPE.registrarNodo(1, L"Bloque_A",          -0.314500, -78.445200, 350, 100);
    mapaESPE.registrarNodo(2, L"Bloque_B",          -0.314700, -78.444900, 650, 100);
    mapaESPE.registrarNodo(3, L"Bloque_H",          -0.315000, -78.444500, 650, 300);
    mapaESPE.registrarNodo(4, L"Biblioteca",        -0.313800, -78.444800, 350, 300);
    mapaESPE.registrarNodo(5, L"Canchas",           -0.315500, -78.446000, 100, 300);
    mapaESPE.registrarNodo(6, L"Comedor_Central",    -0.314800, -78.443900, 450, 480);

    // Conexiones de los caminos
    mapaESPE.agregarArista(0, 1);
    mapaESPE.agregarArista(0, 5);
    mapaESPE.agregarArista(1, 2);
    mapaESPE.agregarArista(1, 4);
    mapaESPE.agregarArista(2, 3);
    mapaESPE.agregarArista(5, 4);
    mapaESPE.agregarArista(4, 3);
    mapaESPE.agregarArista(3, 6);
    mapaESPE.agregarArista(4, 6);

    int opcion;
    do {
        system("cls");
        std::wcout << L"=======================================================\n";
        std::wcout << L"    SISTEMA DE NAVEGACION ESPE MAPS (SANGOLQUI)        \n";
        std::wcout << L"=======================================================\n";
        std::wcout << L"1. Planificar Ruta / Abrir Mapa Interactivo\n";
        std::wcout << L"2. Ver Matriz de Adyacencia\n";
        std::wcout << L"3. Ver Lista de Adyacencia\n";
        std::wcout << L"4. Salir\n";
        std::wcout << L"-------------------------------------------------------\n";
        std::wcout << L"Seleccione una opcion: ";
        std::cin >> opcion;

        if (opcion == 1) {
            int selOrigen = 0, selDestino = 0;

            system("cls");
            std::wcout << L"=======================================================\n";
            std::wcout << L"      PASO 1: ELECCION DEL PUNTO DE ORIGEN             \n";
            std::wcout << L"=======================================================\n";
            mapaESPE.listarNodosConsola();
            std::wcout << L"\nIngrese el numero de tu ubicacion actual: ";
            std::cin >> selOrigen;

            system("cls");
            std::wcout << L"=======================================================\n";
            std::wcout << L"      PASO 2: ELECCION DEL PUNTO DE DESTINO            \n";
            std::wcout << L"=======================================================\n";
            mapaESPE.listarNodosConsola();
            std::wcout << L"\nIngrese el numero del destino al que deseas ir: ";
            std::cin >> selDestino;

            if (selOrigen >= 1 && selOrigen <= totalPuntos && selDestino >= 1 && selDestino <= totalPuntos) {
                mapaESPE.dijkstra(selOrigen - 1, selDestino - 1);

                std::wcout << L"\n[API / GPS]: Abriendo ventana grafica del mapa...\n";
                
                // Abre la ventana gráfica independiente (Win32 API)
                abrirVentanaMapa();
            } else {
                std::wcout << L"\n[Error]: Opcion invalida.\n";
                Sleep(1500);
            }
        } else if (opcion == 2) {
            system("cls");
            mapaESPE.mostrarMatriz();
            std::wcout << L"\nPresione ENTER para regresar...";
            std::cin.ignore();
            std::cin.get();
        } else if (opcion == 3) {
            system("cls");
            mapaESPE.mostrarLista();
            std::wcout << L"\nPresione ENTER para regresar...";
            std::cin.ignore();
            std::cin.get();
        }

    } while (opcion != 4);

    return 0;
}







