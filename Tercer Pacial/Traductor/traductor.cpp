#include <windows.h>
#include <wininet.h>
#include <mmsystem.h>
#include <cstdio>
#include <iostream>

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "winmm.lib")

using namespace std;

struct Nodo {
    char* original;
    char* traducido;
    char* idiomaOrigen;
    Nodo* siguiente;
};

int largoCadena(const char* c) {
    int n = 0;
    const char* p = c;
    while (*p != '\0') { n++; p++; }
    return n;
}

char* duplicarCadena(const char* origen) {
    int longitud = largoCadena(origen);
    char* copia = new char[longitud + 1];
    char* destino = copia;
    const char* fuente = origen;
    while (*fuente != '\0') {
        *destino = *fuente;
        destino++;
        fuente++;
    }
    *destino = '\0';
    return copia;
}

bool compararCadenas(const char* a, const char* b) {
    while (*a != '\0' && *b != '\0') {
        char ca = *a;
        char cb = *b;
        if (ca >= 'A' && ca <= 'Z') ca = ca + 32;
        if (cb >= 'A' && cb <= 'Z') cb = cb + 32;
        if (ca != cb) return false;
        a++;
        b++;
    }
    return *a == '\0' && *b == '\0';
}

Nodo* buscarEnCache(Nodo* cabeza, const char* palabra, const char* idiomaOrigen) {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        if (compararCadenas(actual->original, palabra) &&
            compararCadenas(actual->idiomaOrigen, idiomaOrigen)) {
            return actual;
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

Nodo* agregarACache(Nodo* cabeza, const char* palabra, const char* traduccion, const char* idiomaOrigen) {
    Nodo* nuevo = new Nodo;
    nuevo->original = duplicarCadena(palabra);
    nuevo->traducido = duplicarCadena(traduccion);
    nuevo->idiomaOrigen = duplicarCadena(idiomaOrigen);
    nuevo->siguiente = cabeza;
    return nuevo;
}

void liberarCache(Nodo* cabeza) {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        Nodo* siguiente = actual->siguiente;
        delete[] actual->original;
        delete[] actual->traducido;
        delete[] actual->idiomaOrigen;
        delete actual;
        actual = siguiente;
    }
}

char* codificarURL(const char* texto) {
    int longitud = largoCadena(texto);
    char* salida = new char[longitud * 3 + 1];
    char* destino = salida;
    const char* fuente = texto;
    const char* hexDigitos = "0123456789ABCDEF";

    while (*fuente != '\0') {
        unsigned char c = (unsigned char)(*fuente);
        bool seguro = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
        if (seguro) {
            *destino = c;
            destino++;
        } else {
            *destino = '%';
            destino++;
            *destino = *(hexDigitos + (c / 16));
            destino++;
            *destino = *(hexDigitos + (c % 16));
            destino++;
        }
        fuente++;
    }
    *destino = '\0';
    return salida;
}

char* concatenar(const char* a, const char* b, const char* c, const char* d, const char* e) {
    int total = largoCadena(a) + largoCadena(b) + largoCadena(c) + largoCadena(d) + largoCadena(e);
    char* resultado = new char[total + 1];
    char* destino = resultado;

    const char* fuente = a;
    while (*fuente != '\0') { *destino = *fuente; destino++; fuente++; }
    fuente = b;
    while (*fuente != '\0') { *destino = *fuente; destino++; fuente++; }
    fuente = c;
    while (*fuente != '\0') { *destino = *fuente; destino++; fuente++; }
    fuente = d;
    while (*fuente != '\0') { *destino = *fuente; destino++; fuente++; }
    fuente = e;
    while (*fuente != '\0') { *destino = *fuente; destino++; fuente++; }

    *destino = '\0';
    return resultado;
}

char* descargarURL(const char* url) {
    HINTERNET hInternet = InternetOpenA("Traductor", INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, 0);
    if (hInternet == nullptr) return nullptr;

    HINTERNET hConexion = InternetOpenUrlA(hInternet, url, nullptr, 0, INTERNET_FLAG_RELOAD, 0);
    if (hConexion == nullptr) {
        InternetCloseHandle(hInternet);
        return nullptr;
    }

    const int TAM_BLOQUE = 4096;
    char bloque[TAM_BLOQUE];
    DWORD bytesLeidos = 0;

    int capacidad = TAM_BLOQUE * 4;
    char* buffer = new char[capacidad];
    char* escritura = buffer;
    int totalEscrito = 0;

    while (InternetReadFile(hConexion, bloque, TAM_BLOQUE, &bytesLeidos) && bytesLeidos > 0) {
        if (totalEscrito + (int)bytesLeidos >= capacidad) {
            int nuevaCapacidad = capacidad * 2;
            char* nuevoBuffer = new char[nuevaCapacidad];
            char* destinoTemp = nuevoBuffer;
            char* fuenteTemp = buffer;
            int contados = 0;
            while (contados < totalEscrito) {
                *destinoTemp = *fuenteTemp;
                destinoTemp++;
                fuenteTemp++;
                contados++;
            }
            delete[] buffer;
            buffer = nuevoBuffer;
            capacidad = nuevaCapacidad;
            escritura = buffer + totalEscrito;
        }

        char* origenBloque = bloque;
        DWORD copiados = 0;
        while (copiados < bytesLeidos) {
            *escritura = *origenBloque;
            escritura++;
            origenBloque++;
            copiados++;
        }
        totalEscrito += bytesLeidos;
    }

    *escritura = '\0';

    InternetCloseHandle(hConexion);
    InternetCloseHandle(hInternet);

    return buffer;
}

int hexAValor(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0;
}

char* escribirUTF8(char* destino, unsigned int codepoint) {
    if (codepoint < 0x80) {
        *destino = (char)codepoint;
        destino++;
    } else if (codepoint < 0x800) {
        *destino = (char)(0xC0 | (codepoint >> 6)); destino++;
        *destino = (char)(0x80 | (codepoint & 0x3F)); destino++;
    } else if (codepoint < 0x10000) {
        *destino = (char)(0xE0 | (codepoint >> 12)); destino++;
        *destino = (char)(0x80 | ((codepoint >> 6) & 0x3F)); destino++;
        *destino = (char)(0x80 | (codepoint & 0x3F)); destino++;
    } else {
        *destino = (char)(0xF0 | (codepoint >> 18)); destino++;
        *destino = (char)(0x80 | ((codepoint >> 12) & 0x3F)); destino++;
        *destino = (char)(0x80 | ((codepoint >> 6) & 0x3F)); destino++;
        *destino = (char)(0x80 | (codepoint & 0x3F)); destino++;
    }
    return destino;
}

char* decodificarJSON(const char* inicio, const char* fin) {
    int capacidad = (int)(fin - inicio) * 4 + 1;
    char* resultado = new char[capacidad];
    char* destino = resultado;
    const char* fuente = inicio;

    while (fuente != fin) {
        if (*fuente == '\\' && (fuente + 1) != fin) {
            char siguiente = *(fuente + 1);
            if (siguiente == 'u' && (fuente + 5) < fin + 1 && (fuente + 6) <= fin) {
                unsigned int codepoint = (hexAValor(fuente[2]) << 12) | (hexAValor(fuente[3]) << 8) |
                                          (hexAValor(fuente[4]) << 4) | hexAValor(fuente[5]);
                fuente += 6;

                if (codepoint >= 0xD800 && codepoint <= 0xDBFF &&
                    fuente + 6 <= fin && fuente[0] == '\\' && fuente[1] == 'u') {
                    unsigned int bajo = (hexAValor(fuente[2]) << 12) | (hexAValor(fuente[3]) << 8) |
                                        (hexAValor(fuente[4]) << 4) | hexAValor(fuente[5]);
                    if (bajo >= 0xDC00 && bajo <= 0xDFFF) {
                        codepoint = 0x10000 + ((codepoint - 0xD800) << 10) + (bajo - 0xDC00);
                        fuente += 6;
                    }
                }
                destino = escribirUTF8(destino, codepoint);
            } else if (siguiente == 'n') { *destino = '\n'; destino++; fuente += 2; }
            else if (siguiente == 't') { *destino = '\t'; destino++; fuente += 2; }
            else if (siguiente == '"' || siguiente == '\\' || siguiente == '/') {
                *destino = siguiente; destino++; fuente += 2;
            } else { *destino = *fuente; destino++; fuente++; }
        } else {
            *destino = *fuente;
            destino++;
            fuente++;
        }
    }
    *destino = '\0';
    return resultado;
}

char* extraerTraduccion(const char* respuesta) {
    if (respuesta == nullptr) return nullptr;

    const char* patron = "[[[\"";
    const char* pos = respuesta;

    while (*pos != '\0') {
        const char* p1 = pos;
        const char* p2 = patron;
        while (*p2 != '\0' && *p1 == *p2) { p1++; p2++; }

        if (*p2 == '\0') {
            const char* inicio = p1;
            const char* temp = inicio;
            while (*temp != '\0') {
                if (*temp == '\\' && *(temp + 1) != '\0') { temp += 2; continue; }
                if (*temp == '"') break;
                temp++;
            }
            return decodificarJSON(inicio, temp);
        }
        pos++;
    }
    return nullptr;
}

char* traducirPalabra(const char* palabra, const char* idiomaOrigen, const char* idiomaDestino) {
    char* palabraCodificada = codificarURL(palabra);

    char* parteFinal = concatenar("&tl=", idiomaDestino, "&dt=t&q=", palabraCodificada, "");
    char* url = concatenar(
        "https://translate.googleapis.com/translate_a/single?client=gtx&sl=",
        idiomaOrigen,
        parteFinal,
        "", ""
    );

    char* respuesta = descargarURL(url);
    char* traduccion = extraerTraduccion(respuesta);

    delete[] palabraCodificada;
    delete[] parteFinal;
    delete[] url;
    if (respuesta != nullptr) delete[] respuesta;

    return traduccion;
}

bool descargarArchivoBinario(const char* url, const char* rutaArchivo) {
    HINTERNET hInternet = InternetOpenA(
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0 Safari/537.36",
        INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, 0);
    if (hInternet == nullptr) {
        cout << "  (error: InternetOpenA fallo, codigo " << GetLastError() << ")" << endl;
        return false;
    }

    const char* encabezados = "Referer: https://translate.google.com/\r\n";
    HINTERNET hConexion = InternetOpenUrlA(hInternet, url, encabezados, -1L,
        INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 0);
    if (hConexion == nullptr) {
        cout << "  (error: InternetOpenUrlA fallo, codigo " << GetLastError() << ")" << endl;
        InternetCloseHandle(hInternet);
        return false;
    }

    FILE* archivo = fopen(rutaArchivo, "wb");
    if (archivo == nullptr) {
        InternetCloseHandle(hConexion);
        InternetCloseHandle(hInternet);
        return false;
    }

    const int TAM_BLOQUE = 4096;
    char bloque[TAM_BLOQUE];
    DWORD bytesLeidos = 0;
    int totalDescargado = 0;

    while (InternetReadFile(hConexion, bloque, TAM_BLOQUE, &bytesLeidos) && bytesLeidos > 0) {
        fwrite(bloque, 1, bytesLeidos, archivo);
        totalDescargado += (int)bytesLeidos;
    }

    fclose(archivo);
    InternetCloseHandle(hConexion);
    InternetCloseHandle(hInternet);

    if (totalDescargado < 500) {
        cout << "  (advertencia: solo se descargaron " << totalDescargado
             << " bytes, probablemente no es audio valido)" << endl;
        return false;
    }

    return true;
}

void reproducirVoz(const char* palabra, const char* idioma) {
    char* palabraCodificada = codificarURL(palabra);
    char* parteFinal = concatenar("&client=tw-ob&q=", palabraCodificada, "", "", "");
    char* url = concatenar(
        "https://translate.google.com/translate_tts?ie=UTF-8&tl=",
        idioma, parteFinal, "", ""
    );

    const char* archivoTemp = "audio_temp.mp3";
    bool exito = descargarArchivoBinario(url, archivoTemp);

    if (exito) {
        mciSendStringA("close audio_traductor", nullptr, 0, nullptr);
        MCIERROR errAbrir = mciSendStringA("open audio_temp.mp3 type mpegvideo alias audio_traductor",
            nullptr, 0, nullptr);
        if (errAbrir == 0) {
            mciSendStringA("play audio_traductor wait", nullptr, 0, nullptr);
            mciSendStringA("close audio_traductor", nullptr, 0, nullptr);
        } else {
            cout << "  (error: no se pudo abrir el audio con MCI, codigo " << errAbrir << ")" << endl;
        }
    } else {
        cout << "No se pudo descargar el audio (revisa tu conexion a internet)." << endl;
    }

    delete[] palabraCodificada;
    delete[] parteFinal;
    delete[] url;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Nodo* cache = nullptr;
    char entrada[1000]; // Agrandamos el buffer para soportar oraciones completas
    char opcion[10];

    cout << "=== TRADUCTOR ESPANOL <-> INGLES (API de Google) ===" << endl;

    while (true) {
        cout << endl << "1. Espanol -> Ingles" << endl;
        cout << "2. Ingles -> Espanol" << endl;
        cout << "3. Salir" << endl;
        cout << "Elige una opcion: ";
        cin >> opcion;

        if (compararCadenas(opcion, "3")) break;

        if (!compararCadenas(opcion, "1") && !compararCadenas(opcion, "2")) {
            cout << "Opcion invalida." << endl;
            continue;
        }

        const char* idiomaOrigen = compararCadenas(opcion, "1") ? "es" : "en";
        const char* idiomaDestino = compararCadenas(opcion, "1") ? "en" : "es";

        cout << "Escribe una palabra o frase: ";
        
        // Limpiamos el buffer del cin (el salto de línea '\n' que quedó al presionar enter en la opción)
        cin.ignore(10000, '\n'); 
        
        // Leemos la frase completa incluyendo espacios
        cin.getline(entrada, 1000);

        Nodo* enCache = buscarEnCache(cache, entrada, idiomaOrigen);
        char* traduccion = nullptr;

        if (enCache != nullptr) {
            traduccion = duplicarCadena(enCache->traducido);
            cout << "(desde cache) ";
        } else {
            traduccion = traducirPalabra(entrada, idiomaOrigen, idiomaDestino);
            if (traduccion != nullptr) {
                cache = agregarACache(cache, entrada, traduccion, idiomaOrigen);
            }
        }

        if (traduccion != nullptr) {
            cout << "Traduccion: " << traduccion << endl;
            cout << "Reproduciendo pronunciacion..." << endl;
            reproducirVoz(traduccion, idiomaDestino);
        } else {
            cout << "No se pudo traducir la frase (revisa tu conexion a internet)." << endl;
        }

        if (traduccion != nullptr) delete[] traduccion;
    }

    liberarCache(cache);
    cout << "Hasta luego." << endl;
    return 0;
}








