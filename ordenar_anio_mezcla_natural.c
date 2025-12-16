/**
 * @file mezcla_anio_natural_libros.c
 * @brief Ordenación de libros por año usando el método de mezcla natural.
 *
 * Este programa implementa la ordenación de registros de tipo `Libro` almacenados
 * en un archivo binario. Utiliza el método de clasificación por mezcla natural,
 * que divide el archivo original en secuencias ordenadas y luego las mezcla 
 * utilizando dos archivos auxiliares hasta obtener un archivo totalmente ordenado.
 *
 * Funcionalidades principales:
 *  - Separación del archivo original en dos archivos auxiliares con secuencias ordenadas.
 *  - Mezcla de las secuencias de los auxiliares en el archivo principal.
 *  - Visualización de los registros ordenados por pantalla.
 *  - Posibilidad de guardar el archivo ordenado en un nuevo archivo binario.
 *
 * Archivos auxiliares:
 *  - aux1.bin: primer archivo temporal para las secuencias.
 *  - aux2.bin: segundo archivo temporal para las secuencias.
 *  - libros_biblioteca_anio.bin: archivo final ordenado.
 *
 * Autor: Kun Chen Lin
 * Fecha: 17/11/2025
 * Versión: 1.0
 *
 * Dependencias:
 *  - common.h: define la estructura `Libro` y funciones auxiliares para manipulación de registros.
 *
 * Ejemplo de uso:
 *  ./mezcla_natural_libros
 *  Preguntará si desea guardar el archivo ordenado y mostrará el contenido ordenado.
 */



#include "common.h" ///< Incluye el archivo de cabecera con la definición de la estructura Libro y funciones auxiliares

// --- Prototipos de módulos ---

/**
 * @brief Clasificación principal mediante el método de mezcla natural.
 * @param nombreF Nombre del archivo principal.
 * @param nombreF1 Nombre del primer archivo auxiliar.
 * @param nombreF2 Nombre del segundo archivo auxiliar.
 */
void clasificacion_mezcla_natural(const char *nombreF, const char *nombreF1, const char *nombreF2);

/**
 * @brief Separa el archivo original en dos archivos auxiliares con secuencias ordenadas.
 * @param nombreF Nombre del archivo principal.
 * @param nombreF1 Nombre del primer archivo auxiliar.
 * @param nombreF2 Nombre del segundo archivo auxiliar.
 */
void separar(const char *nombreF, const char *nombreF1, const char *nombreF2);

/**
 * @brief Copia una secuencia ordenada desde un archivo origen a uno destino.
 * @param f_origen Puntero al archivo origen.
 * @param f_destino Puntero al archivo destino.
 * @param reg Registro actual que se está copiando.
 */
void copiar_secuencia(FILE *f_origen, FILE *f_destino, Libro *reg);

/**
 * @brief Lee el siguiente registro y verifica si se mantiene el orden por año.
 * @param f Archivo del cual se lee.
 * @param reg Registro actual a comparar.
 * @param parar Indicador de fin de secuencia (1 si termina la secuencia).
 */
void leer_y_chequear(FILE *f, Libro *reg, int *parar);

/**
 * @brief Mezcla las secuencias ordenadas de los dos archivos auxiliares en el archivo principal.
 * @param nombreF Nombre del archivo principal.
 * @param nombreF1 Nombre del primer archivo auxiliar.
 * @param nombreF2 Nombre del segundo archivo auxiliar.
 * @param L Puntero a un entero donde se guarda el número de secuencias resultantes.
 */
void mezcla_natural(const char *nombreF, const char *nombreF1, const char *nombreF2, int *L);

// --- Implementación ---

/**
 * @brief Función principal del programa. 
 * Ordena los registros del archivo binario de libros por año de publicación 
 * usando el método de mezcla natural.
 * 
 * @return 0 si la ejecución fue correcta, 1 si ocurrió algún error.
 */
int main(void) {
    const char *nombreF  = "libros_biblioteca.bin";   ///< Archivo principal
    const char *nombreF1 = "aux1.bin";                ///< Archivo auxiliar 1
    const char *nombreF2 = "aux2.bin";                ///< Archivo auxiliar 2

    /// Clasificación por mezcla natural
    clasificacion_mezcla_natural(nombreF, nombreF1, nombreF2);

    /// Mostrar resultado final por pantalla
    FILE *f = fopen(nombreF, "rb"); ///< Abre el archivo binario ya ordenado
    if (!f) { perror("fopen resultado"); return 1; } ///< Verifica apertura correcta

    /// Cabecera de la tabla de salida
    printf("\n%-30s %-20s %-6s %-9s %-8s\n",
           "Título", "Autor", "Año", "Prestado", "Precio");
    printf("-------------------------------------------------------------------------------\n");

    Libro L; ///< Registro auxiliar para lectura
    while (fread(&L, sizeof(Libro), 1, f) == 1) { ///< Lee mientras haya registros
        printf("%-30s %-20s %-6d %-9s %8.2f\n",
               L.titulo, L.autor, L.anioPublicacion, L.prestado, L.precio);
    }
    fclose(f);

    /// Pregunta al usuario si desea guardar el resultado ordenado en otro archivo
    char opcion;
    printf("\n¿Guardar resultado en 'libros_biblioteca_anio.bin'? (s/n): ");
    scanf(" %c", &opcion);
    if (opcion == 's' || opcion == 'S') {
        FILE *orig = fopen(nombreF, "rb");
        FILE *dest = fopen("libros_biblioteca_anio.bin", "wb");
        if (!orig || !dest) { perror("fopen guardar"); return 1; }

        Libro reg;
        while (fread(&reg, sizeof(Libro), 1, orig) == 1) ///< Copia todos los registros
            fwrite(&reg, sizeof(Libro), 1, dest);

        fclose(orig);
        fclose(dest);
        printf(" Guardado en 'libros_biblioteca_anio.bin'\n");
    }

    return 0;
}

/**
 * @brief Controla el proceso completo de separación y mezcla hasta que los datos queden ordenados.
 * @param nombreF Nombre del archivo principal.
 * @param nombreF1 Nombre del primer archivo auxiliar.
 * @param nombreF2 Nombre del segundo archivo auxiliar.
 */
void clasificacion_mezcla_natural(const char *nombreF, const char *nombreF1, const char *nombreF2) {
    int L = 0;
    do {
        separar(nombreF, nombreF1, nombreF2);          ///< Separa en dos archivos
        mezcla_natural(nombreF, nombreF1, nombreF2, &L); ///< Mezcla los archivos ordenadamente
    } while (L > 1); ///< Repite mientras haya más de una secuencia
}

/**
 * @brief Divide el archivo original en dos archivos auxiliares con secuencias ordenadas.
 * @param nombreF Nombre del archivo principal.
 * @param nombreF1 Nombre del primer archivo auxiliar.
 * @param nombreF2 Nombre del segundo archivo auxiliar.
 */
void separar(const char *nombreF, const char *nombreF1, const char *nombreF2) {
    FILE *f  = fopen(nombreF, "rb");
    FILE *f1 = fopen(nombreF1, "wb");
    FILE *f2 = fopen(nombreF2, "wb");
    if (!f || !f1 || !f2) { perror("fopen separar"); exit(1); }

    Libro reg;
    if (fread(&reg, sizeof(Libro), 1, f) != 1) { ///< Si no hay registros, sale
        fclose(f); fclose(f1); fclose(f2);
        return;
    }

    int alterna = 0; ///< Variable para alternar entre f1 y f2
    while (!feof(f)) {
        if (alterna == 0)
            copiar_secuencia(f, f1, &reg); ///< Copia una secuencia al aux1
        else
            copiar_secuencia(f, f2, &reg); ///< Copia una secuencia al aux2
        alterna = 1 - alterna; ///< Cambia de archivo
    }

    fclose(f);
    fclose(f1);
    fclose(f2);
}

/**
 * @brief Copia una secuencia ordenada desde un archivo origen a uno destino.
 * @param f_origen Archivo origen.
 * @param f_destino Archivo destino.
 * @param reg Registro actual que se está copiando.
 */
void copiar_secuencia(FILE *f_origen, FILE *f_destino, Libro *reg) {
    int parar = 0; ///< Indicador de fin de secuencia
    do {
        fwrite(reg, sizeof(Libro), 1, f_destino); ///< Escribe registro en el destino
        leer_y_chequear(f_origen, reg, &parar);   ///< Lee el siguiente y comprueba orden
    } while (!parar);
}

/**
 * @brief Lee el siguiente registro y determina si se mantiene el orden ascendente por año.
 * @param f Archivo de lectura.
 * @param reg Registro actual.
 * @param parar Bandera que indica el fin de una secuencia (1 si termina).
 */
void leer_y_chequear(FILE *f, Libro *reg, int *parar) {
    Libro reg_sig;
    if (fread(&reg_sig, sizeof(Libro), 1, f) == 1) {
        if (reg->anioPublicacion > reg_sig.anioPublicacion)
            *parar = 1; ///< Detecta ruptura de orden
        *reg = reg_sig; ///< Actualiza registro actual
    } else {
        *parar = 1; ///< Fin del archivo
    }
}

/**
 * @brief Mezcla las secuencias ordenadas de los dos archivos auxiliares en el principal.
 * @param nombreF Nombre del archivo principal.
 * @param nombreF1 Nombre del primer archivo auxiliar.
 * @param nombreF2 Nombre del segundo archivo auxiliar.
 * @param L Puntero a un entero donde se guarda el número de secuencias resultantes.
 */
void mezcla_natural(const char *nombreF, const char *nombreF1, const char *nombreF2, int *L) {
    FILE *f1 = fopen(nombreF1, "rb");
    FILE *f2 = fopen(nombreF2, "rb");
    FILE *f  = fopen(nombreF, "wb");
    if (!f || !f1 || !f2) { perror("fopen mezcla"); exit(1); }

    *L = 0;
    Libro R1, R2;
    int fin1 = fread(&R1, sizeof(Libro), 1, f1) != 1;
    int fin2 = fread(&R2, sizeof(Libro), 1, f2) != 1;

    while (!fin1 && !fin2) {
        int fin_orden = 0;
        while (!fin_orden) {
            if (R1.anioPublicacion <= R2.anioPublicacion) {
                fwrite(&R1, sizeof(Libro), 1, f);
                leer_y_chequear(f1, &R1, &fin_orden);
                if (fin_orden) copiar_secuencia(f2, f, &R2);
            } else {
                fwrite(&R2, sizeof(Libro), 1, f);
                leer_y_chequear(f2, &R2, &fin_orden);
                if (fin_orden) copiar_secuencia(f1, f, &R1);
            }
        }
        (*L)++;
        fin1 = feof(f1);
        fin2 = feof(f2);
    }

    /// Copiar colas restantes
    while (!feof(f1)) {
        copiar_secuencia(f1, f, &R1);
        (*L)++;
    }
    while (!feof(f2)) {
        copiar_secuencia(f2, f, &R2);
        (*L)++;
    }

    fclose(f);
    fclose(f1);
    fclose(f2);
}
