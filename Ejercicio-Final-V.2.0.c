/**
 * @file Ejercicio-Final-V.2.0.c
 * @brief Programa que genera números aleatorios, los ordena usando inserción directa
 *        y realiza una búsqueda binaria. Incluye además un ejemplo de estructura
 *        `Asignatura`.
 *
 * Este programa demuestra:
 *   - Generación de números aleatorios.
 *   - Ordenación mediante Inserción Directa.
 *   - Búsqueda binaria sobre un vector ordenado.
 *   - Uso básico de estructuras en C.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief Estructura que representa una asignatura universitaria.
 *
 * Contiene el nombre de la asignatura, su número de créditos
 * y el curso en el que se imparte.
 */
typedef struct {
    char nombre[64];   /**< Nombre completo de la asignatura */
    int creditos;      /**< Número de créditos ECTS */
    int curso;         /**< Curso académico en el que se imparte */
} Asignatura;

/**
 * @brief Ordena un vector de números en orden ascendente usando el método
 *        de Inserción Directa.
 *
 * Este algoritmo compara cada elemento con los anteriores y lo sitúa
 * en su posición correcta desplazando los mayores hacia la derecha.
 *
 * @param v Puntero al vector de números reales.
 * @param n Número de elementos del vector.
 */
void ordenacion(double *v, int n) {
    int i, j;
    double aux;

    for (i = 0; i < n; i++) {
        aux = v[i];
        j = i;

        while (j > 0 && v[j - 1] > aux) {
            v[j] = v[j - 1];
            j--;
        }

        v[j] = aux;
    }
}

/**
 * @brief Realiza una búsqueda binaria sobre un vector ordenado.
 *
 * El usuario debe introducir el valor que quiere buscar. Si el valor
 * se encuentra en el vector, se muestra la posición real (comenzando en 1).
 *
 * @param v Puntero al vector ordenado.
 * @param n Número de elementos del vector.
 * @param x Valor que se desea buscar en el vector.
 */
void binaria(double *v, int n, double x) {
    int izq = 0;
    int der = n - 1;
    int cen;
    int encontrado = 0;

    while (izq <= der) {
        cen = (izq + der) / 2;

        if (v[cen] == x) {
            printf("Valor %.2f encontrado en la posicion real %d\n", x, cen + 1);
            encontrado = 1;
            break;
        }

        if (v[cen] > x)
            der = cen - 1;
        else
            izq = cen + 1;
    }

    if (!encontrado)
        printf("El valor %.2f NO se encuentra en el vector.\n", x);
}

/**
 * @brief Genera números aleatorios en el rango [1, 100].
 *
 * Llena el vector con valores aleatorios enteros convertidos a `double`.
 *
 * @param v Vector donde se almacenarán los números generados.
 * @param n Cantidad de números a generar.
 */
void generar_num(double *v, int n) {
    for (int i = 0; i < n; i++) {
        v[i] = rand() % 100 + 1;
    }
}

/**
 * @brief Imprime en pantalla el contenido de un vector de números reales.
 *
 * Muestra los valores con dos decimales separados por espacio.
 *
 * @param v Vector de números reales.
 * @param n Número de elementos del vector.
 */
void imprimir(double *v, int n) {
    for (int i = 0; i < n; i++) {
        printf("%.2f ", v[i]);
    }
    printf("\n");
}

/**
 * @brief Función principal del programa.
 *
 * Ejecuta los siguientes pasos:
 *   1. Generación de un vector de números aleatorios.
 *   2. Impresión del vector original.
 *   3. Ordenación mediante Inserción Directa.
 *   4. Impresión del vector ordenado.
 *   5. Solicitud al usuario de un número a buscar.
 *   6. Búsqueda binaria del número indicado.
 *   7. Declaración de un vector de estructuras `Asignatura` como ejemplo.
 *
 * @return 0 si el programa finaliza correctamente.
 */
int main() {

    srand(time(NULL));

    int n = 10;
    double v[n];

    generar_num(v, n);

    printf("Numeros antes de ordenar:\n");
    imprimir(v, n);

    ordenacion(v, n);

    printf("\nNumeros despues de ordenar:\n");
    imprimir(v, n);

    double buscar;
    printf("\nIntroduce un numero a buscar: ");
    scanf("%lf", &buscar);

    binaria(v, n, buscar);

    /* Ejemplo de estructura Asignatura */
    Asignatura asignatura[3] = {
        {"Programacion", 6, 1},
        {"Algebra", 6, 1},
        {"Calculo", 6, 1}
    };

    return 0;
}

