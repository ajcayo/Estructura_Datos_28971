#ifndef _DLL_H_
#define _DLL_H_

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

class DLLIMPORT DllClass
{
    private:
        int ancho;
        int alto;

    public:
        DllClass();
        virtual ~DllClass();

        void capturarPantalla();

        int obtenerAncho();
        int obtenerAlto();

        void mostrarDatos();
};

#endif