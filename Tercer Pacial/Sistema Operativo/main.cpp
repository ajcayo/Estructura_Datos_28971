#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

using namespace std;

// Estructura para simular un sistema de archivos simple
struct NodoArchivo {
    string nombre;
    string contenido; // <--- Aquí se guardará el texto del archivo
    bool esDirectorio;
    NodoArchivo* padre; 
    map<string, NodoArchivo*> hijos;
};

// Variables globales del sistema operativo simulado
NodoArchivo* raiz;
NodoArchivo* directorioActual;
bool sistemaCorriendo = true;

// Inicializa el disco duro virtual en RAM
void inicializarSistemaArchivos() {
    raiz = new NodoArchivo{"/", "", true, nullptr, {}};
    directorioActual = raiz;
    
    raiz->hijos["sistema"] = new NodoArchivo{"sistema", "", true, raiz, {}};
    raiz->hijos["documentos"] = new NodoArchivo{"documentos", "", true, raiz, {}};
}

// Pantalla de carga simulada (Boot)
void arrancarSistema() {
    cout << "========================================" << endl;
    cout << "  INICIANDO PEER-OS v1.2...             " << endl;
    cout << "========================================" << endl;
    cout << "[ OK ] Cargando Kernel en memoria..." << endl;
    cout << "[ OK ] Montando sistema de archivos virtual..." << endl;
    cout << "[ OK ] Consola CLI lista." << endl;
    cout << "Escribe 'ayuda' para ver los comandos disponibles.\n" << endl;
}

// Intérprete de comandos (Shell)
void procesarComando(string linea) {
    stringstream ss(linea);
    string comando, argumento, operador;
    ss >> comando >> argumento >> operador;

    if (comando == "ayuda") {
        cout << "Comandos disponibles:\n";
        cout << "  ls             - Listar archivos y carpetas\n";
        cout << "  cd [nombre]    - Cambiar de directorio ('cd ..' para regresar)\n";
        cout << "  mkdir [nombre] - Crear una nueva carpeta\n";
        cout << "  touch [nombre] - Crear un archivo vacio\n";
        cout << "  cat [nombre]   - Leer el contenido de un archivo\n";
        cout << "  cat > [nombre] - Escribir texto dentro de un archivo\n";
        cout << "  limpiar        - Limpiar la pantalla\n";
        cout << "  salir          - Apagar el sistema operativo\n";
    } 
    else if (comando == "ls") {
        if (directorioActual->hijos.empty()) {
            cout << "(Directorio vacio)" << endl;
        } else {
            for (auto const& par : directorioActual->hijos) {
                string nombre = par.first;
                NodoArchivo* nodo = par.second;

                if (nodo->esDirectorio) cout << "[DIR]  " << nombre << endl;
                else cout << "[FILE] " << nombre << endl;
            }
        }
    } 
    else if (comando == "cd") {
        if (argumento.empty()) {
            cout << "Error: Debes especificar el nombre de la carpeta." << endl;
        } 
        else if (argumento == "..") {
            if (directorioActual->padre != nullptr) {
                directorioActual = directorioActual->padre;
            } else {
                cout << "Ya estas en el directorio raiz (/)." << endl;
            }
        } 
        else {
            if (directorioActual->hijos.find(argumento) != directorioActual->hijos.end()) {
                NodoArchivo* destino = directorioActual->hijos[argumento];
                if (destino->esDirectorio) {
                    directorioActual = destino;
                } else {
                    cout << "Error: '" << argumento << "' no es un directorio." << endl;
                }
            } else {
                cout << "Error: No se encontro la carpeta '" << argumento << "'." << endl;
            }
        }
    }
    else if (comando == "mkdir") {
        if (argumento.empty()) {
            cout << "Error: Debes especificar un nombre para la carpeta." << endl;
        } else {
            directorioActual->hijos[argumento] = new NodoArchivo{argumento, "", true, directorioActual, {}};
            cout << "Carpeta '" << argumento << "' creada con exito." << endl;
        }
    }
    else if (comando == "touch") {
        if (argumento.empty()) {
            cout << "Error: Debes especificar un nombre para el archivo." << endl;
        } else {
            directorioActual->hijos[argumento] = new NodoArchivo{argumento, "(Archivo vacio, usa 'cat > " + argumento + "' para escribir)", false, directorioActual, {}};
            cout << "Archivo '" << argumento << "' creado con exito." << endl;
        }
    }
    else if (comando == "cat") {
        if (argumento.empty()) {
            cout << "Error: Especifica un archivo. Ejemplo: 'cat archivo.txt' o 'cat > archivo.txt'" << endl;
            return;
        }

        // CASO 1: cat > archivo (Modo Escritura)
        if (argumento == ">") {
            // Si usaron espacios como "cat > nota", el nombre del archivo quedó en la variable 'operador'
            string nombreArchivo = operador; 
            if (nombreArchivo.empty()) {
                cout << "Error: Especifica el nombre del archivo despues del '>'." << endl;
                return;
            }

            if (directorioActual->hijos.find(nombreArchivo) != directorioActual->hijos.end()) {
                NodoArchivo* archivo = directorioActual->hijos[nombreArchivo];
                if (!archivo->esDirectorio) {
                    cout << "Escribe el contenido del archivo (Presiona ENTER para guardar):\n> ";
                    string nuevoContenido;
                    getline(cin, nuevoContenido);
                    archivo->contenido = nuevoContenido;
                    cout << "Archivo guardado con exito." << endl;
                } else {
                    cout << "Error: '" << nombreArchivo << "' es un directorio, no un archivo." << endl;
                }
            } else {
                cout << "Error: El archivo '" << nombreArchivo << "' no existe. Crealo primero con 'touch'." << endl;
            }
        }
        // CASO 2: cat archivo (Modo Lectura)
        else {
            if (directorioActual->hijos.find(argumento) != directorioActual->hijos.end()) {
                NodoArchivo* archivo = directorioActual->hijos[argumento];
                if (!archivo->esDirectorio) {
                    cout << archivo->contenido << endl;
                } else {
                    cout << "Error: '" << argumento << "' es un directorio, no un archivo." << endl;
                }
            } else {
                cout << "Error: No se encontro el archivo '" << argumento << "'." << endl;
            }
        }
    }
    else if (comando == "limpiar") {
        cout << "\033[2J\033[1;1H";
    } 
    else if (comando == "salir") {
        cout << "Apagando el sistema... ¡Adios!" << endl;
        sistemaCorriendo = false;
    } 
    else if (!comando.empty()) {
        cout << "Comando '" << comando << "' no reconocido. Escribe 'ayuda'." << endl;
    }
}

int main() {
    inicializarSistemaArchivos();
    arrancarSistema();

    string lineaComando;
    while (sistemaCorriendo) {
        cout << "User@PeerOS:" << (directorioActual == raiz ? "" : "/") << directorioActual->nombre << "$ ";
        getline(cin, lineaComando);
        procesarComando(lineaComando);
    }

    return 0;
}

