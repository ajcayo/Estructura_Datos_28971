#ifndef PERSONA_H
#define PERSONA_H

#include <string>
using namespace std;

class Persona {
private:
    string cedula;
    string nombre;

public:
    Persona();
    Persona(string c, string n);

    void setCedula(string c);
    void setNombre(string n);

    string getCedula();
    string getNombre();
};

#endif