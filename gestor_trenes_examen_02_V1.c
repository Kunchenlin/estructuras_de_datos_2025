#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @file gestor_trenes.c
 * @brief Sistema de gestión de trenes logísticos utilizando ABB (ordenado por destino) 
 * y Max-Heap (ordenado por distancia para priorizar operaciones).
 * 
 * Opciones implementadas:
 * - ABB: Registrar, buscar, eliminar, listar todos, filtrar por tipo de carga o distancia
 * - Max-Heap: Programar operación, consultar próxima, atender operación, 
 *   mostrar operaciones, ordenar con Heapsort
 */

// Constantes
#define MAX_CODE_LEN 50
#define MAX_HEAP 100

// Estructura del Tren (Nodo del ABB)
typedef struct tren {
    char id_tren[MAX_CODE_LEN];
    char compania[MAX_CODE_LEN];
    char origen[MAX_CODE_LEN];
    char destino[MAX_CODE_LEN];          // Clave principal del ABB
    int distancia;                        // Distancia en km
    int fecha_operacion;                  // AAAAMMDD
    int hora_operacion;                   // HHMM
    char tipo_carga[MAX_CODE_LEN];
    struct tren* izquierdo;
    struct tren* derecho;
} Tren;

// Estructura de Operación (Elemento del Max-Heap)
typedef struct operacion {
    Tren* tren;                          // Puntero al tren
    int clave_prioridad;                 // Distancia (a mayor distancia, mayor prioridad)
} Operacion;

// Estructura del Max-Heap
typedef struct heap {
    Operacion elementos[MAX_HEAP];
    int tamano;
} Heap;

// Declaración de funciones del ABB
Tren* insertar_tren(Tren* raiz, Tren* nuevo_tren);
Tren* buscar_tren_por_destino(Tren* raiz, const char* destino);
Tren* buscar_tren_por_id(Tren* raiz, const char* id_tren);
Tren* eliminar_tren(Tren* raiz, const char* destino, const char* id_tren);
Tren* encontrar_min(Tren* raiz);
void recorrer_inorden(Tren* raiz);
void filtrar_por_carga(Tren* raiz, const char* tipo_carga);
void filtrar_por_distancia_minima(Tren* raiz, int distancia_min);
void liberar_arbol(Tren* raiz);

// Declaración de funciones del Max-Heap
void insertar_heap(Heap* heap, Operacion nueva_op);
void max_heapify(Heap* heap, int i);
Operacion extraer_max_heap(Heap* heap);
void mostrar_heap(Heap* heap);
void heapsort_y_mostrar(Heap* heap);

// ========================================
// FUNCIÓN PRINCIPAL
// ========================================
int main() {
    Tren* arbol_trenes = NULL;
    Heap heap_operaciones = {.tamano = 0};
    int opcion;

    do {
        printf("\n============================================\n");
        printf(" Sistema de Gestión de Trenes Logísticos\n");
        printf("============================================\n");
        printf("--- ABB (Árbol Binario de Búsqueda) ---\n");
        printf("1. Registrar tren\n");
        printf("2. Buscar tren por destino\n");
        printf("3. Eliminar tren\n");
        printf("4. Listar todos los trenes (ordenados por destino)\n");
        printf("5. Listar trenes por tipo de carga o distancia mínima\n");
        printf("\n--- Max-Heap (Planificación de Operaciones) ---\n");
        printf("6. Programar operación\n");
        printf("7. Consultar próxima operación\n");
        printf("8. Atender operación\n");
        printf("9. Mostrar todas las operaciones programadas\n");
        printf("10. Mostrar planificación ordenada del día (Heapsort)\n");
        printf("\n11. Salir\n");
        printf("Elige una opción: ");
        
        if (scanf("%d", &opcion) != 1) {
            while(getchar() != '\n');
            opcion = 0;
        }

        char id_tren[MAX_CODE_LEN], destino[MAX_CODE_LEN], tipo_carga[MAX_CODE_LEN];
        Tren* tren_encontrado;
        int distancia_min, filtro_opcion;

        switch(opcion) {
            case 1: { // Registrar tren
                Tren* nuevo = (Tren*)malloc(sizeof(Tren));
                if (!nuevo) {
                    printf("Error de memoria.\n");
                    break;
                }
                
                printf("ID del tren: ");
                scanf("%s", nuevo->id_tren);
                printf("Compañía operadora: ");
                scanf("%s", nuevo->compania);
                printf("Origen: ");
                scanf("%s", nuevo->origen);
                printf("Destino: ");
                scanf("%s", nuevo->destino);
                printf("Distancia (km): ");
                scanf("%d", &nuevo->distancia);
                printf("Fecha de operación (AAAAMMDD): ");
                scanf("%d", &nuevo->fecha_operacion);
                printf("Hora de operación (HHMM): ");
                scanf("%d", &nuevo->hora_operacion);
                printf("Tipo de carga: ");
                scanf("%s", nuevo->tipo_carga);
                
                nuevo->izquierdo = nuevo->derecho = NULL;
                arbol_trenes = insertar_tren(arbol_trenes, nuevo);
                break;
            }

            case 2: { // Buscar por destino
                printf("Ingrese destino: ");
                scanf("%s", destino);
                printf("\n--- Trenes con destino a %s ---\n", destino);
                tren_encontrado = buscar_tren_por_destino(arbol_trenes, destino);
                if (!tren_encontrado) {
                    printf("No se encontraron trenes con ese destino.\n");
                }
                break;
            }

            case 3: { // Eliminar tren
                printf("Ingrese destino: ");
                scanf("%s", destino);
                printf("Ingrese ID del tren: ");
                scanf("%s", id_tren);
                arbol_trenes = eliminar_tren(arbol_trenes, destino, id_tren);
                break;
            }

            case 4: { // Listar todos (inorden)
                printf("\n--- Trenes ordenados por destino ---\n");
                recorrer_inorden(arbol_trenes);
                break;
            }

            case 5: { // Filtrar por carga o distancia
                printf("Filtrar por:\n");
                printf("1. Tipo de carga\n");
                printf("2. Distancia mínima\n");
                printf("Opción: ");
                scanf("%d", &filtro_opcion);
                
                if (filtro_opcion == 1) {
                    printf("Ingrese tipo de carga: ");
                    scanf("%s", tipo_carga);
                    printf("\n--- Trenes con carga: %s ---\n", tipo_carga);
                    filtrar_por_carga(arbol_trenes, tipo_carga);
                } else if (filtro_opcion == 2) {
                    printf("Ingrese distancia mínima (km): ");
                    scanf("%d", &distancia_min);
                    printf("\n--- Trenes con distancia >= %d km ---\n", distancia_min);
                    filtrar_por_distancia_minima(arbol_trenes, distancia_min);
                } else {
                    printf("Opción inválida.\n");
                }
                break;
            }

            case 6: { // Programar operación
                printf("Ingrese ID del tren: ");
                scanf("%s", id_tren);
                tren_encontrado = buscar_tren_por_id(arbol_trenes, id_tren);
                
                if (tren_encontrado) {
                    Operacion nueva_op;
                    nueva_op.tren = tren_encontrado;
                    nueva_op.clave_prioridad = tren_encontrado->distancia;
                    insertar_heap(&heap_operaciones, nueva_op);
                    printf("Operación programada correctamente (Prioridad: %d km).\n", 
                           tren_encontrado->distancia);
                } else {
                    printf("Error: Tren no encontrado.\n");
                }
                break;
            }

            case 7: { // Consultar próxima operación
                if (heap_operaciones.tamano > 0) {
                    Tren* t = heap_operaciones.elementos[0].tren;
                    printf("\n--- Próxima operación (mayor prioridad) ---\n");
                    printf("ID: %s | Destino: %s | Distancia: %d km | Compañía: %s\n",
                           t->id_tren, t->destino, t->distancia, t->compania);
                } else {
                    printf("No hay operaciones programadas.\n");
                }
                break;
            }

            case 8: { // Atender operación
                if (heap_operaciones.tamano > 0) {
                    Operacion op = extraer_max_heap(&heap_operaciones);
                    Tren* t = op.tren;
                    printf("\n--- Operación atendida ---\n");
                    printf("ID: %s | Destino: %s | Distancia: %d km\n",
                           t->id_tren, t->destino, t->distancia);
                    printf("Origen: %s | Compañía: %s | Carga: %s\n",
                           t->origen, t->compania, t->tipo_carga);
                } else {
                    printf("No hay operaciones para atender.\n");
                }
                break;
            }

            case 9: { // Mostrar heap
                printf("\n--- Operaciones programadas (array del heap) ---\n");
                mostrar_heap(&heap_operaciones);
                break;
            }

            case 10: { // Heapsort
                printf("\n--- Planificación ordenada descendente (Heapsort) ---\n");
                heapsort_y_mostrar(&heap_operaciones);
                break;
            }

            case 11: // Salir
                liberar_arbol(arbol_trenes);
                printf("Saliendo del sistema...\n");
                break;

            default:
                printf("Opción inválida.\n");
                while(getchar() != '\n');
        }
    } while (opcion != 11);

    return 0;
}

// ========================================
// IMPLEMENTACIÓN DEL ABB
// ========================================

// Insertar tren en el ABB (ordenado por destino, criterio secundario id_tren)
Tren* insertar_tren(Tren* raiz, Tren* nuevo_tren) {
    if (raiz == NULL) {
        printf("Tren registrado: %s (destino: %s)\n", nuevo_tren->id_tren, nuevo_tren->destino);
        return nuevo_tren;
    }
    
    int cmp = strcmp(nuevo_tren->destino, raiz->destino);
    
    if (cmp < 0) {
        raiz->izquierdo = insertar_tren(raiz->izquierdo, nuevo_tren);
    } else if (cmp > 0) {
        raiz->derecho = insertar_tren(raiz->derecho, nuevo_tren);
    } else {
        // Destinos iguales: usar ID como criterio secundario
        int cmp_id = strcmp(nuevo_tren->id_tren, raiz->id_tren);
        if (cmp_id < 0) {
            raiz->izquierdo = insertar_tren(raiz->izquierdo, nuevo_tren);
        } else if (cmp_id > 0) {
            raiz->derecho = insertar_tren(raiz->derecho, nuevo_tren);
        } else {
            printf("Error: Ya existe un tren con destino '%s' e ID '%s'.\n", 
                   nuevo_tren->destino, nuevo_tren->id_tren);
            free(nuevo_tren);
        }
    }
    return raiz;
}

// Buscar y mostrar todos los trenes con un destino específico
Tren* buscar_tren_por_destino(Tren* raiz, const char* destino) {
    if (raiz == NULL) return NULL;
    
    // Recorrer todo el árbol para encontrar todos los trenes con ese destino
    Tren* encontrado = NULL;
    
    buscar_tren_por_destino(raiz->izquierdo, destino);
    
    if (strcmp(raiz->destino, destino) == 0) {
        printf("ID: %s | Compañía: %s | Origen: %s | Distancia: %d km | Carga: %s\n",
               raiz->id_tren, raiz->compania, raiz->origen, raiz->distancia, raiz->tipo_carga);
        encontrado = raiz;
    }
    
    buscar_tren_por_destino(raiz->derecho, destino);
    
    return encontrado;
}

// Buscar tren por ID (recorrido completo)
Tren* buscar_tren_por_id(Tren* raiz, const char* id_tren) {
    if (raiz == NULL) return NULL;
    
    if (strcmp(raiz->id_tren, id_tren) == 0) {
        return raiz;
    }
    
    Tren* izq = buscar_tren_por_id(raiz->izquierdo, id_tren);
    if (izq) return izq;
    
    return buscar_tren_por_id(raiz->derecho, id_tren);
}

// Encontrar el nodo mínimo (más a la izquierda)
Tren* encontrar_min(Tren* raiz) {
    while (raiz && raiz->izquierdo) {
        raiz = raiz->izquierdo;
    }
    return raiz;
}

// Eliminar tren del ABB
Tren* eliminar_tren(Tren* raiz, const char* destino, const char* id_tren) {
    if (raiz == NULL) {
        printf("Error: Tren no encontrado.\n");
        return NULL;
    }
    
    int cmp = strcmp(destino, raiz->destino);
    
    if (cmp < 0) {
        raiz->izquierdo = eliminar_tren(raiz->izquierdo, destino, id_tren);
    } else if (cmp > 0) {
        raiz->derecho = eliminar_tren(raiz->derecho, destino, id_tren);
    } else {
        // Destino coincide, verificar ID
        int cmp_id = strcmp(id_tren, raiz->id_tren);
        
        if (cmp_id < 0) {
            raiz->izquierdo = eliminar_tren(raiz->izquierdo, destino, id_tren);
        } else if (cmp_id > 0) {
            raiz->derecho = eliminar_tren(raiz->derecho, destino, id_tren);
        } else {
            // Nodo encontrado - Casos de eliminación
            
            // Caso 1: Nodo hoja
            if (raiz->izquierdo == NULL && raiz->derecho == NULL) {
                printf("Tren eliminado: %s\n", raiz->id_tren);
                free(raiz);
                return NULL;
            }
            
            // Caso 2: Un hijo (derecho)
            if (raiz->izquierdo == NULL) {
                Tren* temp = raiz->derecho;
                printf("Tren eliminado: %s\n", raiz->id_tren);
                free(raiz);
                return temp;
            }
            
            // Caso 2: Un hijo (izquierdo)
            if (raiz->derecho == NULL) {
                Tren* temp = raiz->izquierdo;
                printf("Tren eliminado: %s\n", raiz->id_tren);
                free(raiz);
                return temp;
            }
            
            // Caso 3: Dos hijos - Usar sucesor inorden
            Tren* sucesor = encontrar_min(raiz->derecho);
            
            // Copiar datos del sucesor
            strcpy(raiz->id_tren, sucesor->id_tren);
            strcpy(raiz->compania, sucesor->compania);
            strcpy(raiz->origen, sucesor->origen);
            strcpy(raiz->destino, sucesor->destino);
            raiz->distancia = sucesor->distancia;
            raiz->fecha_operacion = sucesor->fecha_operacion;
            raiz->hora_operacion = sucesor->hora_operacion;
            strcpy(raiz->tipo_carga, sucesor->tipo_carga);
            
            // Eliminar el sucesor
            raiz->derecho = eliminar_tren(raiz->derecho, sucesor->destino, sucesor->id_tren);
        }
    }
    
    return raiz;
}

// Recorrido inorden (muestra trenes ordenados por destino)
void recorrer_inorden(Tren* raiz) {
    if (raiz == NULL) return;
    
    recorrer_inorden(raiz->izquierdo);
    printf("ID: %s | Destino: %s | Origen: %s | Distancia: %d km | Compañía: %s | Carga: %s\n",
           raiz->id_tren, raiz->destino, raiz->origen, raiz->distancia, 
           raiz->compania, raiz->tipo_carga);
    recorrer_inorden(raiz->derecho);
}

// Filtrar por tipo de carga
void filtrar_por_carga(Tren* raiz, const char* tipo_carga) {
    if (raiz == NULL) return;
    
    filtrar_por_carga(raiz->izquierdo, tipo_carga);
    
    if (strcmp(raiz->tipo_carga, tipo_carga) == 0) {
        printf("ID: %s | Destino: %s | Distancia: %d km | Compañía: %s\n",
               raiz->id_tren, raiz->destino, raiz->distancia, raiz->compania);
    }
    
    filtrar_por_carga(raiz->derecho, tipo_carga);
}

// Filtrar por distancia mínima
void filtrar_por_distancia_minima(Tren* raiz, int distancia_min) {
    if (raiz == NULL) return;
    
    filtrar_por_distancia_minima(raiz->izquierdo, distancia_min);
    
    if (raiz->distancia >= distancia_min) {
        printf("ID: %s | Destino: %s | Distancia: %d km | Carga: %s\n",
               raiz->id_tren, raiz->destino, raiz->distancia, raiz->tipo_carga);
    }
    
    filtrar_por_distancia_minima(raiz->derecho, distancia_min);
}

// Liberar memoria del árbol
void liberar_arbol(Tren* raiz) {
    if (raiz == NULL) return;
    liberar_arbol(raiz->izquierdo);
    liberar_arbol(raiz->derecho);
    free(raiz);
}

// ========================================
// IMPLEMENTACIÓN DEL MAX-HEAP
// ========================================

// Insertar en Max-Heap y realizar heapify-up
void insertar_heap(Heap* heap, Operacion nueva_op) {
    if (heap->tamano >= MAX_HEAP) {
        printf("Error: Heap lleno.\n");
        return;
    }
    
    // Insertar al final
    int i = heap->tamano;
    heap->elementos[i] = nueva_op;
    heap->tamano++;
    
    // Heapify-up: subir el elemento mientras sea mayor que su padre
    while (i > 0 && heap->elementos[(i-1)/2].clave_prioridad < heap->elementos[i].clave_prioridad) {
        Operacion temp = heap->elementos[i];
        heap->elementos[i] = heap->elementos[(i-1)/2];
        heap->elementos[(i-1)/2] = temp;
        i = (i-1)/2;
    }
}

// Max-heapify: mantener propiedad de Max-Heap hacia abajo
void max_heapify(Heap* heap, int i) {
    int izq = 2*i + 1;
    int der = 2*i + 2;
    int mayor = i;
    
    if (izq < heap->tamano && 
        heap->elementos[izq].clave_prioridad > heap->elementos[mayor].clave_prioridad) {
        mayor = izq;
    }
    
    if (der < heap->tamano && 
        heap->elementos[der].clave_prioridad > heap->elementos[mayor].clave_prioridad) {
        mayor = der;
    }
    
    if (mayor != i) {
        Operacion temp = heap->elementos[i];
        heap->elementos[i] = heap->elementos[mayor];
        heap->elementos[mayor] = temp;
        max_heapify(heap, mayor);
    }
}

// Extraer máximo (raíz) del Max-Heap
Operacion extraer_max_heap(Heap* heap) {
    if (heap->tamano <= 0) {
        Operacion vacia = {NULL, 0};
        return vacia;
    }
    
    Operacion max = heap->elementos[0];
    heap->elementos[0] = heap->elementos[heap->tamano - 1];
    heap->tamano--;
    max_heapify(heap, 0);
    
    return max;
}

// Mostrar heap como array
void mostrar_heap(Heap* heap) {
    if (heap->tamano == 0) {
        printf("No hay operaciones programadas.\n");
        return;
    }
    
    for (int i = 0; i < heap->tamano; i++) {
        Tren* t = heap->elementos[i].tren;
        printf("[%d] ID: %s | Destino: %s | Prioridad: %d km\n",
               i, t->id_tren, t->destino, heap->elementos[i].clave_prioridad);
    }
}

// Heapsort: ordenar y mostrar sin modificar el heap original
void heapsort_y_mostrar(Heap* heap) {
    if (heap->tamano == 0) {
        printf("No hay operaciones para ordenar.\n");
        return;
    }
    
    // Crear copia del heap
    Heap copia;
    copia.tamano = heap->tamano;
    for (int i = 0; i < heap->tamano; i++) {
        copia.elementos[i] = heap->elementos[i];
    }
    
    // Extraer elementos ordenadamente (orden descendente)
    int tam_original = copia.tamano;
    for (int i = 0; i < tam_original; i++) {
        Operacion op = extraer_max_heap(&copia);
        Tren* t = op.tren;
        printf("%d. ID: %s | Destino: %s | Distancia: %d km | Compañía: %s\n",
               i+1, t->id_tren, t->destino, t->distancia, t->compania);
    }
}      
