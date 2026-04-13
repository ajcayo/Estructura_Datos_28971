/***********************************************************************
 * Module:  Quebrado.h
 * Author:  Janine
 * Modified: jueves, 9 de abril de 2026 07:09:19 p. m.
 * Purpose: Declaration of the class Quebrado
 ***********************************************************************/

#ifndef QUEBRADO_H
#define QUEBRADO_H

class Quebrado {
private:
    float num;
    float den;

public:
    Quebrado();
    Quebrado(float n, float d);

    void setNumerador(float n);
    void setDenominador(float d);

    float getNumerador();
    float getDenominador();
};

#endif 
