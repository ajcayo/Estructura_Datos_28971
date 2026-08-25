/*
    Programa: Problema de la Mochila utilizando Búsqueda Exhaustiva

    Descripción:
    Este programa solicita al usuario el número de objetos, así como el peso y
    el valor de cada uno. Luego pide la capacidad máxima de la mochila y, mediante
    un algoritmo de búsqueda exhaustiva, evalúa todas las combinaciones posibles
    de objetos para determinar cuál ofrece el mayor valor sin superar el peso
    permitido. Finalmente, muestra los objetos seleccionados y el valor máximo
    obtenido.

    El acceso a los arreglos se realiza mediante aritmética de punteros.
*/
#include <iostream>
using namespace std;

int main() {

    int n;

    cout << "Ingrese el numero de objetos: ";
    cin >> n;

    int *pesos = new int[n];
    int *valores = new int[n];

    for (int i = 0; i < n; i++) {

        cout << "\nObjeto " << i + 1 << endl;

        cout << "Peso: ";
        cin >> *(pesos + i);

        cout << "Valor: ";
        cin >> *(valores + i);

    }

    int capacidad;

    cout << "\nIngrese la capacidad de la mochila: ";
    cin >> capacidad;

    int mejorValor = 0;
    int mejorCombinacion = 0;

    // Prueba todas las combinaciones posibles
    for (int i = 0; i < (1 << n); i++) {

        int pesoTotal = 0;
        int valorTotal = 0;

        for (int j = 0; j < n; j++) {

            if (i & (1 << j)) {

                pesoTotal += *(pesos + j);
                valorTotal += *(valores + j);

            }

        }

        if (pesoTotal <= capacidad && valorTotal > mejorValor) {

            mejorValor = valorTotal;
            mejorCombinacion = i;

        }

    }

    cout << "\nObjetos seleccionados:\n";

    for (int j = 0; j < n; j++) {

        if (mejorCombinacion & (1 << j)) {

            cout << "Objeto " << j + 1
                 << " -> Peso: " << *(pesos + j)
                 << "  Valor: " << *(valores + j)
                 << endl;

        }

    }

    cout << "\nValor maximo: " << mejorValor << endl;

    delete[] pesos;
    delete[] valores;

    return 0;
}

