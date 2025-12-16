/**
 * @file gestor_estudiantes_seneca.c
 * @brief Sistema de gestión de estudiantes del programa nacional "Plan Séneca"
 * @author Kun Chen Lin
 * @date Diciembre 2025
 *
 * Este programa gestiona información de estudiantes participantes en el programa
 * "Plan Séneca". Lee datos desde un archivo de texto con formato CSV (separado por
 * punto y coma) y ofrece un menú interactivo con las siguientes opciones:
 *   1. Mostrar todos los estudiantes (formato tabular)
 *   2. Buscar estudiante por nombre (búsqueda lineal)
 *   3. Listar estudiantes becados o no becados
 *   4. Ordenar estudiantes por nota media (algoritmo de selección directa)
 *   5. Salir del programa
 *
 * Formato del archivo de entrada (estudiantes_seneca.txt):
 *     nombre;universidad;edad;becado;nota_media
 *
 * Ejemplo:
 *     Juan García Pérez;Universidad de Sevilla;22;Si;85.5
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------------------------------------------------------------------------
// CONSTANTES
// ---------------------------------------------------------------------------

/** @brief Longitud máxima para el nombre completo del estudiante */
#define MAX_NOMBRE 100

/** @brief Longitud máxima para el nombre de la universidad */
#define MAX_UNIVERSIDAD 100

/** @brief Capacidad máxima de estudiantes que puede gestionar el programa */
#define MAX_ESTUDIANTES 100

/** @brief Nombre del archivo de datos de entrada */
#define ARCHIVO_DATOS "estudiantes_seneca.txt"

// ---------------------------------------------------------------------------
// ESTRUCTURAS
// ---------------------------------------------------------------------------

/**
 * @struct Estudiante
 * @brief Representa la información de un estudiante del Plan Séneca
 */
typedef struct {
    char nombre[MAX_NOMBRE];              /**< Nombre completo del estudiante */
    char universidad[MAX_UNIVERSIDAD];    /**< Universidad de procedencia */
    int edad;                             /**< Edad del estudiante */
    char becado[3];                       /**< Estado de beca: "Si" o "No" */
    double nota_media;                    /**< Nota media sobre 100 */
} Estudiante;

// ---------------------------------------------------------------------------
// PROTOTIPOS DE FUNCIONES
// ---------------------------------------------------------------------------

int cargar_estudiantes(const char *nombre_archivo, Estudiante estudiantes[], int max);
void mostrar_estudiantes(const Estudiante estudiantes[], int num_estudiantes);
void buscar_por_nombre(const Estudiante estudiantes[], int num_estudiantes);
void listar_por_beca(const Estudiante estudiantes[], int num_estudiantes);
void ordenar_por_nota(Estudiante estudiantes[], int num_estudiantes);
void imprimir_separador(void);
void imprimir_cabecera_tabla(void);
void imprimir_estudiante(const Estudiante *est, int posicion);
void limpiar_buffer(void);

// ---------------------------------------------------------------------------
// IMPLEMENTACIÓN DE FUNCIONES
// ---------------------------------------------------------------------------

/**
 * @brief Carga los estudiantes desde el archivo de texto
 * @param nombre_archivo Ruta del archivo de entrada
 * @param estudiantes Array donde se almacenarán los estudiantes
 * @param max Capacidad máxima del array
 * @return Número de estudiantes cargados, o -1 si hay error
 */
int cargar_estudiantes(const char *nombre_archivo, Estudiante estudiantes[], int max) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return -1;
    }
    
    int contador = 0;
    
    while (contador < max && !feof(archivo)) {
        Estudiante est;
        memset(&est, 0, sizeof(Estudiante));
        
        // Leer los campos separados por punto y coma
        int campos_leidos = fscanf(archivo, "%99[^;];%99[^;];%d;%2[^;];%lf\n",
                                    est.nombre,
                                    est.universidad,
                                    &est.edad,
                                    est.becado,
                                    &est.nota_media);
        
        if (campos_leidos == 5) {
            estudiantes[contador] = est;
            contador++;
        } else if (campos_leidos != EOF) {
            // Línea mal formada, consumir hasta el final de línea
            int c;
            while ((c = fgetc(archivo)) != '\n' && c != EOF) {}
        }
    }
    
    fclose(archivo);
    return contador;
}

/**
 * @brief Imprime una línea de separación horizontal
 */
void imprimir_separador(void) {
    printf("+");
    for (int i = 0; i < 5; i++) printf("-");
    printf("+");
    for (int i = 0; i < 32; i++) printf("-");
    printf("+");
    for (int i = 0; i < 30; i++) printf("-");
    printf("+");
    for (int i = 0; i < 6; i++) printf("-");
    printf("+");
    for (int i = 0; i < 8; i++) printf("-");
    printf("+");
    for (int i = 0; i < 12; i++) printf("-");
    printf("+\n");
}

/**
 * @brief Imprime la cabecera de la tabla de estudiantes
 */
void imprimir_cabecera_tabla(void) {
    imprimir_separador();
    printf("| %-3s | %-30s | %-28s | %-4s | %-6s | %-10s |\n",
           "POS", "NOMBRE", "UNIVERSIDAD", "EDAD", "BECADO", "NOTA MEDIA");
    imprimir_separador();
}

/**
 * @brief Imprime los datos de un estudiante en formato tabular
 * @param est Puntero al estudiante a mostrar
 * @param posicion Posición del estudiante en la lista
 */
void imprimir_estudiante(const Estudiante *est, int posicion) {
    printf("| %-3d | %-30s | %-28s | %-4d | %-6s | %10.2f |\n",
           posicion,
           est->nombre,
           est->universidad,
           est->edad,
           est->becado,
           est->nota_media);
}

/**
 * @brief Muestra todos los estudiantes en formato de tabla
 * @param estudiantes Array de estudiantes
 * @param num_estudiantes Número de estudiantes en el array
 */
void mostrar_estudiantes(const Estudiante estudiantes[], int num_estudiantes) {
    printf("\n");
    printf("================================================================================\n");
    printf("                    LISTADO DE ESTUDIANTES - PLAN SÉNECA\n");
    printf("================================================================================\n");
    printf("Total de estudiantes: %d\n\n", num_estudiantes);
    
    if (num_estudiantes == 0) {
        printf("No hay estudiantes registrados.\n");
        return;
    }
    
    imprimir_cabecera_tabla();
    
    for (int i = 0; i < num_estudiantes; i++) {
        imprimir_estudiante(&estudiantes[i], i + 1);
    }
    
    imprimir_separador();
    printf("\n");
}

/**
 * @brief Busca un estudiante por nombre usando búsqueda lineal
 * @param estudiantes Array de estudiantes
 * @param num_estudiantes Número de estudiantes en el array
 */
void buscar_por_nombre(const Estudiante estudiantes[], int num_estudiantes) {
    char nombre_buscado[MAX_NOMBRE];
    
    printf("\n");
    printf("================================================================================\n");
    printf("                    BÚSQUEDA DE ESTUDIANTE POR NOMBRE\n");
    printf("================================================================================\n");
    printf("Introduzca el nombre del estudiante a buscar: ");
    
    limpiar_buffer();
    if (fgets(nombre_buscado, sizeof(nombre_buscado), stdin) == NULL) {
        printf("Error de lectura.\n");
        return;
    }
    
    // Eliminar el salto de línea
    nombre_buscado[strcspn(nombre_buscado, "\n")] = 0;
    
    printf("\nBuscando estudiante: \"%s\"...\n\n", nombre_buscado);
    
    // Búsqueda lineal
    int encontrado = 0;
    for (int i = 0; i < num_estudiantes; i++) {
        if (strcasecmp(estudiantes[i].nombre, nombre_buscado) == 0) {
            printf("¡ESTUDIANTE ENCONTRADO!\n\n");
            imprimir_cabecera_tabla();
            imprimir_estudiante(&estudiantes[i], i + 1);
            imprimir_separador();
            encontrado = 1;
            break;
        }
    }
    
    if (!encontrado) {
        printf("No se encontró ningún estudiante con el nombre \"%s\".\n", nombre_buscado);
    }
    
    printf("\n");
}

/**
 * @brief Lista estudiantes según su estado de beca
 * @param estudiantes Array de estudiantes
 * @param num_estudiantes Número de estudiantes en el array
 */
void listar_por_beca(const Estudiante estudiantes[], int num_estudiantes) {
    char opcion[5];
    
    printf("\n");
    printf("================================================================================\n");
    printf("                    LISTADO POR ESTADO DE BECA\n");
    printf("================================================================================\n");
    printf("¿Qué estudiantes desea listar?\n");
    printf("  [Si] Estudiantes Becados\n");
    printf("  [No] Estudiantes No Becados\n");
    printf("  Respuesta (Si/No): ");
    
    if (fgets(opcion, sizeof(opcion), stdin) == NULL) {
        printf("Error de lectura.\n");
        return;
    }
    
    // Eliminar el salto de línea
    opcion[strcspn(opcion, "\n")] = 0;
    
    // Validar entrada
    if (strcasecmp(opcion, "si") != 0 && strcasecmp(opcion, "no") != 0) {
        printf("\nERROR: Respuesta no válida. Debe introducir 'Si' o 'No'.\n\n");
        return;
    }
    
    printf("\nListando estudiantes con estado de beca: \"%s\"\n\n", opcion);
    
    int contador = 0;
    imprimir_cabecera_tabla();
    
    for (int i = 0; i < num_estudiantes; i++) {
        if (strcasecmp(estudiantes[i].becado, opcion) == 0) {
            imprimir_estudiante(&estudiantes[i], i + 1);
            contador++;
        }
    }
    
    imprimir_separador();
    printf("\nTotal de estudiantes mostrados: %d\n\n", contador);
}

/**
 * @brief Ordena los estudiantes por nota media usando el algoritmo de selección directa
 * @param estudiantes Array de estudiantes a ordenar
 * @param num_estudiantes Número de estudiantes en el array
 */
void ordenar_por_nota(Estudiante estudiantes[], int num_estudiantes) {
    printf("\n");
    printf("================================================================================\n");
    printf("                    ORDENAR POR NOTA MEDIA (SELECCIÓN DIRECTA)\n");
    printf("================================================================================\n");
    
    if (num_estudiantes == 0) {
        printf("No hay estudiantes para ordenar.\n\n");
        return;
    }
    
    printf("Ordenando estudiantes por nota media (de mayor a menor)...\n\n");
    
    // Algoritmo de ordenación por selección directa (descendente)
    for (int i = 0; i < num_estudiantes - 1; i++) {
        int indice_maximo = i;
        
        // Buscar el elemento con mayor nota en el resto del array
        for (int j = i + 1; j < num_estudiantes; j++) {
            if (estudiantes[j].nota_media > estudiantes[indice_maximo].nota_media) {
                indice_maximo = j;
            }
        }
        
        // Intercambiar si se encontró un elemento mayor
        if (indice_maximo != i) {
            Estudiante temp = estudiantes[i];
            estudiantes[i] = estudiantes[indice_maximo];
            estudiantes[indice_maximo] = temp;
        }
    }
    
    printf("¡Ordenación completada!\n\n");
    
    // Mostrar resultado
    imprimir_cabecera_tabla();
    for (int i = 0; i < num_estudiantes; i++) {
        imprimir_estudiante(&estudiantes[i], i + 1);
    }
    imprimir_separador();
    printf("\n");
}

/**
 * @brief Limpia el buffer de entrada estándar
 */
void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/**
 * @brief Muestra el menú principal del programa
 */
void mostrar_menu(void) {
    printf("================================================================================\n");
    printf("              GESTIÓN DE ESTUDIANTES - PROGRAMA PLAN SÉNECA\n");
    printf("================================================================================\n");
    printf("  1. Mostrar todos los estudiantes\n");
    printf("  2. Buscar estudiante por nombre (búsqueda lineal)\n");
    printf("  3. Listar estudiantes becados o no becados\n");
    printf("  4. Ordenar por nota media (selección directa)\n");
    printf("  5. Salir\n");
    printf("================================================================================\n");
    printf("Seleccione una opción: ");
}

/**
 * @brief Función principal del programa
 * @return 0 si la ejecución fue exitosa, 1 en caso de error
 */
int main(void) {
    Estudiante estudiantes[MAX_ESTUDIANTES];
    int num_estudiantes = 0;
    int opcion;
    
    // Carga inicial de datos
    printf("\n");
    printf("================================================================================\n");
    printf("              SISTEMA DE GESTIÓN - PROGRAMA PLAN SÉNECA\n");
    printf("================================================================================\n");
    printf("Cargando datos desde el archivo '%s'...\n", ARCHIVO_DATOS);
    
    num_estudiantes = cargar_estudiantes(ARCHIVO_DATOS, estudiantes, MAX_ESTUDIANTES);
    
    if (num_estudiantes < 0) {
        printf("ERROR: No se pudo cargar el archivo de datos.\n");
        printf("Asegúrese de que el archivo '%s' existe y tiene el formato correcto.\n", ARCHIVO_DATOS);
        return 1;
    }
    
    printf("¡Carga completada! Se cargaron %d estudiantes.\n", num_estudiantes);
    printf("\n");
    
    // Bucle principal del menú
    while (1) {
        mostrar_menu();
        
        if (scanf("%d", &opcion) != 1) {
            printf("\nERROR: Entrada no válida. Por favor, introduzca un número.\n\n");
            limpiar_buffer();
            continue;
        }
        
        limpiar_buffer(); // Limpiar buffer después de scanf
        
        switch (opcion) {
            case 1:
                mostrar_estudiantes(estudiantes, num_estudiantes);
                break;
            
            case 2:
                buscar_por_nombre(estudiantes, num_estudiantes);
                break;
            
            case 3:
                listar_por_beca(estudiantes, num_estudiantes);
                break;
            
            case 4:
                ordenar_por_nota(estudiantes, num_estudiantes);
                break;
            
            case 5:
                printf("\n");
                printf("================================================================================\n");
                printf("            Gracias por usar el Sistema de Gestión - Plan Séneca\n");
                printf("================================================================================\n");
                printf("\n");
                return 0;
            
            default:
                printf("\nERROR: Opción no válida. Por favor, seleccione una opción del 1 al 5.\n\n");
                break;
        }
    }
    
    return 0;
}
