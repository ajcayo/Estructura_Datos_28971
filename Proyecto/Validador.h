#pragma once
#include <string>
#include <iostream>

class Validador {
public:
    // ── Utilidades de texto ────────────────────────────────────
    static std::string aMayusculas(std::string texto);
    static std::string trim(std::string texto);
    static bool        validarTexto(std::string texto);
    static std::string leerTextoValido(std::string mensaje);

    // ── Lectura segura de datos ────────────────────────────────
    static int         leerEntero(std::string mensaje, int min, int max);
    static std::string leerSoloLetras(const std::string& mensaje);
    static std::string leerSoloNumeros(const std::string& mensaje, int longitud = 0);

    // ── Validaciones Ecuador ───────────────────────────────────
    static bool        validarCedula(std::string cedula);
    static std::string pedirCedula(const std::string& mensaje);
    static bool        validarPlaca(std::string placa);
    static std::string normalizarPlaca(const std::string& placa);
    static std::string pedirPlaca(const std::string& mensaje);

    // ── Cifrado Cesar ──────────────────────────────────────────
    static std::string cifrarPlacaCesar(std::string placa, int desplazamiento);

    // ── Cifrado XOR bit a bit ──────────────────────────────────
    static std::string mostrarBits(unsigned char byte);
    static std::string cifrarXOR(std::string texto, unsigned char clave);
    static std::string descifrarXOR(std::string cifrado, unsigned char clave);
    static void        mostrarProcesoXOR(std::string texto, unsigned char clave);
};