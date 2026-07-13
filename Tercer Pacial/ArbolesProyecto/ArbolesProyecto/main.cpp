#include <iostream>
#include <vector>
#include <conio.h>
#include "ArbolBinario.h"
#include "ArbolRojoNegro.h"
#include "ArbolB.h"
#include "ArbolBMas.h"
#include "Graficador.h"
using namespace std;

vector<int> pedirNumeros(){
    vector<int> nums;
    int n;
    cout << "Cuantos numeros desea ingresar? ";
    cin >> n;
    cout << "Ingrese los " << n << " numeros:\n";
    for(int i = 0; i < n; i++){
        int v;
        cout << "  Numero " << (i+1) << ": ";
        cin >> v;
        nums.push_back(v);
    }
    return nums;
}

void imprimirRecorridos(const vector<int>& pre, const vector<int>& in_, const vector<int>& post){
    cout << "\n--- RECORRIDOS ---\n";
    cout << "PreOrden : "; for(int v : pre)  cout << v << " "; cout << "\n";
    cout << "InOrden  : "; for(int v : in_)  cout << v << " "; cout << "\n";
    cout << "PostOrden: "; for(int v : post) cout << v << " "; cout << "\n\n";
}

int main(){
    int opcion;
    do {
        system("cls");
        cout << "=================================================\n";
        cout << "   VISUALIZADOR DE ARBOLES - MENU PRINCIPAL\n";
        cout << "=================================================\n";
        cout << " 1. Arbol Binario de Busqueda (ABB)\n";
        cout << " 2. Arbol B\n";
        cout << " 3. Arbol B+\n";
        cout << " 4. Arbol Rojo-Negro\n";
        cout << " 5. Salir\n";
        cout << "-------------------------------------------------\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        if(opcion == 1){
            ArbolBinario arbol;
            vector<int> numeros = pedirNumeros();
            for(int v : numeros) arbol.insertar(v);
            imprimirRecorridos(arbol.preOrden(), arbol.inOrden(), arbol.postOrden());
            cout << "Presione una tecla para ver el arbol graficamente...";
            _getch();
            VentanaGrafica::mostrar("Arbol Binario de Busqueda", [&arbol](HDC hdc, RECT client){
                int centerX = (client.right - client.left) / 2;
                arbol.dibujar(hdc, centerX, 60, 200);
            });

        } else if(opcion == 2){
            ArbolB arbol;
            vector<int> numeros = pedirNumeros();
            for(int v : numeros) arbol.insertar(v);
            imprimirRecorridos(arbol.preOrden(), arbol.inOrden(), arbol.postOrden());
            cout << "Presione una tecla para ver el arbol graficamente...";
            _getch();
            VentanaGrafica::mostrar("Arbol B", [&arbol](HDC hdc, RECT client){
                int centerX = (client.right - client.left) / 2;
                arbol.dibujar(hdc, centerX, 60, client.right - 100);
            });

        } else if(opcion == 3){
            ArbolBMas arbol;
            vector<int> numeros = pedirNumeros();
            for(int v : numeros) arbol.insertar(v);
            imprimirRecorridos(arbol.preOrden(), arbol.inOrden(), arbol.postOrden());
            cout << "Presione una tecla para ver el arbol graficamente...";
            _getch();
            VentanaGrafica::mostrar("Arbol B+", [&arbol](HDC hdc, RECT client){
                int centerX = (client.right - client.left) / 2;
                arbol.dibujar(hdc, centerX, 60, client.right - 100);
            });

        } else if(opcion == 4){
            ArbolRojoNegro arbol;
            vector<int> numeros = pedirNumeros();
            for(int v : numeros) arbol.insertar(v);
            imprimirRecorridos(arbol.preOrden(), arbol.inOrden(), arbol.postOrden());
            cout << "Presione una tecla para ver el arbol graficamente...";
            _getch();
            VentanaGrafica::mostrar("Arbol Rojo-Negro", [&arbol](HDC hdc, RECT client){
                int centerX = (client.right - client.left) / 2;
                arbol.dibujar(hdc, centerX, 60, 200);
            });
        }

    } while(opcion != 5);

    cout << "Hasta luego!\n";
    return 0;
}
