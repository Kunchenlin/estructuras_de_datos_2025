/**
 * @file ordenacion_binaria.c
 * @brief Ejemplo de ordenación, búsqueda binaria interactiva y manejo de estructuras.
 *
 * El programa genera un vector de números aleatorios, los ordena y permite
 * al usuario introducir un valor para realizar una búsqueda binaria.
 */

#include <stdio.h> 
#include <stdlib.h>
#include <time.h> 

/**
 * @brief Estructura que representa una asignatura con su nombre,
 *        número de créditos y curso correspondiente.
 */
typedef struct{
    char nombre[64]; 
    int creditos; 
    int curso; 
} Asignatura; 


/**
 * @brief Ordena un vector mediante el método de inserción directa.
 *
 * @param v Vector de números a ordenar.
 * @param n Cantidad de elementos del vector.
 */
void ordenacion(double *v, int n){
    int i, j; 
    double aux; 
    
    for (i = 0; i < n; i++){
        aux = v[i];
        j = i; 
        
        while (j > 0 && v[j-1] > aux){
            v[j] = v[j-1]; 
            j--; 
        }
        
        v[j] = aux;
    }
}


/**
 * @brief Realiza una búsqueda binaria sobre un vector ordenado.
 *
 * @param v Vector ordenado donde se realiza la búsqueda.
 * @param n Número de elementos del vector.
 * @param x Valor a buscar.
 *
 * @return int Devuelve la posición donde se encontró el valor,
 *             o -1 si el valor no se encuentra.
 */
int binaria(double *v, int n, double x){

    int izq = 0, der = n - 1;
    int cen;

    while (izq <= der){
        cen = (izq + der) / 2;

        if (v[cen] == x){
            return cen;
        }
        else if (v[cen] > x){
            der = cen - 1;
        }
        else{
            izq = cen + 1;
        }
    }
    return -1;
}


/**
 * @brief Genera números aleatorios entre 1 y 100.
 *
 * @param v Vector donde se almacenarán los números generados.
 * @param n Cantidad de elementos a generar.
 */
void generar_num(double *v, int n){
    for (int i = 0; i < n; i++){
        v[i] = rand() % 100 + 1; 
    }
}


/**
 * @brief Imprime en pantalla los valores de un vector.
 *
 * @param v Vector a imprimir.
 * @param n Número de elementos del vector.
 */
void imprimir(double *v, int n){
    for (int i = 0; i < n; i++){
        printf("%.2f\n", v[i]); 
    }
    printf("\n");
}


/**
 * @brief Función principal del programa.
 *
 * Genera un vector aleatorio, lo ordena, lo muestra en pantalla, solicita
 * al usuario un valor para buscar y ejecuta la búsqueda binaria.
 *
 * @return int Devuelve 0 si el programa finaliza correctamente.
 */
int main () {
    
    srand(time(NULL));

    int n = 10; 
    double v[n]; 
    double valor_buscar;

    generar_num(v, n); 
    
    printf("Numeros antes de ordenar:\n"); 
    imprimir(v, n); 
    
    ordenacion(v, n); 
    
    printf("Numeros despues de ordenar:\n"); 
    imprimir(v, n); 

    /** Solicitar valor al usuario **/
    printf("Introduce un valor para buscar: ");
    scanf("%lf", &valor_buscar);

    int pos = binaria(v, n, valor_buscar);

    if (pos != -1){
        printf("Valor encontrado en la posicion %d\n", pos + 1);
    } else {
        printf("El valor NO se encuentra en el vector.\n");
    }

    /** Ejemplo de uso de la estructura Asignatura */
    Asignatura asignatura[3] = {
        {"Programacion", 6, 1}, 
        {"Algebra", 6, 1}, 
        {"Calculo", 6, 1}    
    };

    return 0;
}

