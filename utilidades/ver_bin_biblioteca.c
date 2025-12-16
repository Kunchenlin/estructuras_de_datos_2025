#include "common.h"

/**
 * @file ver_bin_biblioteca.c
 * @brief Funciones para visualizar el contenido de un archivo binario de libros.
 *
 * Este módulo proporciona la función `mostrar_libros` que permite imprimir en pantalla
 * todos los registros de un archivo binario de tipo `Libro`. La salida se muestra
 * en formato tabular con columnas fijas:
 * 
 *  POS | AÑO | TÍTULO | AUTOR | PRESTADO | PRECIO
 *
 * Uso típico:
 * @code
 * mostrar_libros("libros_biblioteca.bin");
 * @endcode
 *
 * Requisitos:
 *  - El archivo binario debe existir y contener registros válidos de tipo `Libro`.
 *  - Incluye la cabecera `common.h` donde se define la estructura `Libro`.
 *
 * Funciones principales:
 *  - `void mostrar_libros(const char *nombreArchivo)`: Muestra en pantalla todos los libros.
 */

void mostrar_libros(const char *nombreArchivo) {
    // Abrir archivo en modo lectura binaria
    FILE *f = fopen(nombreArchivo, "rb");
    if (!f) { 
        printf("¡ERROR! Archivo '%s' no encontrado. Asegúrate de que existe.\n", nombreArchivo);
        return; 
    }

    Libro libro;  ///< Variable para leer cada registro
    // Separador para la tabla (~86 caracteres)
    const char *separador = "--------------------------------------------------------------------------------------";
    
    // Imprimir cabecera de la tabla
    printf("%s\n", separador);
    printf("%-5s | %-5s | %-35s | %-20s | %-8s | %-8s |\n", 
           "POS", "AÑO", "TÍTULO", "AUTOR", "PREST.", "PRECIO");
    printf("%s\n", separador);

    int i = 0;  ///< Contador de registros
    // Leer registros hasta fin de archivo
    while (fread(&libro, sizeof(Libro), 1, f) == 1) {
        // Imprimir cada libro en fila con columnas alineadas
        printf("%-5d | %-5d | %-35s | %-20s | %-8s | %-8.2f |\n",
               ++i, 
               libro.anioPublicacion, 
               libro.titulo, 
               libro.autor, 
               libro.prestado, 
               libro.precio);
    }

    // Imprimir línea final y cerrar archivo
    printf("%s\n", separador);
    fclose(f);
}
