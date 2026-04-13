#ifndef VALIDADORCEDULA_H
#define VALIDADORCEDULA_H

#include <iostream>
#include <string>
using namespace std;

template <typename T>
class ValidadorCedula {
private:
    T cedula;

public:
    ValidadorCedula(T c);

    bool esValida();
    void mostrarResultado();
};


template <typename T>
ValidadorCedula<T>::ValidadorCedula(T c) {
    cedula = c;
}

template <typename T>
bool ValidadorCedula<T>::esValida() {

    if (cedula.length() != 10) {
        return false;
    }

    int provincia = stoi(cedula.substr(0, 2));
    if (provincia < 1 || provincia > 24) {
        return false;
    }

    int suma = 0;

    for (int i = 0; i < 9; i++) {
        int digito = cedula[i] - '0';

        if (i % 2 == 0) {
            digito *= 2;
            if (digito > 9) {
                digito -= 9;
            }
        }

        suma += digito;
    }

    int verificador = (10 - (suma % 10)) % 10;
    int ultimo = cedula[9] - '0';

    return verificador == ultimo;
}

template <typename T>
void ValidadorCedula<T>::mostrarResultado() {
    if (esValida()) {
        cout << "Cedula valida" << endl;
    } else {
        cout << "Cedula invalida" << endl;
    }
}

#endif 