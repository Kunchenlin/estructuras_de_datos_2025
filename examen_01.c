/**
 * @file examen_ejercicio_01.c
 * @brief Programa para la gestión de trenes 
 */

// --- Librerías estándar necesarias para el programa ---
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Constantes ---
/** @brief Capacidad inicial del array de salidas del Min-Heap. */
#define MAX_NODOS 100 
/** @brief Longitud máxima para el ID del tren. */
#define MAX_CODE_LEN 10
/** @brief Longitud máxima para cadenas de texto (origen, destino, aerolínea). */
#define MAX_STR_LEN 50
/** @brief Factor de crecimiento para la redimensión dinámica del Heap. */
#define REDIMENSION_FACTOR 2

// --- Definiciones de estructuras de datos --- 

typedef struct tren {
    char id_tren[MAX_CODE_LEN]; /// Código único del vuelo (clave ABB).
    char compania[MAX_STR_LEN];   /// Nombre de la compañía ferroviaria.
    char origen[MAX_STR_LEN];        /// Ciudad de origen.
    char destino[MAX_STR_LEN];       /// Ciudad de destino.
    char carga_tipo[MAX_STR_LEN];  /// Tipo de carga (pasajeros, mercancías, mixto).
    int distancia;              /// Distancia en kilómetros.
    int fecha_operacion;                /// Fecha programada de salida (AAAAMMDD).
    int hora_operacion;                 /// Hora programada de salida (HHMM).
    struct tren* izquierdo;         /// Puntero al subárbol izquierdo (menor clave).
    struct tren* derecho;           /// Puntero al subárbol derecho (mayor clave).
} Tren;

/**
 * @struct salida
 * @brief Representa una salida programada y es un elemento dentro del Min-Heap.
 */
typedef struct salida {
    Tren* tren;            /// Puntero al nodo Vuelo asociado.
    /** @brief Clave de prioridad para el Heap, generada como AAAAMMDDHHMM. */
    int clave_salida;        
} Salida;

/**
 * @struct heap
 * @brief Estructura que implementa un Min-Heap (Montículo de Mínimos) dinámico.
 */
typedef struct heap {
    Salida* elementos;       /// Array dinámico de elementos Salida (almacén del montículo).
    int capacidad;           /// Capacidad máxima actual del array.
    int tamano;              /// Número actual de elementos en el Heap.
} Heap;

// --- Prototipos de funciones ---
// Funciones del ABB
Tren* insertar_tren(Tren* raiz, Tren* nuevo_tren);
Tren* buscar_tren(Tren* raiz, const char* destino);
/** @brief Elimina un tren, con validación de si está en el Heap. */
Tren* eliminar_tren(Tren* raiz, const char* id_tren, Heap* heap_salidas);
Tren* encontrar_min(Tren* raiz);
void recorrer_inorden(Tren* raiz);
void liberar_arbol(Tren* raiz);
/** @brief Comprueba si el tren está pendiente de salida en el Heap. */
int tren_en_heap(Heap* heap, const char* id_tren);
void listar_tren_por_carga_o_distancia_minima(Tren* raiz, const char* filtro, int tipo_filtro);

// Funciones del Min-Heap
void insertar_heap(Heap* heap, Salida nueva_salida);
Salida extraer_min_heap(Heap* heap);
void min_heapify(Heap* heap, int i);
void heapsort_y_mostrar(Heap* heap);

/**
 * @brief Función principal que implementa el menú interactivo del sistema de gestión aeroportuaria.
 * @return 0 si la ejecución es exitosa, 1 en caso de error de asignación de memoria inicial.
 */
int main() {
    Tren* arbol_tren = NULL;  /// Puntero a la raíz del Árbol de Vuelos.

    /// Inicialización del Heap dinámico.
    Heap monticulo_salidas = {.elementos = (Salida*)malloc(sizeof(Salida) * MAX_NODOS), .capacidad = MAX_NODOS, .tamano = 0};
    if (monticulo_salidas.elementos == NULL) {
        printf("Error fatal: No se pudo asignar memoria para el montículo.\n");
        return 1;
    }
    
    int opcion;
    do {
        // --- Menú de opciones ---
        printf("\n============================================\n");
        printf(" Sistema de Gestión De Trenes (ABB & Heap)\n");
        printf("============================================\n");
        printf("1. Registrar nuevo tren\n");
        printf("2. Buscar tren por ID\n");
        printf("3. Eliminar tren\n");
        printf("4. Listar todos los trenes (código: inorden)\n");
        printf("5. Listar trenes por carga o distancia minima\n");
        printf("6. Salir\n");
        printf("Elige una opción: ");
        
        /// Manejo de entrada para robustez.
        if (scanf("%d", &opcion) != 1) { 
            while(getchar() != '\n');
            opcion = 0; 
        }

        // Declaraciones de variables de control del menú
        char id_tren[MAX_CODE_LEN];
        char destino[MAX_STR_LEN];
        char compania[MAX_STR_LEN];
        int fecha_salida, hora_salida;
        Tren* tren;
        Salida nueva_salida;
        int num_vuelos_a_generar;
        
        switch(opcion) {
            case 1: // Registrar nuevo vuelo
                tren = (Tren*)malloc(sizeof(Tren));
                if (tren == NULL) { printf("Error: Fallo de asignación de memoria.\n"); break; }
                printf("Ingrese ID del tren: "); scanf("%s", tren->id_tren);
                printf("Ingrese ciudad de origen: "); scanf("%s", tren->origen);
                printf("Ingrese ciudad de destino: "); scanf("%s", tren->destino);
                printf("Ingrese compañia: "); scanf("%s", tren->compania);
                printf("Ingrese distancia (km): "); scanf("%d", &tren->distancia);
                printf("Ingrese fecha de operación (AAAAMMDD): "); scanf("%d", &tren->fecha_operacion);
                printf("Ingrese hora de operación (HHMM): "); scanf("%d", &tren->hora_operacion);
                printf("Ingrese tipo de carga (pasajeros, mercancías, mixto): "); scanf("%s", tren->carga_tipo);
                tren->izquierdo = tren->derecho = NULL; 
                arbol_tren = insertar_tren(arbol_tren, tren);
                break;

            case 2: // Buscar tren por destino
                printf("Ingrese destino: "); scanf("%s", destino);
                tren = buscar_tren(arbol_tren, destino);
                if (tren) {
                    printf("Tren encontrado:\n");
                    printf("ID: %s, Origen: %s, Destino: %s, Compañia: %s, Distancia: %d km, Fecha: %d, Hora: %d, Tipo de carga: %s\n",
                           tren->id_tren, tren->origen, tren->destino, tren->compania,
                           tren->distancia, tren->fecha_operacion, tren->hora_operacion,
                           tren->carga_tipo);
                } else {
                    printf("Vuelo no encontrado.\n");
                }
                break;

            case 3: // Eliminar vuelo
                printf("Ingrese código del tren a eliminar: "); scanf("%s", id_tren);
                arbol_tren = eliminar_tren(arbol_tren, id_tren, &monticulo_salidas);
                break;

            case 4: // Listar Inorden
                printf("--- Listado de Trenes (Inorden) ---\n");
                recorrer_inorden(arbol_tren);
                break;

            case 5: // Listar por filtro
                printf("Filtrar por:\n1. Tipo de carga\n2. Distancia mínima\nElige una opción: ");
                int tipo_filtro;   
                char filtro[MAX_STR_LEN]; 
                scanf("%d", &tipo_filtro);
                if (tipo_filtro == 1) {
                    printf("Ingrese tipo de carga (pasajeros, mercancías, mixto): ");
                    printf("--- Trenes con tipo de carga especificado ---\n");
                    scanf("%s", filtro);
                    listar_tren_por_carga_o_distancia_minima(arbol_tren, filtro, tipo_filtro);
                } else if (tipo_filtro == 2) {
                    printf("Ingrese distancia mínima (km): ");
                    scanf("%s", filtro);
                    printf(" --- Trenes con distancia mínima especificada ---\n");
                    listar_tren_por_carga_o_distancia_minima(arbol_tren, filtro, tipo_filtro);
                } else {
                    printf("Opción inválida.\n");
                    break;
                }

            case 6: // Salir
                liberar_arbol(arbol_tren); 
                free(monticulo_salidas.elementos); 
                printf("Saliendo del programa y liberando memoria...\n");
                break;

            default:
                printf("Opción inválida, por favor ingrese una opción válida.\n");
                while(getchar() != '\n'); 
        }
    } while (opcion != 6);

    return 0;
} 



/**
 * @brief Inserta un nuevo tren en el ABB, manteniendo la propiedad de orden.
 * @param raiz La raíz del ABB.
 * @param nuevo_tren El nodo Vuelo a insertar.
 * @return La nueva raíz del ABB.
 */
Tren* insertar_tren(Tren* raiz, Tren* nuevo_tren) {
    if (raiz == NULL) {
        return nuevo_tren;
    }
    int cmp = strcmp(nuevo_tren->destino, raiz->destino);
    
    if (cmp < 0) {
        /// Recursivamente a la izquierda.
        raiz->izquierdo = insertar_tren(raiz->izquierdo, nuevo_tren);
    } else if (cmp > 0) {
        /// Recursivamente a la derecha.
        raiz->derecho = insertar_tren(raiz->derecho, nuevo_tren);
    } else {
        /// Clave duplicada: no se inserta.
        printf("Error: El tren ya existe (%s). No insertado.\n", nuevo_tren->destino);
        free(nuevo_tren); 
    }
    return raiz;
}


/**
 * @brief Busca un tren en el ABB por su destino de forma eficiente.
 * @param raiz La raíz del ABB.
 * @param destino El código del vuelo a buscar.
 * @return Puntero al nodo Vuelo si se encuentra, NULL en caso contrario.
 */
Tren* buscar_tren(Tren* raiz, const char* id_tren) {
    if (raiz == NULL) return NULL;
    int cmp = strcmp(id_tren, raiz->id_tren);
    
    if (cmp == 0) return raiz; // Encontrado
    
    if (cmp < 0) {
        return buscar_tren(raiz->izquierdo, id_tren); // Buscar a la izquierda
    }
    return buscar_tren(raiz->derecho, id_tren); // Buscar a la derecha
}

/**
 * @brief Libera recursivamente toda la memoria asignada al ABB (Postorden).
 * @param raiz La raíz del ABB.
 */
void liberar_arbol(Tren* raiz) {
    if (raiz == NULL) return;
    liberar_arbol(raiz->izquierdo);
    liberar_arbol(raiz->derecho);
    free(raiz);
}

Tren* eliminar_tren(Tren* raiz, const char* id_tren, Heap* heap_salidas) {
    if (raiz == NULL) {
        printf("Error: Tren no encontrado (%s).\n", id_tren);
        return NULL;
    }
    
    int cmp = strcmp(id_tren, raiz->id_tren);
    if (cmp < 0) {
        raiz->izquierdo = eliminar_tren(raiz->izquierdo, id_tren, heap_salidas);
    } else if (cmp > 0) {
        raiz->derecho = eliminar_tren(raiz->derecho, id_tren, heap_salidas);
    } else {
        // Validar si el tren está en el Heap
        if (tren_en_heap(heap_salidas, id_tren)) {
            printf("Error: No se puede eliminar el tren %s porque está pendiente de salida.\n", id_tren);
            return raiz;
        }
        
        // Nodo con un solo hijo o sin hijos
        if (raiz->izquierdo == NULL) {
            Tren* temp = raiz->derecho;
            free(raiz);
            return temp;
        } else if (raiz->derecho == NULL) {
            Tren* temp = raiz->izquierdo;
            free(raiz);
            return temp;
        }
        
        // Nodo con dos hijos: obtener el sucesor inorder
        Tren* temp = encontrar_min(raiz->derecho);
        strcpy(raiz->id_tren, temp->id_tren);
        strcpy(raiz->compania, temp->compania);
        strcpy(raiz->origen, temp->origen);
        strcpy(raiz->destino, temp->destino);
        strcpy(raiz->carga_tipo, temp->carga_tipo);
        raiz->distancia = temp->distancia;
        raiz->fecha_operacion = temp->fecha_operacion;
        raiz->hora_operacion = temp->hora_operacion;
        
        raiz->derecho = eliminar_tren(raiz->derecho, temp->id_tren, heap_salidas);
    }
    return raiz;
}
/**
 * @brief Recorre el ABB en orden (Inorden) e imprime la información de los vuelos.
 * @param raiz La raíz del ABB.
 */
void recorrer_inorden(Tren* raiz) {
    if (raiz == NULL) return;
    recorrer_inorden(raiz->izquierdo);
    printf("ID: %s, Origen: %s, Destino: %s, Compañia: %s, Distancia: %d km, Fecha: %d, Hora: %d, Tipo de carga: %s\n",
           raiz->id_tren, raiz->origen, raiz->destino, raiz->compania,
           raiz->distancia, raiz->fecha_operacion, raiz->hora_operacion,
           raiz->carga_tipo);
    recorrer_inorden(raiz->derecho);
}

void listar_tren_por_carga_o_distancia_minima(Tren* raiz, const char* filtro, int tipo_filtro) {
    if (raiz == NULL) return;

    listar_tren_por_carga_o_distancia_minima(raiz->izquierdo, filtro, tipo_filtro);

    if (tipo_filtro == 1 && strcmp(raiz->carga_tipo, filtro) == 0) {
        printf("ID: %s, Origen: %s, Destino: %s, Compañia: %s, Distancia: %d km, Fecha: %d, Hora: %d, Tipo de carga: %s\n",
               raiz->id_tren, raiz->origen, raiz->destino, raiz->compania,
               raiz->distancia, raiz->fecha_operacion, raiz->hora_operacion,
               raiz->carga_tipo);
    } else if (tipo_filtro == 2 && raiz->distancia >= atoi(filtro)) {
        printf("ID: %s, Origen: %s, Destino: %s, Compañia: %s, Distancia: %d km, Fecha: %d, Hora: %d, Tipo de carga: %s\n",
               raiz->id_tren, raiz->origen, raiz->destino, raiz->compania,
               raiz->distancia, raiz->fecha_operacion, raiz->hora_operacion,
               raiz->carga_tipo);
    }

    listar_tren_por_carga_o_distancia_minima(raiz->derecho, filtro, tipo_filtro);
}
int tren_en_heap(Heap* heap, const char* id_tren) {
    for (int i = 0; i < heap->tamano; i++) {
        if (strcmp(heap->elementos[i].tren->id_tren, id_tren) == 0) {
            return 1; // Encontrado
        }
    }
    return 0; // No encontrado
}

Tren* encontrar_min(Tren* raiz) {
    Tren* actual = raiz;
    while (actual && actual->izquierdo != NULL) {
        actual = actual->izquierdo;
    }
    return actual;
}
// ===============================================
// === IMPLEMENTACIÓN DE FUNCIONES DEL HEAP ===
// ===============================================

/**
 * @brief Redimensiona dinámicamente el array del Heap (duplica capacidad).
 * @param heap Puntero al Heap a redimensionar.
 */
void redimensionar_heap(Heap* heap) {
    if (heap->tamano < heap->capacidad) return;

    heap->capacidad *= REDIMENSION_FACTOR;
    Salida* nuevo_elementos = (Salida*)realloc(heap->elementos, sizeof(Salida) * heap->capacidad);

    if (nuevo_elementos == NULL) {
        fprintf(stderr, "Error crítico: Fallo de reasignación de memoria para el Heap.\n");
        exit(1);
    }
    heap->elementos = nuevo_elementos;
    printf("(Heap redimensionado a capacidad %d)\n", heap->capacidad);
}

/**
 * @brief Inserta una nueva salida en el Min-Heap, manteniendo la propiedad de montículo.
 * @param heap Puntero al Heap.
 * @param nueva_salida La estructura Salida a insertar.
 */
void insertar_heap(Heap* heap, Salida nueva_salida) {
    redimensionar_heap(heap); 
    
    heap->tamano++;
    int i = heap->tamano - 1;
    heap->elementos[i] = nueva_salida;

    /// Operación de subir (percolate up) para restaurar la propiedad de Heap.
    while (i != 0 && heap->elementos[(i - 1) / 2].clave_salida > heap->elementos[i].clave_salida) {
        Salida temp = heap->elementos[i];
        heap->elementos[i] = heap->elementos[(i - 1) / 2];
        heap->elementos[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

/**
 * @brief Extrae la salida con la menor clave de prioridad (la próxima salida) del Min-Heap.
 * @param heap Puntero al Heap.
 * @return La estructura Salida extraída.
 */
Salida extraer_min_heap(Heap* heap) {
    if (heap->tamano <= 0) {
        fprintf(stderr, "Error: Intento de extraer de un montículo vacío.\n");
        Salida vacia = {NULL, 0};
        return vacia; 
    }
    
    Salida raiz = heap->elementos[0];
    heap->elementos[0] = heap->elementos[heap->tamano - 1];
    heap->tamano--;
    min_heapify(heap, 0);
    return raiz;
}

/**
 * @brief Restaura la propiedad de Min-Heap a partir del índice dado (operación de 'bajar' o sift-down).
 * @param heap Puntero al Heap.
 * @param i Índice a partir del cual se debe reestructurar el montículo.
 */
void min_heapify(Heap* heap, int i) {
    int izq = 2 * i + 1;
    int der = 2 * i + 2;
    int min = i;

    if (izq < heap->tamano && heap->elementos[izq].clave_salida < heap->elementos[min].clave_salida)
        min = izq;
    if (der < heap->tamano && heap->elementos[der].clave_salida < heap->elementos[min].clave_salida)
        min = der;
        
    if (min != i) {
        Salida temp = heap->elementos[i];
        heap->elementos[i] = heap->elementos[min];
        heap->elementos[min] = temp;
        min_heapify(heap, min);
    }
}
