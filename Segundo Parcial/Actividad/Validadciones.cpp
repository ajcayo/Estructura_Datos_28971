#pragma once
#include <string>
#include <iostream>
#include <cctype>
#include <algorithm>

// ============================================================
//  VALIDACIONES ECUADOR - Cedula y Placa
// ============================================================

// --- Leer solo texto (letras) con validacion en tiempo real ---
// No permite numeros ni espacios vacios. Pide reingresar si queda vacio.
std::string leerSoloLetras(const std::string& mensaje) {
    std::string valor;
    while (true) {
        std::cout << mensaje;
        std::getline(std::cin, valor);
        // Eliminar espacios al inicio y al final
        size_t ini = valor.find_first_not_of(" \t");
        size_t fin = valor.find_last_not_of(" \t");
        if (ini == std::string::npos) {
            std::cout << "  [!] El campo no puede estar vacio. Intente de nuevo.\n";
            continue;
        }
        valor = valor.substr(ini, fin - ini + 1);

        // Verificar que solo tenga letras y espacios simples entre palabras
        bool valido = true;
        for (char c : valor) {
            if (!isalpha((unsigned char)c) && c != ' ') {
                valido = false;
                break;
            }
        }
        if (!valido) {
            std::cout << "  [!] Solo se permiten letras. Sin numeros ni simbolos.\n";
            continue;
        }
        if (valor.empty()) {
            std::cout << "  [!] El campo no puede estar vacio. Intente de nuevo.\n";
            continue;
        }
        break;
    }
    return valor;
}

// --- Leer solo numeros (digitos) ---
// No permite letras ni espacios. Retorna el string numerico.
std::string leerSoloNumeros(const std::string& mensaje, int longitud = 0) {
    std::string valor;
    while (true) {
        std::cout << mensaje;
        std::getline(std::cin, valor);

        // Eliminar espacios
        valor.erase(std::remove(valor.begin(), valor.end(), ' '), valor.end());

        if (valor.empty()) {
            std::cout << "  [!] El campo no puede estar vacio. Intente de nuevo.\n";
            continue;
        }

        bool soloDigitos = true;
        for (char c : valor) {
            if (!isdigit((unsigned char)c)) {
                soloDigitos = false;
                break;
            }
        }
        if (!soloDigitos) {
            std::cout << "  [!] Solo se permiten numeros. Sin letras ni simbolos.\n";
            continue;
        }
        if (longitud > 0 && (int)valor.size() != longitud) {
            std::cout << "  [!] Debe ingresar exactamente " << longitud << " digitos.\n";
            continue;
        }
        break;
    }
    return valor;
}

// --- Leer un numero entero en rango ---
int leerEnteroEnRango(const std::string& mensaje, int minVal, int maxVal) {
    std::string entrada;
    int numero;
    while (true) {
        std::cout << mensaje;
        std::getline(std::cin, entrada);
        entrada.erase(std::remove(entrada.begin(), entrada.end(), ' '), entrada.end());
        if (entrada.empty()) {
            std::cout << "  [!] El campo no puede estar vacio.\n";
            continue;
        }
        bool soloDigitos = true;
        for (char c : entrada) {
            if (!isdigit((unsigned char)c)) { soloDigitos = false; break; }
        }
        if (!soloDigitos) {
            std::cout << "  [!] Ingrese solo numeros.\n";
            continue;
        }
        try {
            numero = std::stoi(entrada);
        } catch (...) {
            std::cout << "  [!] Numero fuera de rango.\n";
            continue;
        }
        if (numero < minVal || numero > maxVal) {
            std::cout << "  [!] Ingrese un valor entre " << minVal << " y " << maxVal << ".\n";
            continue;
        }
        break;
    }
    return numero;
}

// ============================================================
//  VALIDACION CEDULA ECUATORIANA (algoritmo de modulo 10)
// ============================================================
// Lambda recursiva: recalcula el digito verificador sumando
// los productos de los coeficientes [2,1,2,1...] con los digitos.
bool validarCedula(const std::string& cedula) {
    if (cedula.size() != 10) return false;
    for (char c : cedula) {
        if (!isdigit((unsigned char)c)) return false;
    }

    // Provincia: 01-24 (incluye 30 para Ecuatorianos en el exterior)
    int provincia = std::stoi(cedula.substr(0, 2));
    if (provincia < 1 || (provincia > 24 && provincia != 30)) return false;

    // Tercer digito: 0-5 para personas naturales
    int tercero = cedula[2] - '0';
    if (tercero < 0 || tercero > 5) return false; // sociedades privadas (6) y publicas (9) no aplican aqui

    // Lambda recursiva: suma acumulada del algoritmo modulo 10
    // Procesa cada digito multiplicado por coeficiente alternado (2 o 1)
    auto sumaVerificador = [&](auto& self, int idx, int acum) -> int {
        if (idx == 9) return acum; // digito 9 es el verificador, no se procesa
        int coef = (idx % 2 == 0) ? 2 : 1;
        int prod = (cedula[idx] - '0') * coef;
        if (prod >= 10) prod -= 9;
        return self(self, idx + 1, acum + prod);
    };

    int suma = sumaVerificador(sumaVerificador, 0, 0);
    int digitoCalculado = (suma % 10 == 0) ? 0 : (10 - (suma % 10));
    int digitoReal = cedula[9] - '0';

    return digitoCalculado == digitoReal;
}

// Pide cedula hasta que sea valida
std::string pedirCedula(const std::string& mensaje) {
    std::string cedula;
    while (true) {
        cedula = leerSoloNumeros(mensaje, 10);
        if (!validarCedula(cedula)) {
            std::cout << "  [!] Cedula ecuatoriana no valida. Verifique los 10 digitos.\n";
        } else {
            break;
        }
    }
    return cedula;
}

// ============================================================
//  VALIDACION PLACA ECUATORIANA
//  Formato: AAA-1234  (3 letras, guion, 4 digitos)
//  Tambien acepta placas de motos: AB-1234C  (pero aqui autos)
//  Formato estandar: 3 letras + 4 numeros (sin guion se acepta tambien)
// ============================================================
bool validarPlaca(const std::string& placa) {
    // Aceptar con o sin guion: ABC1234 o ABC-1234
    std::string p = placa;
    // Pasar a mayusculas
    for (char& c : p) c = toupper((unsigned char)c);

    // Eliminar guion si existe
    if (p.size() == 8 && p[3] == '-') {
        p = p.substr(0, 3) + p.substr(4);
    }

    if (p.size() != 7) return false;

    // Lambda recursiva: valida los primeros 3 caracteres como letras
    auto validarLetras = [&](auto& self, int idx) -> bool {
        if (idx == 3) return true;
        if (!isalpha((unsigned char)p[idx])) return false;
        return self(self, idx + 1);
    };

    // Lambda recursiva: valida los ultimos 4 caracteres como digitos
    auto validarDigitos = [&](auto& self, int idx) -> bool {
        if (idx == 7) return true;
        if (!isdigit((unsigned char)p[idx])) return false;
        return self(self, idx + 1);
    };

    return validarLetras(validarLetras, 0) && validarDigitos(validarDigitos, 3);
}

// Normaliza la placa a formato ABC-1234
std::string normalizarPlaca(const std::string& placa) {
    std::string p = placa;
    for (char& c : p) c = toupper((unsigned char)c);
    if (p.size() == 8 && p[3] == '-') return p; // ya tiene guion
    if (p.size() == 7) return p.substr(0, 3) + "-" + p.substr(3);
    return p;
}

// Pide placa hasta que sea valida
std::string pedirPlaca(const std::string& mensaje) {
    std::string placa;
    while (true) {
        std::cout << mensaje;
        std::getline(std::cin, placa);
        // Eliminar espacios
        placa.erase(std::remove(placa.begin(), placa.end(), ' '), placa.end());
        if (placa.empty()) {
            std::cout << "  [!] El campo no puede estar vacio.\n";
            continue;
        }
        if (!validarPlaca(placa)) {
            std::cout << "  [!] Placa no valida. Formato: ABC-1234 (3 letras + 4 numeros).\n";
            continue;
        }
        placa = normalizarPlaca(placa);
        break;
    }
    return placa;
}