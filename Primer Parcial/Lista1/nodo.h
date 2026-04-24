#ifndef NODO_H
#define NODO_H
#include <string>
using namespace std;

class Nodo {
private:
    string cedula;
    string nombre;
    Nodo* siguiente;

public:
    Nodo(string, string);

    void setCedula(string);
    void setNombre(string);
    void setSiguiente(Nodo*);

    string getCedula();
    string getNombre();
    Nodo* getSiguiente();
};

#endif