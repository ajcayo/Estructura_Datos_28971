#ifndef PANTALLA_H
#define PANTALLA_H

class Pantalla
{
    private:
        int ancho;
        int alto;

    public:
        Pantalla();

        void obtenerDatos();

        int getAncho();
        int getAlto();
};

#endif