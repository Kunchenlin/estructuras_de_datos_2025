/**
 * @file listar_prestado.c
 * @brief Lista los libros de un archivo binario según su estado de préstamo ("Si" o "No").
 *
 * Este programa permite consultar los registros almacenados en el archivo binario
 * `libros_biblioteca.bin` y muestra en pantalla aquellos que coincidan con el
 * estado de préstamo indicado por el usuario.
 *
 * Utiliza la estructura `Libro` definida en `common.h`.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> ///< Para usar strcasecmp() (comparación sin distinción entre mayúsculas y minúsculas)
#include "common.h"  ///< Incluye la definición de la estructura Libro y constantes globales

/**
 * @brief Imprime una línea horizontal de separación.
 *
 * Esta función se utiliza para mejorar la legibilidad del formato de salida en consola.
 * Genera una línea de 90 caracteres con el carácter '-'.
 */
void imprimir_separador(void) {
    for (int i = 0; i < 90; ++i) {
        putchar('-');
    }
    putchar('\n');
}

/**
 * @brief Lista los libros en función de su estado de préstamo ("Si"/"No").
 *
 * Esta función solicita al usuario un criterio ("Si" para libros prestados,
 * "No" para los disponibles) y recorre el archivo binario `libros_biblioteca.bin`
 * mostrando únicamente los libros cuyo campo `prestado` coincide con el criterio.
 *
 * El formato de salida se muestra en una tabla con columnas:
 * - POS: posición del libro en el resultado
 * - AUTOR
 * - TÍTULO
 * - PRESTADO
 *
 * La comparación de cadenas se realiza sin distinguir mayúsculas ni minúsculas.
 */
void listar_por_prestado(void) {
    FILE *archivo = NULL;             ///< Puntero al archivo binario
    Libro libro_actual;               ///< Variable temporal para almacenar cada registro leído
    char respuesta_usuario[5];        ///< Respuesta del usuario ("Si" o "No")
    int libros_encontrados = 0;       ///< Contador de resultados encontrados
    size_t elementos_leidos;          ///< Control del número de elementos leídos por fread()

    // 1. Mostrar menú e introducir el criterio de búsqueda
    printf("========================================================================================\n");
    printf("                                LISTADO DE LIBROS POR ESTADO DE PRÉSTAMO\n");
    printf("========================================================================================\n");
    printf("¿Qué libros desea listar?\n");
    printf("  [Si] Libros Prestados\n");
    printf("  [No] Libros No Prestados\n");
    printf("  Respuesta (Si/No): ");

    // Lectura segura de la respuesta del usuario
    if (fgets(respuesta_usuario, sizeof(respuesta_usuario), stdin) == NULL) {
        printf("\nERROR de lectura de la entrada.\n");
        return;
    }

    // Elimina el salto de línea al final de la cadena
    chomp(respuesta_usuario);

    // 2. Validar la respuesta ingresada (solo se aceptan "Si" o "No")
    if (strcasecmp(respuesta_usuario, "si") != 0 && strcasecmp(respuesta_usuario, "no") != 0) {
        printf("\nERROR: Respuesta no válida. Debe introducir 'Si' o 'No'.\n");
        return;
    }

    // 3. Abrir el archivo binario en modo lectura
    archivo = fopen(NOMBRE_ARCHIVO, "rb");
    if (archivo == NULL) {
        perror("\nERROR al abrir el archivo binario " NOMBRE_ARCHIVO);
        return;
    }

    // Cabecera de resultados
    printf("\nListando libros con estado: %s\n", respuesta_usuario);
    imprimir_separador();
    printf("| %-5s | %-30s | %-30s | %-10s |\n", "POS", "AUTOR", "TÍTULO", "PRESTADO");
    imprimir_separador();

    // 4. Recorrer todos los registros del archivo binario
    while ((elementos_leidos = fread(&libro_actual, sizeof(Libro), 1, archivo)) == 1) {

        // Comparar el campo prestado con la respuesta del usuario
        if (strcasecmp(libro_actual.prestado, respuesta_usuario) == 0) {
            printf("| %-5d | %-30s | %-30s | %-10s |\n",
                   libros_encontrados + 1,
                   libro_actual.autor,
                   libro_actual.titulo,
                   libro_actual.prestado);

            libros_encontrados++;
        }
    }

    // 5. Cerrar archivo y mostrar resumen
    fclose(archivo);
    imprimir_separador();

    if (libros_encontrados == 0) {
        printf("No se encontraron libros con el estado seleccionado (%s).\n", respuesta_usuario);
    } else {
        printf("Total de libros encontrados: %d\n", libros_encontrados);
    }

    printf("========================================================================================\n");
}

/**
 * @brief Función principal del programa.
 *
 * Llama a la función `listar_por_prestado()` para mostrar los libros según su estado de préstamo.
 *
 * @return 0 si el programa se ejecuta correctamente.
 */
int main(void) {
    listar_por_prestado();
    return 0;
}
