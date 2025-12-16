#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"  // Contiene la definición de la estructura Libro, NOMBRE_ARCHIVO y CAD_TITULO

/**
 * @brief Realiza una búsqueda lineal de un libro en el archivo binario por título.
 *
 * La función abre el archivo binario definido en NOMBRE_ARCHIVO y recorre
 * secuencialmente todos los registros (estructura Libro) hasta encontrar
 * un título que coincida exactamente con el proporcionado por el usuario.
 *
 * Si el título es encontrado, muestra su información completa (título, autor y estado de préstamo)
 * y la posición del registro en el archivo. Si no se encuentra, informa al usuario.
 *
 * @param titulo_buscado Cadena de texto con el título a buscar.
 */
void busqueda_lineal_archivo(const char *titulo_buscado) {
    FILE *archivo = NULL;
    Libro libro_actual;              // Estructura para almacenar temporalmente cada registro leído
    long posicion_registro = 0;      // Contador de posición (índice del registro dentro del archivo)
    size_t elementos_leidos;         // Controla la cantidad de registros leídos con fread()

    // 1. Abrir el archivo en modo lectura binaria
    archivo = fopen(NOMBRE_ARCHIVO, "rb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo binario");
        return;
    }

    printf("Buscando el título: \"%s\" en el archivo...\n", titulo_buscado);

    // 2. Leer secuencialmente los registros hasta el final del archivo
    // Implementación clásica del algoritmo de búsqueda lineal
    while ((elementos_leidos = fread(&libro_actual, sizeof(Libro), 1, archivo)) == 1) {

        // 3. Comparar el título del registro actual con el buscado (strcmp retorna 0 si son iguales)
        if (strcmp(libro_actual.titulo, titulo_buscado) == 0) {
            
            // 4. Mostrar la información del libro encontrado
            printf("----------------------------------------\n");
            printf("¡LIBRO ENCONTRADO!\n");
            printf("Posición del registro (índice 0): %ld\n", posicion_registro); 
            printf("Título: %s\n", libro_actual.titulo);
            printf("Autor: %s\n", libro_actual.autor);
            printf("Estado Préstamo: %s\n", libro_actual.prestado);
            printf("----------------------------------------\n");

            fclose(archivo);
            return; // Se termina la función al encontrar el libro
        }

        // Incrementar el índice para el siguiente registro
        posicion_registro++;
    }

    // 5. Evaluar si la lectura terminó correctamente o por error
    if (feof(archivo)) {
        printf("----------------------------------------\n");
        printf("BÚSQUEDA FINALIZADA: El título \"%s\" no fue encontrado.\n", titulo_buscado);
        printf("----------------------------------------\n");
    } else if (ferror(archivo)) {
        perror("Error de lectura durante la búsqueda");
    }

    fclose(archivo);
}

/**
 * @brief Programa principal para ejecutar la búsqueda de un libro por título.
 *
 * Solicita al usuario un título de libro por consola, elimina el salto de línea
 * generado por fgets() y llama a la función busqueda_lineal_archivo() para
 * realizar la búsqueda en el archivo binario.
 *
 * @return int Devuelve 0 si la ejecución fue correcta, o 1 si hubo error de lectura.
 */
int main(void) {

    char titulo[CAD_TITULO]; // Buffer para almacenar el título introducido por el usuario

    printf("=======================================\n");
    printf("   BÚSQUEDA DE LIBRO EN ARCHIVO BINARIO\n");
    printf("=======================================\n");

    printf("Introduzca el TÍTULO a buscar (máx. %d caracteres): \n", CAD_TITULO - 1);
    
    // Lectura segura del título desde stdin
    if (fgets(titulo, CAD_TITULO, stdin) == NULL) {
        printf("Error de lectura.\n");
        return 1;
    }
    
    // Eliminar el salto de línea '\n' que fgets deja al final
    titulo[strcspn(titulo, "\n")] = 0; 
    
    // Llamada a la función de búsqueda
    busqueda_lineal_archivo(titulo);

    return 0;
}
