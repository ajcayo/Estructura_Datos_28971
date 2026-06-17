#ifndef DETECTORCICLOS_H
#define DETECTORCICLOS_H


#include "Nodo.h"

class DetectorCiclos
{
    public:
      
        bool detectar(Nodo* cabeza);

  
        void detectarConPasos(Nodo* cabeza);
};

#endif