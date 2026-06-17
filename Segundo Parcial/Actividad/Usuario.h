#pragma once
#include <string>

class Usuario {
private: 
    std::string nombre;
    std::string apellido;
    std::string cedula;
public:
    Usuario(std::string nom, std::string ape, std::string ced);
    std::string getNombre() const;
    std::string getApellido() const;
    std::string getCedula() const;
    void setNombre( std::string& nom);
    void setApellido(std::string& ape);
    void setCedula( std::string& ced);

};