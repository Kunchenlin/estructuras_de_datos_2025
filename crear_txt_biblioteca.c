/**
 * @file crear_txt_biblioteca.c
 * @brief Programa que crea un archivo de texto con información de libros introducidos por el usuario.
 *
 * Este programa permite registrar varios libros, introduciendo sus datos por teclado,
 * y los guarda en el archivo `libros_biblioteca.txt` con formato:
 * `titulo;autor;anioPublicacion;prestado;precio`
 *
 * Usa la estructura `Libro` definida en `common.h`.
 */

#include <stdio.h>
#include "common.h"

/**
 * @brief Limpia el buffer de entrada estándar después de una lectura con `scanf()`.
 *
 * Esta función consume todos los caracteres restantes en el buffer de entrada
 * hasta encontrar un salto de línea ('\\n') o el final del archivo (`EOF`).
 * Se utiliza para evitar que queden caracteres pendientes antes de una lectura
 * con `fgets()` u otra función similar.
 */
void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Función principal del programa.
 *
 * Solicita al usuario la cantidad de libros a registrar y, para cada uno,
 * pide los datos básicos (título, autor, año de publicación, estado de préstamo y precio).
 * Luego, los guarda en el archivo de texto `libros_biblioteca.txt`.
 *
 * @return 0 si el programa finaliza correctamente, o 1 si ocurre un error.
 */
int main(void) {
    int N; ///< Número de libros a registrar.

    printf("Cantidad de libros: ");

    // Se solicita el número de libros a introducir.
    // Si el valor no es válido (no numérico o <= 0), se muestra error.
    if (scanf("%d", &N) != 1 || N <= 0) {
        fprintf(stderr, "N invalido\n");
        return 1;
    }

    getchar(); // Consume el salto de línea dejado por scanf()

    // Se abre el archivo de texto en modo escritura ("w").
    // Si no se puede abrir, se muestra un error.
    FILE *f = fopen("libros_biblioteca.txt", "w");
    if (!f) { 
        perror("fopen libros_biblioteca.txt"); 
        return 1; 
    }

    // Bucle principal para registrar cada libro
    for (int i = 0; i < N; ++i) {
        Libro L = {0}; ///< Estructura para almacenar los datos del libro actual.

        printf("\n[%d/%d] Título: ", i + 1, N);
        fgets(L.titulo, CAD_TITULO, stdin);
        chomp(L.titulo); // Elimina el salto de línea final (si lo hay)
        limpiar_buffer(); // Limpia caracteres sobrantes en el buffer

        printf("Autor: ");
        fgets(L.autor, CAD_AUTOR, stdin);
        chomp(L.autor);

        printf("Año publicación: ");
        scanf("%d", &L.anioPublicacion);
        limpiar_buffer();

        printf("Prestado (Sí/No): ");
        fgets(L.prestado, sizeof L.prestado, stdin);
        chomp(L.prestado);

        printf("Precio (euros): ");
        scanf("%lf", &L.precio);
        limpiar_buffer();

        // Escribe la información del libro en el archivo con formato separado por ';'
        fprintf(f, "%s;%s;%d;%s;%.2lf\n",
                L.titulo, L.autor, L.anioPublicacion, L.prestado, L.precio);

        printf("-> Guardado.\n");
    }

    // Cierre del archivo
    fclose(f);

    printf("\nOK: creado 'libros_biblioteca.txt'\n");
    return 0;
}
