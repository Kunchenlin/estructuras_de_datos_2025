/**
 * @file busqueda_struct.c
 * @brief Búsqueda binaria sobre un vector de structs "Alumno" ordenado por id.
 */

#include <stdio.h>
#include <string.h>

/**
 * @struct Alumno
 * @brief Representa un alumno con id, nombre y nota.
 */
typedef struct {
    int id;            /**< Clave por la que se ordena y busca */
    char nombre[30];
    float nota;
} Alumno;

/**
 * @brief Compara el id de un alumno con un id dado.
 *
 * @param a Alumno a comparar.
 * @param id ID buscado.
 * @return <0 si a.id < id, 0 si son iguales, >0 si a.id > id.
 */
int cmp_id(const Alumno *a, int id) {
    return a->id - id;
}

/**
 * @brief Búsqueda binaria sobre un array de alumnos.
 *
 * @param A Array de alumnos.
 * @param N Tamaño del array.
 * @param id ID que se busca.
 * @param comps Referencia al contador de comparaciones.
 * @return Índice del alumno con ese id o -1 si no existe.
 */
int binsearch_alumno(const Alumno A[], int N, int id, int *comps) {
    int izq = 0, der = N - 1;
    *comps = 0;

    while (izq <= der) {
        int cen = (izq + der) / 2;
        (*comps)++;

        int r = cmp_id(&A[cen], id);
        if (r == 0)
            return cen;
        else if (r > 0)
            der = cen - 1;
        else
            izq = cen + 1;
    }

    return -1;
}

int main() {
    Alumno lista[10] = {
        {10, "Ana",    7.5},
        {14, "Luis",   6.0},
        {20, "María",  8.2},
        {23, "Jorge",  5.9},
        {27, "Lucía",  9.1},
        {31, "Carlos", 6.7},
        {35, "Elena",  7.0},
        {44, "Rosa",   8.8},
        {47, "Pablo",  5.5},
        {60, "Sofía",  9.5}
    };

    int ids[] = {23, 10, 60, 15, 5, 99};
    int comps, idx;

    printf("\nRESULTADOS BÚSQUEDA BINARIA (ALUMNOS)\n");
    printf("ID\tÍndice\tComparaciones\n");
    printf("-----------------------------------\n");

    for (int i = 0; i < 6; i++) {
        idx = binsearch_alumno(lista, 10, ids[i], &comps);
        printf("%2d\t%4d\t%6d\n", ids[i], idx, comps);
    }

    return 0;
}

