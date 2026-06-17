#pragma once
#include "ListaUsuarios.h"
#include "ListaVehiculos.h"
#include "ListaDoble.h"


class GestorArchivos {
public:
  
    static bool guardarTodo(const ListaUsuarios& lu,
                            const ListaVehiculos& lv,
                            const ListaDoble&     lr);

  
    static bool cargarTodo(ListaUsuarios& lu,
                           ListaVehiculos& lv,
                           ListaDoble&     lr);
};