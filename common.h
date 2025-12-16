#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @file common.h
 * @brief Definiciones comunes y estructura base para la gestión de libros.
 *
 * Este archivo contiene constantes, tipos y funciones auxiliares compartidos
 * por los diferentes módulos del programa (búsqueda, ordenación, listado, etc.).
 */

// ---------------------------------------------------------------------------
// CONSTANTES GLOBALES
// ---------------------------------------------------------------------------

/** @brief Longitud máxima permitida para el título de un libro (incluyendo '\0'). */
#define CAD_TITULO 81

/** @brief Longitud máxima permitida para el nombre del autor (incluyendo '\0'). */
#define CAD_AUTOR 51

/** @brief Nombre del archivo binario principal donde se almacenan los libros. */
#define NOMBRE_ARCHIVO "libros_biblioteca.bin"

// ---------------------------------------------------------------------------
// ESTRUCTURA PRINCIPAL
// ---------------------------------------------------------------------------

/**
 * @struct Libro
 * @brief Representa un libro almacenado en la biblioteca.
 *
 * Cada registro se guarda en formato binario dentro de @ref NOMBRE_ARCHIVO.
 * Los campos de texto se almacenan como cadenas terminadas en '\0'.
 */
typedef struct {
    char titulo[CAD_TITULO];     /**< Título completo del libro. */
    char autor[CAD_AUTOR];       /**< Nombre del autor o autora. */
    int anioPublicacion;         /**< Año de publicación (ejemplo: 1999). */
    char prestado[3];            /**< Estado del préstamo: "Si" o "No". */
    double precio;               /**< Precio del libro en euros. */
} Libro;

// ---------------------------------------------------------------------------
// FUNCIONES AUXILIARES
// ---------------------------------------------------------------------------

/**
 * @brief Elimina el carácter de nueva línea '\n' de una cadena leída con fgets().
 *
 * @param s Cadena de caracteres a limpiar. Si es NULL, no realiza ninguna acción.
 *
 * @note Es una función inline para evitar sobrecarga en llamadas frecuentes.
 */
static inline void chomp(char *s) {
    if (!s) return;
    size_t n = strlen(s);
    if (n > 0 && s[n - 1] == '\n') s[n - 1] = '\0';
}

#endif // COMMON_H

