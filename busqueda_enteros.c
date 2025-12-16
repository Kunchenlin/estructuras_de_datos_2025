/**
 * @file busqueda_enteros.c
 * @brief Implementación y pruebas de búsqueda binaria sobre un vector de enteros ordenado.
 *
 * Se compara un valor X con el elemento central del intervalo. 
 * En cada iteración se reduce a la mitad el intervalo, midiendo comparaciones.
 */

#include <stdio.h>

#define TAM 10

/**
 * @brief Crea el vector ordenado utilizado en las pruebas.
 * @param v Vector donde se almacenarán los valores.
 */
void crear_vector(int v[]) {
    int datos[TAM] = {3, 7, 12, 18, 21, 25, 30, 34, 42, 50};
    for (int i = 0; i < TAM; i++) {
        v[i] = datos[i];
    }
}

/**
 * @brief Imprime el contenido del vector.
 * @param v Vector de enteros.
 */
void imprimir_vector(int v[]) {
    printf("Contenido del vector:\n");
    for (int i = 0; i < TAM; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
}

/**
 * @brief Realiza una búsqueda binaria sobre un vector ordenado.
 *
 * Cuenta cada comparación entre V[cen] y X.
 *
 * @param V Vector donde se busca.
 * @param N Tamaño del vector.
 * @param X Valor a buscar.
 * @param comps Referencia donde se acumulan las comparaciones.
 * @return Índice donde se encontró X o -1 si no está.
 */
int busqueda_binaria(const int V[], int N, int X, int *comps) {
    int izq = 0;
    int der = N - 1;
    *comps = 0;

    while (izq <= der) {
        int cen = (izq + der) / 2;
        (*comps)++;

        if (V[cen] == X)
            return cen;
        else if (V[cen] > X)
            der = cen - 1;
        else
            izq = cen + 1;
    }

    return -1;
}

int main() {
    int V[TAM];
    crear_vector(V);
    imprimir_vector(V);

    int valores[] = {18, 3, 50, 19, -5, 99};
    int comps, idx;

    printf("\nRESULTADOS BÚSQUEDA BINARIA (ENTEROS)\n");
    printf("Valor\tÍndice\tComparaciones\n");
    printf("-----------------------------------\n");

    for (int i = 0; i < 6; i++) {
        idx = busqueda_binaria(V, TAM, valores[i], &comps);
        printf("%4d\t%4d\t%6d\n", valores[i], idx, comps);
    }

    return 0;
}

