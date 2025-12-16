#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

/**
 * @file txt_a_bin.c
 * @brief Convierte un archivo de texto de libros a formato binario.
 *
 * Este programa lee un archivo de texto (`libros_biblioteca.txt`) con registros de libros
 * separados por punto y coma (formato CSV simplificado) y genera un archivo binario
 * (`libros_biblioteca.bin`) con estructuras de tipo `Libro`.
 *
 * Formato esperado en cada línea del archivo de texto:
 *     titulo;autor;anio;prestado;precio
 *
 * Campos:
 *  - titulo: hasta 99 caracteres.
 *  - autor: hasta 99 caracteres.
 *  - anio: año de publicación (entero).
 *  - prestado: cadena indicando si el libro está prestado (sí/no).
 *  - precio: valor en punto flotante (double).
 *
 * Ejemplo de línea válida:
 *     "Cien años de soledad;Gabriel García Márquez;1967;no;15.50"
 *
 * Funcionamiento:
 *  - Se leen todas las líneas del archivo de texto.
 *  - Se valida que cada línea contenga los 5 campos requeridos.
 *  - Se escriben los registros correctamente formateados en el archivo binario.
 *  - Las líneas mal formadas se omiten con aviso en stderr.
 *
 * @return 0 si la conversión se realiza correctamente, 1 en caso de error.
 */

int main(void) {
    // Abrir archivo de texto para lectura
    FILE *fin = fopen("libros_biblioteca.txt", "r");
    if (!fin) {
        perror("fopen libros_biblioteca.txt");
        return 1;
    }
    
    // Abrir archivo binario para escritura
    FILE *fout = fopen("libros_biblioteca.bin", "wb");
    if (!fout) {
        perror("fopen libros_biblioteca.bin");
        fclose(fin);
        return 1;
    }
    
    Libro L;          ///< Estructura para almacenar un registro leído
    int linea = 0;    ///< Número de línea actual del archivo de texto
    int escritos = 0; ///< Contador de registros binarios escritos
    
    // Leer línea a línea hasta fin de archivo
    while (1) {
        ++linea;

        // Inicializar la estructura a cero para garantizar terminación nula
        memset(&L, 0, sizeof(L)); 
        
        // Leer campos separados por ';'
        // %99[^;] lee hasta 99 caracteres o hasta encontrar ';' (para evitar desbordamiento)
        int n = fscanf(fin, "%99[^;];%99[^;];%d;%2[^;];%lf",
                       L.titulo, L.autor, &L.anioPublicacion, L.prestado, &L.precio);
        
        if (n == EOF) break; // Fin de archivo
        
        // Verificar que se hayan leído los 5 campos
        if (n != 5) {
            fprintf(stderr, "Linea %d mal formada (leidos=%d). Se omite.\n", linea, n);
            
            // Consumir caracteres restantes hasta el final de línea
            int c; 
            while ((c = fgetc(fin)) != '\n' && c != EOF) {}
            continue;
        }
        
        // Escribir registro en archivo binario
        if (fwrite(&L, sizeof(Libro), 1, fout) != 1) {
            perror("fwrite");
            fclose(fin);
            fclose(fout);
            return 1;
        }
        
        ++escritos;
    }
    
    // Cerrar archivos
    fclose(fin);
    fclose(fout);

    printf("OK: escritos %d registros en 'libros_biblioteca.bin'\n", escritos);
    return 0;
}
