/**
 * @file ordenar_precio_insercion.c
 * @brief Ordenación de archivos binarios de libros por año mediante mezcla natural.
 *
 * Este programa implementa la técnica de clasificación por mezcla natural para
 * ordenar registros de tipo `Libro` almacenados en un archivo binario.
 * La ordenación se realiza sin cargar todos los registros en memoria, usando
 * dos archivos auxiliares para dividir y mezclar secuencias ordenadas.
 *
 * Funcionalidades principales:
 *  - Separación del archivo original en secuencias ordenadas alternas.
 *  - Mezcla de las secuencias mediante archivos auxiliares hasta obtener
 *    un archivo completamente ordenado.
 *  - Carga y visualización de los libros ordenados.
 *
 * Archivos auxiliares:
 *  - AUX1 ("aux1.bin"): primer archivo temporal para secuencias.
 *  - AUX2 ("aux2.bin"): segundo archivo temporal para secuencias.
 *  - ARCHIVO_ORDENADO ("libros_biblioteca_anio.bin"): archivo final ordenado.
 *
 * Autor: Kun Chen Lin
 * Fecha: 17/11/2025
 * Versión: 1.0
 *
 * Requisitos:
 *  - common.h: definición de la estructura `Libro` y macros de archivo.
 *
 * Ejemplo de uso:
 *  Ejecutar el programa para ordenar 'biblioteca.bin' y mostrar resultados:
 *      ./ordenar_precio_insercion.out
 */




#include "common.h"

#define AUX1 "aux1.bin"                  ///< Nombre del primer archivo auxiliar
#define AUX2 "aux2.bin"                  ///< Nombre del segundo archivo auxiliar
#define ARCHIVO_ORDENADO "libros_biblioteca_anio.bin" ///< Archivo final ordenado
#define NOMBRE_ARCHIVO "biblioteca.bin"  ///< Archivo original a ordenar

// ------------------------------------------------------
// Prototipos de funciones
// ------------------------------------------------------

/**
 * @brief Ordena un archivo binario de libros mediante el método de mezcla natural.
 * @param NOMBRE_F Archivo principal.
 * @param NOMBRE_F1 Primer archivo auxiliar.
 * @param NOMBRE_F2 Segundo archivo auxiliar.
 */
void clasificacion_mezcla_natural(const char *NOMBRE_F,
                                  const char *NOMBRE_F1,
                                  const char *NOMBRE_F2);

/**
 * @brief Divide el archivo original en dos archivos auxiliares según las secuencias ordenadas.
 * @param NOMBRE_F Archivo principal.
 * @param NOMBRE_F1 Primer archivo auxiliar.
 * @param NOMBRE_F2 Segundo archivo auxiliar.
 */
void separar(const char *NOMBRE_F, const char *NOMBRE_F1, const char *NOMBRE_F2);

/**
 * @brief Mezcla las subsecuencias ordenadas de los archivos auxiliares en un archivo de salida.
 * @param NOMBRE_F Archivo destino de la mezcla.
 * @param NOMBRE_F1 Primer archivo auxiliar.
 * @param NOMBRE_F2 Segundo archivo auxiliar.
 * @return Número de subsecuencias mezcladas.
 */
int mezcla_natural(const char *NOMBRE_F,
                   const char *NOMBRE_F1,
                   const char *NOMBRE_F2);

/**
 * @brief Carga todos los libros de un archivo binario en memoria.
 * @param nombre_f Archivo binario de entrada.
 * @param n Puntero a variable donde se devuelve el número de libros cargados.
 * @return Puntero a un arreglo dinámico de libros. NULL si no hay libros o error.
 */
Libro *cargar_libros(const char *nombre_f, int *n);

/**
 * @brief Cuenta el número de libros en un archivo binario.
 * @param nombre_f Archivo binario.
 * @return Número de registros de tipo Libro.
 */
int contar_libros(const char *nombre_f);

// ------------------------------------------------------
// Función principal
// ------------------------------------------------------

/**
 * @brief Función principal que ordena un archivo de libros por año
 *        usando mezcla natural y muestra el resultado.
 * @return 0 si todo va bien.
 */
int main(void) {
    printf("Ordenando archivo '%s' por año de publicación...\n", NOMBRE_ARCHIVO);

    // Ordenar archivo
    clasificacion_mezcla_natural(NOMBRE_ARCHIVO, AUX1, AUX2);

    printf("\nArchivo ordenado guardado como '%s'\n", ARCHIVO_ORDENADO);

    // Mostrar el resultado
    int n;
    Libro *libros = cargar_libros(ARCHIVO_ORDENADO, &n);
    if (libros && n > 0) {
        printf("\nLibros ordenados por año:\n\n");
        mostrar_libros(libros, n);
        free(libros);
    } else {
        printf("No se pudieron cargar los libros ordenados.\n");
    }

    return 0;
}

// ------------------------------------------------------
// Implementación de mezcla natural
// ------------------------------------------------------

void clasificacion_mezcla_natural(const char *NOMBRE_F,
                                  const char *NOMBRE_F1,
                                  const char *NOMBRE_F2)
{
    int L;

    do {
        separar(NOMBRE_F, NOMBRE_F1, NOMBRE_F2);        ///< Separa en auxiliares
        L = mezcla_natural(ARCHIVO_ORDENADO, NOMBRE_F1, NOMBRE_F2); ///< Mezcla las secuencias
        rename(ARCHIVO_ORDENADO, NOMBRE_F);            ///< Reemplaza archivo original
    } while (L > 1);

    rename(NOMBRE_F, ARCHIVO_ORDENADO);               ///< Archivo final ordenado
}

void separar(const char *NOMBRE_F, const char *NOMBRE_F1, const char *NOMBRE_F2) {
    FILE *f = fopen(NOMBRE_F, "rb");
    FILE *f1 = fopen(NOMBRE_F1, "wb");
    FILE *f2 = fopen(NOMBRE_F2, "wb");

    if (!f || !f1 || !f2) {
        perror("Error abriendo archivos en separar()");
        exit(EXIT_FAILURE);
    }

    Libro actual, anterior;
    int toggle = 0; ///< Alterna entre archivo f1 y f2

    if (fread(&anterior, sizeof(Libro), 1, f)) {
        fwrite(&anterior, sizeof(Libro), 1, toggle ? f2 : f1);
        while (fread(&actual, sizeof(Libro), 1, f)) {
            if (actual.anioPublicacion < anterior.anioPublicacion)
                toggle = !toggle; ///< Detecta ruptura de secuencia
            fwrite(&actual, sizeof(Libro), 1, toggle ? f2 : f1);
            anterior = actual;
        }
    }

    fclose(f);
    fclose(f1);
    fclose(f2);
}

int mezcla_natural(const char *NOMBRE_F,
                   const char *NOMBRE_F1,
                   const char *NOMBRE_F2)
{
    FILE *f = fopen(NOMBRE_F, "wb");
    FILE *f1 = fopen(NOMBRE_F1, "rb");
    FILE *f2 = fopen(NOMBRE_F2, "rb");

    if (!f || !f1 || !f2) {
        perror("Error abriendo archivos en mezcla_natural()");
        exit(EXIT_FAILURE);
    }

    Libro a, b;
    int hayA = fread(&a, sizeof(Libro), 1, f1);
    int hayB = fread(&b, sizeof(Libro), 1, f2);
    int L = 0;

    while (hayA || hayB) {
        L++; ///< Cada mezcla forma una subsecuencia ordenada

        // Mezcla hasta que se acaben los elementos de ambos
        while (hayA && hayB) {
            if (a.anioPublicacion <= b.anioPublicacion) {
                fwrite(&a, sizeof(Libro), 1, f);
                hayA = fread(&a, sizeof(Libro), 1, f1);
            } else {
                fwrite(&b, sizeof(Libro), 1, f);
                hayB = fread(&b, sizeof(Libro), 1, f2);
            }
        }

        // Copia colas restantes
        while (hayA) {
            fwrite(&a, sizeof(Libro), 1, f);
            hayA = fread(&a, sizeof(Libro), 1, f1);
        }
        while (hayB) {
            fwrite(&b, sizeof(Libro), 1, f);
            hayB = fread(&b, sizeof(Libro), 1, f2);
        }

        hayA = fread(&a, sizeof(Libro), 1, f1);
        hayB = fread(&b, sizeof(Libro), 1, f2);
    }

    fclose(f);
    fclose(f1);
    fclose(f2);
    return L;
}

// ------------------------------------------------------
// Funciones auxiliares
// ------------------------------------------------------

int contar_libros(const char *nombre_f) {
    FILE *f = fopen(nombre_f, "rb");
    if (!f) return 0;
    fseek(f, 0, SEEK_END);
    long bytes = ftell(f);
    fclose(f);
    return (int)(bytes / sizeof(Libro));
}

Libro *cargar_libros(const char *nombre_f, int *n) {
    *n = contar_libros(nombre_f);
    if (*n == 0) return NULL;

    Libro *V = malloc(sizeof(Libro) * (*n));
    if (!V) {
        perror("Error al reservar memoria para libros");
        exit(EXIT_FAILURE);
    }

    FILE *f = fopen(nombre_f, "rb");
    if (!f) {
        perror("Error al abrir archivo para leer libros");
        free(V);
        *n = 0;
        return NULL;
    }

    fread(V, sizeof(Libro), *n, f);
    fclose(f);
    return V;
}
