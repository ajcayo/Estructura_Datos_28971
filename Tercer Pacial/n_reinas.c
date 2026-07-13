/*
 * ============================================================
 *  PROBLEMA DE LAS N REINAS - BACKTRACKING
 * ============================================================
 *  El usuario ingresa N (numero de reinas / tamano del tablero)
 *  y el programa arma un tablero de N x N, coloca las reinas
 *  fila por fila probando columnas, y cuando una reina no tiene
 *  ninguna posicion segura en su fila, "retrocede" (backtrack):
 *  borra la reina anterior, la mueve a la siguiente columna
 *  disponible y sigue intentando.
 *
 *  Compilar (en la terminal de VS Code):
 *      gcc n_reinas.c -o n_reinas
 *  Ejecutar:
 *      ./n_reinas        (Linux/Mac)
 *      n_reinas.exe      (Windows)
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

int N;              // Tamano del tablero (numero de reinas)
int *tablero;        // tablero[fila] = columna donde esta la reina de esa fila
int pasos = 0;        // Contador de intentos, solo para mostrar info
int soluciones = 0;   // Contador de soluciones encontradas

/*
 * Muestra el tablero actual en pantalla.
 * Usamos "R" para la reina y "." para casillas vacias.
 * Solo se dibujan las filas que ya tienen reina colocada
 * (para poder ver el avance paso a paso).
 */
void imprimirTablero(int filaActual) {
    printf("\n");
    for (int fila = 0; fila <= filaActual; fila++) {
        for (int col = 0; col < N; col++) {
            if (tablero[fila] == col)
                printf(" R ");
            else
                printf(" . ");
        }
        printf("\n");
    }
    printf("------------------------------\n");
}

/*
 * Revisa si es seguro colocar una reina en (fila, col)
 * comparando con todas las reinas ya colocadas en filas
 * anteriores (0 .. fila-1).
 *
 * No es necesario revisar la misma fila porque solo colocamos
 * una reina por fila.
 */
int esSeguro(int fila, int col) {
    for (int f = 0; f < fila; f++) {
        int c = tablero[f];

        // Misma columna
        if (c == col)
            return 0;

        // Misma diagonal (la diferencia de filas es igual
        // a la diferencia de columnas, en valor absoluto)
        if (abs(c - col) == abs(f - fila))
            return 0;
    }
    return 1; // No hay conflicto, la posicion es segura
}

/*
 * Funcion recursiva de backtracking.
 * Intenta colocar una reina en cada columna de la fila actual.
 */
void colocarReinas(int fila, int mostrarPasos) {
    // Caso base: ya colocamos una reina en cada una de las N filas
    if (fila == N) {
        soluciones++;
        printf("\n*** SOLUCION #%d encontrada ***\n", soluciones);
        imprimirTablero(N - 1);
        return;
    }

    for (int col = 0; col < N; col++) {
        pasos++;

        if (esSeguro(fila, col)) {
            // Colocamos la reina
            tablero[fila] = col;

            if (mostrarPasos) {
                printf("\nColoco reina en fila %d, columna %d", fila, col);
                imprimirTablero(fila);
            }

            // Probamos con la siguiente fila
            colocarReinas(fila + 1, mostrarPasos);

            // BACKTRACK: si llegamos aca es porque el camino
            // anterior no funciono (o ya se imprimio una solucion
            // y seguimos buscando otras). Quitamos la reina para
            // probar la siguiente columna.
            if (mostrarPasos) {
                printf("\nRetrocedo: quito la reina de fila %d, columna %d\n",
                       fila, col);
            }
            tablero[fila] = -1;
        }
    }
}

int main() {
    printf("=========================================\n");
    printf("   PROBLEMA DE LAS N REINAS - BACKTRACKING\n");
    printf("=========================================\n\n");

    printf("Ingrese el numero de reinas (tamano del tablero N x N): ");
    if (scanf("%d", &N) != 1 || N <= 0) {
        printf("Numero invalido.\n");
        return 1;
    }

    // Reservamos memoria para el tablero segun el N ingresado
    tablero = (int *) malloc(N * sizeof(int));
    for (int i = 0; i < N; i++)
        tablero[i] = -1; // -1 significa "sin reina en esa fila"

    // Para N = 2 o N = 3 no existe solucion, el programa lo
    // detectara solo porque nunca llegara al caso base.

    char opcion;
    printf("\nQuiere ver el proceso paso a paso (colocar/retroceder)? (s/n): ");
    scanf(" %c", &opcion);
    int mostrarPasos = (opcion == 's' || opcion == 'S');

    colocarReinas(0, mostrarPasos);

    printf("\n=========================================\n");
    if (soluciones == 0) {
        printf("No existe solucion para N = %d\n", N);
    } else {
        printf("Total de soluciones encontradas: %d\n", soluciones);
    }
    printf("Total de intentos (pasos) realizados: %d\n", pasos);
    printf("=========================================\n");

    free(tablero);
    return 0;
}
