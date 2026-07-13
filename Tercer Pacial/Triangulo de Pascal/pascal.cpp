#include <iostream>

int main() {
    int filas = 0;
    std::cout << "Ingresa el numero de filas para el Triangulo de Pascal: ";
    std::cin >> filas; // <-- Corregido aquí (std::cin)

    int* val = new int; 

    for (int n = 0; n < filas; n++) {
        for (int espacios = 0; espacios < filas - n - 1; espacios++) {
            std::cout << "  ";
        }

        *val = 1; 
        for (int k = 0; k <= n; k++) {
            std::cout << *val << "   ";
            *val = (*val) * (n - k) / (k + 1);
        }
        std::cout << std::endl;
    }

    delete val;
    return 0;
}