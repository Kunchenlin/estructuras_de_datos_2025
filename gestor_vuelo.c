#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Necesario para la generación aleatoria

/**
 * @file aeropuerto_manager.c
 * @brief Implementación de un sistema de gestión aeroportuaria utilizando un Árbol Binario de Búsqueda (ABB) para vuelos y un Min-Heap para la planificación de salidas.
 * @author Kun Chen Lin
 * @date Diciembre 2025
 */

// Constantes
/** @brief Capacidad inicial del array de salidas del Min-Heap. */
#define MAX_NODOS 100 
/** @brief Longitud máxima para el código de vuelo. */
#define MAX_CODE_LEN 10
/** @brief Longitud máxima para cadenas de texto (origen, destino, aerolínea). */
#define MAX_STR_LEN 50
/** @brief Factor de crecimiento para la redimensión dinámica del Heap. */
#define REDIMENSION_FACTOR 2

// Definición de estructuras

/**
 * @struct vuelo
 * @brief Representa la información de un vuelo y un nodo en el Árbol Binario de Búsqueda (ABB).
 * La clave de ordenación del ABB es el 'codigo_vuelo'.
 */
typedef struct vuelo {
    char codigo_vuelo[MAX_CODE_LEN]; /**< Código único del vuelo (clave del ABB). */
    char origen[MAX_STR_LEN];        /**< Ciudad de origen. */
    char destino[MAX_STR_LEN];       /**< Ciudad de destino. */
    char aerolinea[MAX_STR_LEN];     /**< Nombre de la aerolínea. */
    int fecha_salida;                /**< Fecha programada de salida (AAAAMMDD). */
    int hora_salida;                 /**< Hora programada de salida (HHMM). */
    struct vuelo* izquierdo;         /**< Puntero al subárbol izquierdo. */
    struct vuelo* derecho;           /**< Puntero al subárbol derecho. */
} Vuelo;

/**
 * @struct salida
 * @brief Representa una salida programada y un elemento en el Min-Heap.
 * Contiene un puntero al Vuelo y la clave de prioridad.
 */
typedef struct salida {
    Vuelo* vuelo;            /**< Puntero al nodo Vuelo asociado. */
    int clave_salida;        /**< Clave de prioridad para el Heap (AAAAMMDDHHMM). */
} Salida;

/**
 * @struct heap
 * @brief Estructura que implementa un Min-Heap (Montículo de Mínimos) dinámico.
 * Almacena las salidas ordenadas por la clave_salida.
 */
typedef struct heap {
    Salida* elementos;       /**< Array de elementos Salida. */
    int capacidad;           /**< Capacidad máxima actual del array. */
    int tamano;              /**< Número actual de elementos en el Heap. */
} Heap;

// Declaración de funciones
// ABB

/**
 * @brief Inserta un nuevo vuelo en el Árbol Binario de Búsqueda (ABB) basado en el código de vuelo.
 * @param raiz La raíz del ABB.
 * @param nuevo_vuelo El nodo Vuelo a insertar.
 * @return La nueva raíz del ABB.
 */
Vuelo* insertar_vuelo(Vuelo* raiz, Vuelo* nuevo_vuelo);

/**
 * @brief Busca un vuelo en el ABB por su código.
 * @param raiz La raíz del ABB.
 * @param codigo_vuelo El código del vuelo a buscar.
 * @return Puntero al nodo Vuelo si se encuentra, NULL en caso contrario.
 */
Vuelo* buscar_vuelo(Vuelo* raiz, const char* codigo_vuelo);

/**
 * @brief Elimina un vuelo del ABB, con validación adicional para asegurar que no esté programado en el Heap.
 * @param raiz La raíz del ABB.
 * @param codigo_vuelo El código del vuelo a eliminar.
 * @param heap_salidas Puntero al Heap de salidas para la validación.
 * @return La nueva raíz del ABB.
 */
Vuelo* eliminar_vuelo(Vuelo* raiz, const char* codigo_vuelo, Heap* heap_salidas);

/**
 * @brief Encuentra el nodo con el valor más pequeño (mínimo) en un subárbol.
 * @param raiz La raíz del subárbol.
 * @return Puntero al nodo mínimo.
 */
Vuelo* encontrar_min(Vuelo* raiz);

/**
 * @brief Recorre el ABB en orden (Inorden) e imprime la información de los vuelos.
 * @param raiz La raíz del ABB.
 */
void recorrer_inorden(Vuelo* raiz);

/**
 * @brief Libera recursivamente toda la memoria asignada al ABB.
 * @param raiz La raíz del ABB.
 */
void liberar_arbol(Vuelo* raiz);

/**
 * @brief Verifica si un vuelo específico está actualmente programado en el Heap de salidas.
 * @param heap Puntero al Heap.
 * @param codigo_vuelo El código del vuelo a verificar.
 * @return 1 si el vuelo está en el Heap, 0 en caso contrario.
 */
int vuelo_en_heap(Heap* heap, const char* codigo_vuelo);

// Heap

/**
 * @brief Inserta una nueva salida en el Min-Heap, manteniendo la propiedad de montículo.
 * Redimensiona el Heap si es necesario.
 * @param heap Puntero al Heap.
 * @param nueva_salida La estructura Salida a insertar.
 */
void insertar_heap(Heap* heap, Salida nueva_salida);

/**
 * @brief Extrae la salida con la menor clave de prioridad (la próxima salida) del Min-Heap.
 * @param heap Puntero al Heap.
 * @return La estructura Salida extraída.
 */
Salida extraer_min_heap(Heap* heap);

/**
 * @brief Restaura la propiedad de Min-Heap a partir del índice dado (operación de 'bajar').
 * @param heap Puntero al Heap.
 * @param i Índice a partir del cual se debe reestructurar el montículo.
 */
void min_heapify(Heap* heap, int i);

/**
 * @brief Muestra la planificación completa de salidas utilizando el algoritmo Heapsort.
 * La función opera sobre una copia del Heap para no alterar el orden de prioridades original.
 * @param heap Puntero al Heap de salidas.
 */
void heapsort_y_mostrar(Heap* heap);

/**
 * @brief Muestra todos los vuelos en el ABB que coincidan con un destino o aerolínea.
 * @param raiz La raíz del ABB.
 * @param filtro El destino o aerolínea a buscar.
 * @param tipo_filtro 1 para buscar por destino, 2 para buscar por aerolínea.
 */
void listar_vuelos_por_destino_o_aerolinea(Vuelo* raiz, const char* filtro, int tipo_filtro);

// Utilidades

/**
 * @brief Genera e inserta automáticamente N vuelos con datos aleatorios en el ABB y programa sus salidas en el Heap.
 * @param arbol Puntero al puntero de la raíz del ABB.
 * @param monticulo Puntero al Heap de salidas.
 * @param num_vuelos Número de vuelos a generar.
 */
void generar_vuelos_automaticos(Vuelo** arbol, Heap* monticulo, int num_vuelos);

/**
 * @brief Genera un código de vuelo alfanumérico aleatorio (e.g., AB123).
 * @param buffer Buffer donde se almacenará el código generado.
 * @return Puntero al buffer.
 */
char* generar_codigo_aleatorio(char* buffer);

/**
 * @brief Genera una fecha aleatoria en formato AAAAMMDD.
 * @return La fecha generada.
 */
int generar_fecha_aleatoria();

/**
 * @brief Genera una hora aleatoria en formato HHMM.
 * @return La hora generada.
 */
int generar_hora_aleatoria();


/**
 * @brief Función principal del sistema de gestión aeroportuaria.
 * Implementa el menú interactivo y gestiona las estructuras de datos (ABB y Heap).
 * @return 0 si la ejecución es exitosa, 1 en caso de error de asignación de memoria.
 */
int main() {
    Vuelo* arbol_vuelos = NULL;  // Árbol de vuelos
    // Inicialización del generador de números aleatorios
    srand((unsigned)time(NULL)); 

    // Inicialización más robusta del Heap (con gestión de errores)
    Heap monticulo_salidas = {.elementos = (Salida*)malloc(sizeof(Salida) * MAX_NODOS), .capacidad = MAX_NODOS, .tamano = 0};
    if (monticulo_salidas.elementos == NULL) {
        printf("Error fatal: No se pudo asignar memoria para el montículo.\n");
        return 1;
    }
    
    int opcion;
    do {
        printf("\n============================================\n");
        printf(" Sistema de Gestión Aeroportuaria (ABB & Heap)\n");
        printf("============================================\n");
        printf("1. Registrar nuevo vuelo\n");
        printf("2. Buscar vuelo por código\n");
        printf("3. Eliminar vuelo\n");
        printf("4. Listar todos los vuelos (código: inorden)\n");
        printf("5. Listar vuelos por destino o aerolínea\n");
        printf("6. Programar salida de un vuelo\n");
        printf("7. Consultar próxima salida\n");
        printf("8. Despegar vuelo (atender próxima salida)\n");
        printf("9. Mostrar todas las salidas del montículo\n");
        printf("10. Mostrar planificación completa ordenada (Heapsort)\n");
        printf("--- Opciones Automáticas ---\n");
        printf("12. Generar Vuelos y Salidas Automáticas\n"); // NUEVA FUNCIÓN
        printf("11. Salir\n");
        printf("Elige una opción: ");
        
        // Manejo de entrada robusto
        if (scanf("%d", &opcion) != 1) { 
            while(getchar() != '\n');
            opcion = 0; // Opción inválida
        }

        char codigo_vuelo[MAX_CODE_LEN];
        char destino[MAX_STR_LEN];
        char aerolinea[MAX_STR_LEN];
        int fecha_salida, hora_salida;
        Vuelo* vuelo;
        Salida nueva_salida;
        int num_vuelos_a_generar;
        
        switch(opcion) {
            case 1:
                vuelo = (Vuelo*)malloc(sizeof(Vuelo));
                if (vuelo == NULL) { printf("Error: Fallo de asignación de memoria.\n"); break; }
                printf("Ingrese código de vuelo: "); scanf("%s", vuelo->codigo_vuelo);
                printf("Ingrese ciudad de origen: "); scanf("%s", vuelo->origen);
                printf("Ingrese ciudad de destino: "); scanf("%s", vuelo->destino);
                printf("Ingrese aerolínea: "); scanf("%s", vuelo->aerolinea);
                printf("Ingrese fecha de salida (AAAAMMDD): "); scanf("%d", &vuelo->fecha_salida);
                printf("Ingrese hora de salida (HHMM): "); scanf("%d", &vuelo->hora_salida);
                vuelo->izquierdo = vuelo->derecho = NULL; // Inicialización de punteros
                arbol_vuelos = insertar_vuelo(arbol_vuelos, vuelo);
                break;

            case 2:
                printf("Ingrese código de vuelo: "); scanf("%s", codigo_vuelo);
                vuelo = buscar_vuelo(arbol_vuelos, codigo_vuelo);
                if (vuelo) {
                    printf("Vuelo encontrado: %s, Origen: %s, Destino: %s, Aerolínea: %s, Fecha: %d, Hora: %d\n", vuelo->codigo_vuelo, vuelo->origen, vuelo->destino, vuelo->aerolinea, vuelo->fecha_salida, vuelo->hora_salida);
                } else {
                    printf("Vuelo no encontrado.\n");
                }
                break;

            case 3:
                printf("Ingrese código de vuelo a eliminar: "); scanf("%s", codigo_vuelo);
                arbol_vuelos = eliminar_vuelo(arbol_vuelos, codigo_vuelo, &monticulo_salidas);
                break;

            case 4:
                printf("--- Vuelos en orden por código ---\n");
                recorrer_inorden(arbol_vuelos);
                break;

            case 5:
                printf("¿Desea buscar por destino (1) o aerolínea (2)? ");
                int tipo_filtro;
                if (scanf("%d", &tipo_filtro) != 1) { 
                     printf("Entrada inválida.\n"); break; 
                }
                if (tipo_filtro == 1) {
                    printf("Ingrese destino: "); scanf("%s", destino);
                    printf("--- Vuelos a %s ---\n", destino);
                    listar_vuelos_por_destino_o_aerolinea(arbol_vuelos, destino, 1);
                } else if (tipo_filtro == 2) {
                    printf("Ingrese aerolínea: "); scanf("%s", aerolinea);
                    printf("--- Vuelos de %s ---\n", aerolinea);
                    listar_vuelos_por_destino_o_aerolinea(arbol_vuelos, aerolinea, 2);
                } else {
                    printf("Opción inválida.\n");
                }
                break;

            case 6:
                printf("Ingrese código de vuelo a programar: "); scanf("%s", codigo_vuelo);
                vuelo = buscar_vuelo(arbol_vuelos, codigo_vuelo);
                if (vuelo) {
                    printf("Ingrese fecha de salida (AAAAMMDD): "); scanf("%d", &fecha_salida);
                    printf("Ingrese hora de salida (HHMM): "); scanf("%d", &hora_salida);
                    nueva_salida.vuelo = vuelo;
                    nueva_salida.clave_salida = fecha_salida * 10000 + hora_salida;
                    insertar_heap(&monticulo_salidas, nueva_salida);
                    printf("Salida programada correctamente.\n");
                } else {
                    printf("Error: Vuelo no encontrado.\n");
                }
                break;

            case 7:
                if (monticulo_salidas.tamano > 0) {
                    Salida salida = monticulo_salidas.elementos[0];
                    printf("Próxima salida: %s (%s a %s), Fecha: %d, Hora: %d\n", salida.vuelo->codigo_vuelo, salida.vuelo->aerolinea, salida.vuelo->destino, salida.vuelo->fecha_salida, salida.vuelo->hora_salida);
                } else {
                    printf("No hay salidas programadas.\n");
                }
                break;

            case 8:
                if (monticulo_salidas.tamano > 0) {
                    Salida salida = extraer_min_heap(&monticulo_salidas);
                    printf("✅ Vuelo despegando: %s, %s, Destino: %s, Hora: %d\n", salida.vuelo->codigo_vuelo, salida.vuelo->aerolinea, salida.vuelo->destino, salida.vuelo->hora_salida);
                } else {
                    printf("No hay vuelos para despegar.\n");
                }
                break;

            case 9:
                printf("--- Salidas actuales en el Montículo (Orden no garantizado, solo estructura) ---\n");
                for (int i = 0; i < monticulo_salidas.tamano; i++) {
                    printf("[%d] %s (Clave: %d), Destino: %s, Aerolínea: %s\n", i, monticulo_salidas.elementos[i].vuelo->codigo_vuelo, monticulo_salidas.elementos[i].clave_salida, monticulo_salidas.elementos[i].vuelo->destino, monticulo_salidas.elementos[i].vuelo->aerolinea);
                }
                break;

            case 10:
                printf("--- Planificación completa ordenada por hora (Heapsort) ---\n");
                heapsort_y_mostrar(&monticulo_salidas);
                break;
                
            case 12: // NUEVA FUNCIÓN
                printf("¿Cuántos vuelos desea generar? ");
                if (scanf("%d", &num_vuelos_a_generar) != 1 || num_vuelos_a_generar <= 0) {
                    printf("Número inválido.\n");
                    break;
                }
                generar_vuelos_automaticos(&arbol_vuelos, &monticulo_salidas, num_vuelos_a_generar);
                break;

            case 11:
                liberar_arbol(arbol_vuelos); // Liberar ABB
                free(monticulo_salidas.elementos); // Liberar Heap
                printf("Saliendo del programa y liberando memoria...\n");
                break;

            default:
                printf("Opción inválida, por favor ingrese una opción válida.\n");
                // Limpiar buffer de entrada en caso de caracter inválido
                while(getchar() != '\n'); 
        }
    } while (opcion != 11);

    return 0;
}

// ===============================================
// === IMPLEMENTACIÓN DE FUNCIONES DEL ABB ===
// ===============================================

Vuelo* insertar_vuelo(Vuelo* raiz, Vuelo* nuevo_vuelo) {
    if (raiz == NULL) {
        return nuevo_vuelo;
    }
    int cmp = strcmp(nuevo_vuelo->codigo_vuelo, raiz->codigo_vuelo);
    if (cmp < 0) {
        raiz->izquierdo = insertar_vuelo(raiz->izquierdo, nuevo_vuelo);
    } else if (cmp > 0) {
        raiz->derecho = insertar_vuelo(raiz->derecho, nuevo_vuelo);
    } else {
        printf("Error: El código de vuelo ya existe (%s). No insertado.\n", nuevo_vuelo->codigo_vuelo);
        free(nuevo_vuelo); // Liberar nodo si está duplicado
    }
    return raiz;
}

Vuelo* buscar_vuelo(Vuelo* raiz, const char* codigo_vuelo) {
    if (raiz == NULL) return NULL;
    int cmp = strcmp(codigo_vuelo, raiz->codigo_vuelo);
    if (cmp == 0) return raiz;
    if (cmp < 0) return buscar_vuelo(raiz->izquierdo, codigo_vuelo);
    return buscar_vuelo(raiz->derecho, codigo_vuelo);
}

int vuelo_en_heap(Heap* heap, const char* codigo_vuelo) {
    for (int i = 0; i < heap->tamano; i++) {
        if (strcmp(heap->elementos[i].vuelo->codigo_vuelo, codigo_vuelo) == 0) {
            return 1;
        }
    }
    return 0;
}

Vuelo* eliminar_vuelo(Vuelo* raiz, const char* codigo_vuelo, Heap* heap_salidas) {
    if (raiz == NULL) {
        printf("Error: Vuelo %s no encontrado.\n", codigo_vuelo);
        return NULL;
    }
    
    // **VALIDACIÓN CRÍTICA**: No permitir eliminar si está programado
    if (vuelo_en_heap(heap_salidas, codigo_vuelo)) {
        printf("Error: El vuelo %s no se puede eliminar porque está programado en una salida.\n", codigo_vuelo);
        return raiz;
    }

    int cmp = strcmp(codigo_vuelo, raiz->codigo_vuelo);

    if (cmp < 0) {
        raiz->izquierdo = eliminar_vuelo(raiz->izquierdo, codigo_vuelo, heap_salidas);
    } else if (cmp > 0) {
        raiz->derecho = eliminar_vuelo(raiz->derecho, codigo_vuelo, heap_salidas);
    } else {
        // Nodo encontrado
        Vuelo* temp;
        
        // Caso 1: Cero o un hijo
        if (raiz->izquierdo == NULL) {
            temp = raiz->derecho;
            free(raiz);
            printf("Vuelo %s eliminado.\n", codigo_vuelo);
            return temp;
        } else if (raiz->derecho == NULL) {
            temp = raiz->izquierdo;
            free(raiz);
            printf("Vuelo %s eliminado.\n", codigo_vuelo);
            return temp;
        }

        // Caso 2: Dos hijos (Sucesor inorden)
        temp = encontrar_min(raiz->derecho);
        
        // Copiar TODOS los datos del sucesor (CORRECCIÓN)
        strcpy(raiz->codigo_vuelo, temp->codigo_vuelo);
        strcpy(raiz->origen, temp->origen);
        strcpy(raiz->destino, temp->destino);
        strcpy(raiz->aerolinea, temp->aerolinea);
        raiz->fecha_salida = temp->fecha_salida;
        raiz->hora_salida = temp->hora_salida;

        // Eliminar el sucesor
        raiz->derecho = eliminar_vuelo(raiz->derecho, temp->codigo_vuelo, heap_salidas); 
    }

    return raiz;
}

Vuelo* encontrar_min(Vuelo* raiz) {
    while (raiz && raiz->izquierdo) {
        raiz = raiz->izquierdo;
    }
    return raiz;
}

void recorrer_inorden(Vuelo* raiz) {
    if (raiz == NULL) return;
    recorrer_inorden(raiz->izquierdo);
    printf("Vuelo: %s, Origen: %s, Destino: %s, Aerolínea: %s, Fecha: %d, Hora: %d\n", 
              raiz->codigo_vuelo, raiz->origen, raiz->destino, raiz->aerolinea, raiz->fecha_salida, raiz->hora_salida);
    recorrer_inorden(raiz->derecho);
}

void liberar_arbol(Vuelo* raiz) {
    if (raiz == NULL) return;
    liberar_arbol(raiz->izquierdo);
    liberar_arbol(raiz->derecho);
    free(raiz);
}

void listar_vuelos_por_destino_o_aerolinea(Vuelo* raiz, const char* filtro, int tipo_filtro) {
    if (raiz == NULL) return;

    // Recorrido DFS, no garantiza orden por código pero es simple
    listar_vuelos_por_destino_o_aerolinea(raiz->izquierdo, filtro, tipo_filtro);

    if ((tipo_filtro == 1 && strcmp(raiz->destino, filtro) == 0) || 
        (tipo_filtro == 2 && strcmp(raiz->aerolinea, filtro) == 0)) {
        printf("Vuelo: %s, Origen: %s, Destino: %s, Aerolínea: %s, Fecha: %d, Hora: %d\n",
                raiz->codigo_vuelo, raiz->origen, raiz->destino, raiz->aerolinea,
                raiz->fecha_salida, raiz->hora_salida);
    }

    listar_vuelos_por_destino_o_aerolinea(raiz->derecho, filtro, tipo_filtro);
}

// ===============================================
// === IMPLEMENTACIÓN DE FUNCIONES DEL HEAP ===
// ===============================================

/**
 * @brief Redimensiona dinámicamente el array del Heap cuando se queda sin capacidad.
 * @param heap Puntero al Heap a redimensionar.
 */
void redimensionar_heap(Heap* heap) {
    if (heap->tamano < heap->capacidad) return;

    heap->capacidad *= REDIMENSION_FACTOR;
    Salida* nuevo_elementos = (Salida*)realloc(heap->elementos, sizeof(Salida) * heap->capacidad);

    if (nuevo_elementos == NULL) {
        printf("Error crítico: Fallo de reasignación de memoria para el Heap.\n");
        exit(1);
    }
    heap->elementos = nuevo_elementos;
    printf("(Heap redimensionado a capacidad %d)\n", heap->capacidad);
}

void insertar_heap(Heap* heap, Salida nueva_salida) {
    redimensionar_heap(heap); // Verificar y redimensionar antes de insertar
    
    heap->tamano++;
    int i = heap->tamano - 1;
    heap->elementos[i] = nueva_salida;

    // Operación de subir (percolate up) para mantener la propiedad de Min-Heap
    while (i != 0 && heap->elementos[(i - 1) / 2].clave_salida > heap->elementos[i].clave_salida) {
        Salida temp = heap->elementos[i];
        heap->elementos[i] = heap->elementos[(i - 1) / 2];
        heap->elementos[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

Salida extraer_min_heap(Heap* heap) {
    if (heap->tamano <= 0) {
        // En lugar de salir, devolver una salida vacía y manejar el error
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

void heapsort_y_mostrar(Heap* heap) {
    if (heap->tamano == 0) {
        printf("El montículo de salidas está vacío.\n");
        return;
    }
    
    int original_tamano = heap->tamano;
    
    // 1. Crear una copia profunda de los elementos
    Salida* elementos_copia = (Salida*)malloc(sizeof(Salida) * original_tamano);
    if (elementos_copia == NULL) {
        printf("Error: Fallo de asignación de memoria para el Heapsort.\n");
        return;
    }
    memcpy(elementos_copia, heap->elementos, sizeof(Salida) * original_tamano);

    // 2. Crear una estructura Heap temporal que apunte a la copia
    Heap heap_temp = {
        .elementos = elementos_copia,
        .capacidad = original_tamano,
        .tamano = original_tamano
    };
    
    // 3. Ordenar (Extraer el mínimo repetidamente)
    for (int i = 0; i < original_tamano; i++) {
        Salida salida = extraer_min_heap(&heap_temp);
        printf("Salida: %s, Destino: %s, Fecha: %d, Hora: %d\n",
                salida.vuelo->codigo_vuelo, salida.vuelo->destino,
                salida.vuelo->fecha_salida, salida.vuelo->hora_salida);
    }

    // 4. Liberar la memoria de la copia
    free(elementos_copia);
}


// ===============================================
// === IMPLEMENTACIÓN DE FUNCIONES DE UTILIDAD ===
// ===============================================

char* generar_codigo_aleatorio(char* buffer) {
    const char letras[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char numeros[] = "0123456789";
    // Formato ejemplo: AB123
    buffer[0] = letras[rand() % (sizeof(letras) - 1)];
    buffer[1] = letras[rand() % (sizeof(letras) - 1)];
    buffer[2] = numeros[rand() % (sizeof(numeros) - 1)];
    buffer[3] = numeros[rand() % (sizeof(numeros) - 1)];
    buffer[4] = numeros[rand() % (sizeof(numeros) - 1)];
    buffer[5] = '\0'; // Terminador de cadena
    return buffer;
}

int generar_fecha_aleatoria() {
    // Generar fecha entre el 20251204 y 20251231
    int dia = 4 + (rand() % 28); // 4 a 31
    return 20251200 + dia; 
}

int generar_hora_aleatoria() {
    // Generar hora entre 00:00 y 23:59
    int hora = rand() % 24;
    int minuto = rand() % 60;
    return hora * 100 + minuto;
}

// Lista de posibles aerolíneas y destinos
const char* AEROLINEAS[] = {"Iberia", "Ryanair", "AirEuropa", "Vueling", "Latam", "AmericanAirlines"};
const char* DESTINOS[] = {"Madrid", "Barcelona", "Paris", "Londres", "Roma", "NuevaYork", "MexicoDF", "BuenosAires"};
const int NUM_AERO = sizeof(AEROLINEAS) / sizeof(AEROLINEAS[0]);
const int NUM_DEST = sizeof(DESTINOS) / sizeof(DESTINOS[0]);


void generar_vuelos_automaticos(Vuelo** arbol, Heap* monticulo, int num_vuelos) {
    printf("Iniciando generación de %d vuelos automáticos...\n", num_vuelos);
    
    int vuelos_insertados = 0;
    int intentos = 0;
    
    // Limite para evitar bucles infinitos por códigos duplicados
    while (vuelos_insertados < num_vuelos && intentos < num_vuelos * 5) {
        Vuelo* vuelo = (Vuelo*)malloc(sizeof(Vuelo));
        if (vuelo == NULL) { 
            printf("Error: Fallo de asignación de memoria durante la generación automática.\n");
            break;
        }

        // Generar datos aleatorios
        generar_codigo_aleatorio(vuelo->codigo_vuelo);
        strcpy(vuelo->origen, DESTINOS[rand() % NUM_DEST]);
        strcpy(vuelo->destino, DESTINOS[rand() % NUM_DEST]);
        // Asegurar que Origen y Destino no sean iguales
        while (strcmp(vuelo->origen, vuelo->destino) == 0) {
             strcpy(vuelo->destino, DESTINOS[rand() % NUM_DEST]);
        }

        strcpy(vuelo->aerolinea, AEROLINEAS[rand() % NUM_AERO]);
        
        // La fecha/hora real se usará para la salida programada
        vuelo->fecha_salida = 0; 
        vuelo->hora_salida = 0;
        vuelo->izquierdo = vuelo->derecho = NULL;

        // Intentar insertar en el ABB. Si el código está duplicado, se libera dentro de insertar_vuelo.
        Vuelo* arbol_previo = *arbol;
        *arbol = insertar_vuelo(*arbol, vuelo);
        
        // Si la raíz del árbol cambió o si *arbol no es la raíz previa Y no hubo error de duplicado, se insertó
        if (*arbol != arbol_previo || buscar_vuelo(*arbol, vuelo->codigo_vuelo) == vuelo) {
            
            // Programar salida en el Heap
            Salida nueva_salida;
            nueva_salida.vuelo = vuelo;
            nueva_salida.clave_salida = generar_fecha_aleatoria() * 10000 + generar_hora_aleatoria();
            
            // Sincronizar la fecha/hora en la estructura Vuelo para una mejor visualización en el ABB
            vuelo->fecha_salida = nueva_salida.clave_salida / 10000;
            vuelo->hora_salida = nueva_salida.clave_salida % 10000;
            
            insertar_heap(monticulo, nueva_salida);
            vuelos_insertados++;
        }
        intentos++;
    }

    printf("Se generaron y programaron %d vuelos exitosamente.\n", vuelos_insertados);
}
