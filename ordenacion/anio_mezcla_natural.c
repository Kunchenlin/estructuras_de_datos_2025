/**
 * @file mezcla_natural.c
 * @brief Implementación de la ordenación de un archivo de libros mediante mezcla natural.
 *
 * Este módulo permite ordenar un archivo binario de registros de tipo `Libro` 
 * por año de publicación usando el método de mezcla natural. 
 * Se crean archivos auxiliares para manejar secuencias parciales ordenadas.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "common.h" // Define la estructura Libro y NOMBRE_ARCHIVO

#define AUX1 "aux1.bin" /**< Archivo auxiliar 1 */
#define AUX2 "aux2.bin" /**< Archivo auxiliar 2 */

// ------------------------------------------------------
// PROTOTIPOS DE FUNCIONES
// ------------------------------------------------------

/**
 * @brief Controla todo el proceso de ordenación por mezcla natural.
 * @param NOMBRE_F Archivo principal a ordenar.
 * @param NOMBRE_F1 Archivo auxiliar 1.
 * @param NOMBRE_F2 Archivo auxiliar 2.
 */
void clasificacion_mezcla_natural(const char *NOMBRE_F, const char *NOMBRE_F1, const char *NOMBRE_F2);

/**
 * @brief Separa el archivo principal en dos archivos auxiliares con secuencias ordenadas.
 * @param NOMBRE_F Archivo principal.
 * @param NOMBRE_F1 Archivo auxiliar 1.
 * @param NOMBRE_F2 Archivo auxiliar 2.
 *
 * Cada secuencia termina cuando se detecta un retroceso en el año de publicación.
 */
void separar(const char *NOMBRE_F, const char *NOMBRE_F1, const char *NOMBRE_F2);

/**
 * @brief Copia una secuencia ordenada desde un archivo origen a uno destino.
 * @param F_O Archivo origen.
 * @param F_D Archivo destino.
 * @param REG Registro actual de tipo Libro.
 */
void copiar_secuencia(FILE *F_O, FILE *F_D, Libro *REG);

/**
 * @brief Lee el siguiente registro y determina si continúa la secuencia.
 * @param FICH Archivo de lectura.
 * @param REG Registro actual que se actualizará.
 * @param PARAR Puntero a variable bool que se establece en true si la secuencia termina.
 */
void leer_y_chequear(FILE *FICH, Libro *REG, bool *PARAR);

/**
 * @brief Mezcla secuencias alternas de los archivos auxiliares en el archivo principal.
 * @param NOMBRE_F Archivo principal de salida.
 * @param NOMBRE_F1 Archivo auxiliar 1.
 * @param NOMBRE_F2 Archivo auxiliar 2.
 * @return Número de secuencias resultantes tras la mezcla.
 */
int mezcla_natural(const char *NOMBRE_F, const char *NOMBRE_F1, const char *NOMBRE_F2);

/**
 * @brief Muestra en pantalla el contenido de un archivo binario de libros.
 * @param nombreArchivo Archivo binario que contiene registros Libro.
 */
void mostrar_libros(const char *nombreArchivo);

// ------------------------------------------------------
// IMPLEMENTACIÓN DE FUNCIONES
// ------------------------------------------------------

void clasificacion_mezcla_natural(const char *NOMBRE_F, const char *NOMBRE_F1, const char *NOMBRE_F2) {
    int L; /**< Número de secuencias resultantes tras cada pasada */

    do {
        separar(NOMBRE_F, NOMBRE_F1, NOMBRE_F2);
        L = mezcla_natural(NOMBRE_F, NOMBRE_F1, NOMBRE_F2);
        printf("Secuencias mezcladas en esta pasada: %d\n", L);
    } while (L > 1);
}

void separar(const char *NOMBRE_F, const char *NOMBRE_F1, const char *NOMBRE_F2) {
    FILE *F = fopen(NOMBRE_F, "rb");
    FILE *F1 = fopen(NOMBRE_F1, "wb");
    FILE *F2 = fopen(NOMBRE_F2, "wb");
    Libro REG;

    if (!F || !F1 || !F2) {
        perror("Error al abrir archivos en separar()");
        exit(EXIT_FAILURE);
    }

    if (fread(&REG, sizeof(Libro), 1, F) != 1) {
        fclose(F); fclose(F1); fclose(F2);
        return;
    }

    do {
        copiar_secuencia(F, F1, &REG);
        if (!feof(F)) {
            copiar_secuencia(F, F2, &REG);
        }
    } while (!feof(F));

    fclose(F); fclose(F1); fclose(F2);
}

void copiar_secuencia(FILE *F_O, FILE *F_D, Libro *REG) {
    bool PARAR = false;
    do {
        fwrite(REG, sizeof(Libro), 1, F_D);
        leer_y_chequear(F_O, REG, &PARAR);
    } while (!PARAR);
}

void leer_y_chequear(FILE *FICH, Libro *REG, bool *PARAR) {
    Libro REG_SIG;
    if (fread(&REG_SIG, sizeof(Libro), 1, FICH) == 1) {
        if (REG->anioPublicacion > REG_SIG.anioPublicacion) {
            *PARAR = true;
        }
        *REG = REG_SIG;
    } else {
        *PARAR = true;
    }
}

int mezcla_natural(const char *NOMBRE_F, const char *NOMBRE_F1, const char *NOMBRE_F2) {
    FILE *F = fopen(NOMBRE_F, "wb");
    FILE *F1 = fopen(NOMBRE_F1, "rb");
    FILE *F2 = fopen(NOMBRE_F2, "rb");
    Libro R1, R2;
    bool FIN_ORDEN1 = false, FIN_ORDEN2 = false;
    int L = 0;

    if (!F || !F1 || !F2) {
        perror("Error al abrir archivos en mezcla_natural()");
        exit(EXIT_FAILURE);
    }

    size_t leido1 = fread(&R1, sizeof(Libro), 1, F1);
    size_t leido2 = fread(&R2, sizeof(Libro), 1, F2);

    while (leido1 == 1 || leido2 == 1) {
        L++;
        FIN_ORDEN1 = (leido1 != 1);
        FIN_ORDEN2 = (leido2 != 1);

        while (!FIN_ORDEN1 || !FIN_ORDEN2) {
            if (!FIN_ORDEN1 && (FIN_ORDEN2 || R1.anioPublicacion <= R2.anioPublicacion)) {
                fwrite(&R1, sizeof(Libro), 1, F);
                leer_y_chequear(F1, &R1, &FIN_ORDEN1);
                leido1 = (FIN_ORDEN1 && feof(F1)) ? 0 : 1;
                if (FIN_ORDEN1) { copiar_secuencia(F2, F, &R2); leido2 = 0; break; }
            } else if (!FIN_ORDEN2) {
                fwrite(&R2, sizeof(Libro), 1, F);
                leer_y_chequear(F2, &R2, &FIN_ORDEN2);
                leido2 = (FIN_ORDEN2 && feof(F2)) ? 0 : 1;
                if (FIN_ORDEN2) { copiar_secuencia(F1, F, &R1); leido1 = 0; break; }
            } else {
                break;
            }
        }

        if (leido1 == 0 && !feof(F1)) leido1 = fread(&R1, sizeof(Libro), 1, F1);
        if (leido2 == 0 && !feof(F2)) leido2 = fread(&R2, sizeof(Libro), 1, F2);
    }

    fclose(F); fclose(F1); fclose(F2);
    return L;
}

int main(void) {
    printf("=== ORDENACIÓN POR MEZCLA NATURAL (AÑO) ===\n");

    printf("\nArchivo original ('%s'):\n", NOMBRE_ARCHIVO);
    mostrar_libros(NOMBRE_ARCHIVO);

    clasificacion_mezcla_natural(NOMBRE_ARCHIVO, AUX1, AUX2);

    printf("\n=== ARCHIVO FINAL ORDENADO ===\n");
    mostrar_libros(NOMBRE_ARCHIVO);

    return 0;
}

void mostrar_libros(const char *nombreArchivo) {
    FILE *f = fopen(nombreArchivo, "rb");
    if (!f) { 
        printf("¡ERROR! Archivo '%s' no encontrado.\n", nombreArchivo);
        return; 
    }

    Libro libro;
    printf("----------------------------------------------------------------------------------------------------------------\n");
    printf("%-5s | %-5s | %-40s | %-25s | %-8s |\n", "POS", "AÑO", "TÍTULO", "AUTOR", "PRECIO");
    printf("----------------------------------------------------------------------------------------------------------------\n");

    int i = 0;
    while (fread(&libro, sizeof(Libro), 1, f) == 1) {
        printf("%-5d | %-5d | %-40s | %-25s | %-8.2f |\n",
               ++i, libro.anioPublicacion, libro.titulo, libro.autor, libro.precio);
    }

    printf("----------------------------------------------------------------------------------------------------------------\n");
    fclose(f);
}

