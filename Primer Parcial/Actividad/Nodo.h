#ifndef NODO_H
#define NODO_H

#include <string>
using namespace std;

class Nodo {
private:
    string cedula;
    string nombre;
    string apellido;
    Nodo* siguiente;

public:
    Nodo(string, string, string);

    void setCedula(string);
    void setNombre(string);
    void setApellido(string);
    void setSiguiente(Nodo*);

    string getCedula();
    string getNombre();
    string getApellido();
    Nodo* getSiguiente();
};

#endif