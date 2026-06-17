#include "GestorArchivos.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>

static const char* ARCHIVO_USUARIOS  = "usuarios.txt";
static const char* ARCHIVO_VEHICULOS = "vehiculos.txt";
static const char* ARCHIVO_RESERVAS  = "reservas.txt";



bool GestorArchivos::guardarTodo(const ListaUsuarios& lu,
                                 const ListaVehiculos& lv,
                                 const ListaDoble&     lr) {
    bool exito = true;


    {
        std::ofstream f(ARCHIVO_USUARIOS);
        if (!f.is_open()) {
            std::cout << "  [!] No se pudo abrir " << ARCHIVO_USUARIOS << " para escritura.\n";
            exito = false;
        } else {
            NodoUsuario* nodo = lu.getCabeza();
            while (nodo != nullptr) {
                Usuario* u = nodo->getUsuario();
                f << u->getCedula() << "|"
                  << u->getNombre() << "|"
                  << u->getApellido() << "\n";
                nodo = nodo->getSiguiente();
            }
            f.close();
        }
    }


    {
        std::ofstream f(ARCHIVO_VEHICULOS);
        if (!f.is_open()) {
            std::cout << "  [!] No se pudo abrir " << ARCHIVO_VEHICULOS << " para escritura.\n";
            exito = false;
        } else {
            NodoVehiculo* nodo = lv.getCabeza();
            while (nodo != nullptr) {
                f << nodo->getVehiculo()->getPlaca() << "\n";
                nodo = nodo->getSiguiente();
            }
            f.close();
        }
    }


    {
        std::ofstream f(ARCHIVO_RESERVAS);
        if (!f.is_open()) {
            std::cout << "  [!] No se pudo abrir " << ARCHIVO_RESERVAS << " para escritura.\n";
            exito = false;
        } else {
            Nodo* nodo = lr.getCabeza();
            while (nodo != nullptr) {
                Reserva* r = nodo->getReserva();
                std::tm fecha = r->getFechaAsignada();
                f << r->getUsuario()->getCedula() << "|"
                  << r->getVehiculo()->getPlaca() << "|"
                  << fecha.tm_mday << "|"
                  << (fecha.tm_mon + 1) << "|"
                  << (fecha.tm_year + 1900) << "|"
                  << fecha.tm_wday << "|"
                  << r->getHoraAsignada() << "\n";
                nodo = nodo->getSiguiente();
            }
            f.close();
        }
    }

    return exito;
}


static std::string extraerCampo(const std::string& linea, size_t& pos) {
    size_t inicio = pos;
    while (pos < linea.size() && linea[pos] != '|') pos++;
    std::string campo = linea.substr(inicio, pos - inicio);
    if (pos < linea.size()) pos++; // saltar el '|'
    return campo;
}

bool GestorArchivos::cargarTodo(ListaUsuarios& lu,
                                ListaVehiculos& lv,
                                ListaDoble&     lr) {
    bool exito = true;


    {
        std::ifstream f(ARCHIVO_USUARIOS);
        if (!f.is_open()) {
   
            std::cout << "  [i] " << ARCHIVO_USUARIOS
                      << " no encontrado. Se iniciara sin usuarios previos.\n";
            exito = false;
        } else {
            std::string linea;
            while (std::getline(f, linea)) {
             
                if (!linea.empty() && linea.back() == '\r') linea.pop_back();
                if (linea.empty()) continue;

                size_t pos = 0;
                std::string cedula  = extraerCampo(linea, pos);
                std::string nombre  = extraerCampo(linea, pos);
                std::string apellido = extraerCampo(linea, pos);

                if (!cedula.empty() && !nombre.empty() && !apellido.empty()) {
                    lu.crear(new Usuario(nombre, apellido, cedula));
                }
            }
            f.close();
        }
    }

    {
        std::ifstream f(ARCHIVO_VEHICULOS);
        if (!f.is_open()) {
            std::cout << "  [i] " << ARCHIVO_VEHICULOS
                      << " no encontrado. Se iniciara sin vehiculos previos.\n";
            exito = false;
        } else {
            std::string linea;
            while (std::getline(f, linea)) {
                if (!linea.empty() && linea.back() == '\r') linea.pop_back();
                if (linea.empty()) continue;
                lv.crear(new Vehiculo(linea));
            }
            f.close();
        }
    }

    
    {
        std::ifstream f(ARCHIVO_RESERVAS);
        if (!f.is_open()) {
            std::cout << "  [i] " << ARCHIVO_RESERVAS
                      << " no encontrado. Se iniciara sin turnos previos.\n";
            exito = false;
        } else {
            std::string linea;
            while (std::getline(f, linea)) {
                if (!linea.empty() && linea.back() == '\r') linea.pop_back();
                if (linea.empty()) continue;

                size_t pos = 0;
                std::string cedula = extraerCampo(linea, pos);
                std::string placa  = extraerCampo(linea, pos);
                std::string sDia   = extraerCampo(linea, pos);
                std::string sMes   = extraerCampo(linea, pos);
                std::string sAnio  = extraerCampo(linea, pos);
                std::string sWday  = extraerCampo(linea, pos);
                std::string sHora  = extraerCampo(linea, pos);

                if (cedula.empty() || placa.empty() || sDia.empty()) continue;

         
                Usuario*  u = lu.buscar(cedula);
                Vehiculo* v = lv.buscar(placa);

                if (!u || !v) {
                    std::cout << "  [!] Reserva huerfana ignorada (CI:"
                              << cedula << " Placa:" << placa << ").\n";
                    continue;
                }

         
                std::tm fecha = {0};
                fecha.tm_mday  = std::stoi(sDia);
                fecha.tm_mon   = std::stoi(sMes) - 1;
                fecha.tm_year  = std::stoi(sAnio) - 1900;
                fecha.tm_wday  = sWday.empty() ? 0 : std::stoi(sWday);
                int hora = sHora.empty() ? 8 : std::stoi(sHora);

                lr.agregarReserva(new Reserva(u, v, fecha, hora));
            }
            f.close();
        }
    }

    return exito;
}