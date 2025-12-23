#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @file gestor_drones.c
 * @brief Implementación de un sistema de gestión de drones de reparto mediante un ABB Vectorial y un Max-Heap Dinámico
 */

// Constantes
#define MAX_NODOS 100
#define MAX_ID_LEN 20
#define MAX_STR_LEN 50
#define MAX_CARGA_LEN 30
#define POS_VACIA -1
#define MAX_HEAP 100 // Capacidad inicial

// Estructuras
typedef struct {
    char id_dron[MAX_ID_LEN];
    char compania[MAX_STR_LEN];
    char zona_origen[MAX_STR_LEN];
    char zona_entrega[MAX_STR_LEN];
    int nivel_bateria;
    int fecha_mision;
    int hora_mision;
    char tipo_carga[MAX_CARGA_LEN];
    int izquierdo;
    int derecho;
} Dron;

typedef struct {
    Dron elementos[MAX_NODOS];
    int raiz;
    int tamano;
    int siguiente_libre;
} ABB;

typedef struct {
    int indice_dron;
    int prioridad; // Usamos el nivel de batería como prioridad (Max-Heap)
} ElementoHeap;

typedef struct {
    ElementoHeap* elementos;
    int capacidad;
    int tamano;
} MaxHeap;

// --- Prototipos ---
void inicializarABB(ABB* abb);
int compararDrones(const Dron* d1, const Dron* d2);
int insertarDronABB(ABB* abb, Dron nuevo_dron);
void buscarDronesPorZona(const ABB* abb, const char* zona);
int buscarIndiceDron(const ABB* abb, const char* zona, const char* id);
int eliminarDronABB(ABB* abb, const char* zona, const char* id, MaxHeap* heap);
void recorrerInordenABB(const ABB* abb);
void listarDronesFiltrados(const ABB* abb, const char* filtro, int tipo_filtro);
int dronEnHeap(const MaxHeap* heap, int indice_dron);

MaxHeap* crearMaxHeap(int capacidad);
void insertarHeap(MaxHeap* heap, ElementoHeap nuevo_elemento);
ElementoHeap consultarMaxHeap(const MaxHeap* heap);
ElementoHeap extraerMaxHeap(MaxHeap* heap);
void heapifyUp(MaxHeap* heap, int indice);
void heapifyDown(MaxHeap* heap, int indice);
void redimensionarHeap(MaxHeap* heap);

void cargarDronesPrueba(ABB* abb);
void mostrarDron(const Dron* nodo);

// --- Función para mostrar drones ---
void mostrarDron(const Dron* nodo) {
    if (!nodo) return;
    int anio = nodo->fecha_mision / 10000;
    int mes = (nodo->fecha_mision / 100) % 100;
    int dia = nodo->fecha_mision % 100;

    printf("  Zona: %-15s Origen: %-15s ID: %-10s Batería: %3d%% Carga: %-12s Compañía: %-15s Fecha: %04d-%02d-%02d Hora: %04d\n",
           nodo->zona_entrega,
           nodo->zona_origen,
           nodo->id_dron,
           nodo->nivel_bateria,
           nodo->tipo_carga,
           nodo->compania,
           anio, mes, dia,
           nodo->hora_mision);
}

// ===============================================
// === MAIN ===
// ===============================================
int main() {
    ABB abb;
    MaxHeap* heap = crearMaxHeap(MAX_HEAP);

    if (!heap) {
        printf("Error fatal: No se pudo asignar memoria para el Max-Heap.\n");
        return 1;
    }

    inicializarABB(&abb);

    int opcion;
    do {
        printf("\n============================================\n");
        printf(" SISTEMA DE GESTIÓN DE DRONES DE REPARTO\n");
        printf("============================================\n");
        printf("1. Registrar nuevo dron\n");
        printf("2. Buscar dron por zona de entrega\n");
        printf("3. Eliminar dron\n");
        printf("4. Listar drones por tipo de carga o nivel mínimo de batería\n");
        printf("5. Programar misión (insertar en Max-Heap)\n");
        printf("6. Despachar próxima misión (extraer de Max-Heap)\n");
        printf("7. Mostrar todos los drones (inorden)\n");
        printf("8. Mostrar misiones programadas (Heap)\n");
        printf("9. Cargar drones de prueba del examen\n");
        printf("10. Salir\n");
        printf("Elige una opción: ");

        if (scanf("%d", &opcion) != 1) {
            while(getchar() != '\n');
            printf("Opción inválida. Intente de nuevo.\n");
            continue;
        }
        
        while(getchar() != '\n'); // Limpiar el buffer de entrada

        char buffer[MAX_STR_LEN];
        char id[MAX_ID_LEN];
        char zona[MAX_STR_LEN];
        int tipo_filtro;
        Dron nuevo_dron;
        int indice;
        ElementoHeap nueva_mision;

        switch(opcion) {
            case 1:
                printf("Ingrese ID del dron: "); scanf("%19s", nuevo_dron.id_dron);
                printf("Ingrese compañía operadora: "); scanf("%49s", nuevo_dron.compania);
                printf("Ingrese zona de origen: "); scanf("%49s", nuevo_dron.zona_origen);
                printf("Ingrese zona de entrega (CLAVE PRIMARIA): "); scanf("%49s", nuevo_dron.zona_entrega);
                printf("Ingrese nivel de batería (%%): "); scanf("%d", &nuevo_dron.nivel_bateria);
                printf("Ingrese fecha de misión (AAAAMMDD): "); scanf("%d", &nuevo_dron.fecha_mision);
                printf("Ingrese hora de misión (HHMM): "); scanf("%d", &nuevo_dron.hora_mision);
                printf("Ingrese tipo de carga: "); scanf("%29s", nuevo_dron.tipo_carga);

                nuevo_dron.izquierdo = POS_VACIA;
                nuevo_dron.derecho = POS_VACIA;

                if (!insertarDronABB(&abb, nuevo_dron)) {
                    printf("Error al registrar el dron.\n");
                }
                break;

            case 2:
                printf("Ingrese zona de entrega a buscar: "); scanf("%49s", buffer);
                buscarDronesPorZona(&abb, buffer);
                break;

            case 3:
                printf("Ingrese zona de entrega del dron: "); scanf("%49s", zona);
                printf("Ingrese ID del dron: "); scanf("%19s", id);
                eliminarDronABB(&abb, zona, id, heap);
                break;

            case 4:
                printf("Filtrar por:\n1. Tipo de carga\n2. Nivel mínimo de batería\nSeleccione opción: ");
                if (scanf("%d", &tipo_filtro) != 1) { while(getchar() != '\n'); printf("Opción inválida.\n"); break; }
                while(getchar() != '\n'); 

                if (tipo_filtro == 1) {
                    printf("Ingrese tipo de carga: "); scanf("%29s", buffer);
                    listarDronesFiltrados(&abb, buffer, 1);
                } else if (tipo_filtro == 2) {
                    printf("Ingrese nivel mínimo de batería: "); scanf("%49s", buffer);
                    listarDronesFiltrados(&abb, buffer, 2);
                } else {
                    printf("Opción inválida.\n");
                }
                break;

            case 5:
                printf("Ingrese zona de entrega del dron: "); scanf("%49s", zona);
                printf("Ingrese ID del dron: "); scanf("%19s", id);
                indice = buscarIndiceDron(&abb, zona, id);
                if (indice == POS_VACIA) {
                    printf("Error: Dron no encontrado.\n");
                } else {
                    nueva_mision.indice_dron = indice;
                    nueva_mision.prioridad = abb.elementos[indice].nivel_bateria; 
                    insertarHeap(heap, nueva_mision);
                    printf("Misión programada para dron %s con prioridad %d\n", id, nueva_mision.prioridad);
                }
                break;

            case 6: // Despachar (Extraer) la misión de mayor prioridad
                if (heap->tamano > 0) {
                    ElementoHeap proxima = extraerMaxHeap(heap);
                    if (proxima.indice_dron != POS_VACIA) {
                        Dron* dron = &abb.elementos[proxima.indice_dron];
                        printf("--- Misión Despachada (Extraída del Heap) ---\n");
                        mostrarDron(dron);
                        printf("    Prioridad de Despacho: %d\n", proxima.prioridad);
                    }
                } else {
                    printf("No hay misiones programadas.\n");
                }
                break;

            case 7:
                printf("--- Drones registrados (ordenados por zona de entrega) ---\n");
                recorrerInordenABB(&abb);
                break;

            case 8:
                printf("--- Misiones programadas en el Heap ---\n");
                if (heap->tamano == 0) {
                    printf("No hay misiones programadas.\n");
                } else {
                    for (int i = 0; i < heap->tamano; i++) {
                        Dron* dron = &abb.elementos[heap->elementos[i].indice_dron];
                        mostrarDron(dron);
                        printf("    Prioridad: %d\n", heap->elementos[i].prioridad);
                    }
                }
                break;

            case 9:
                cargarDronesPrueba(&abb);
                break;

            case 10:
                printf("Saliendo del programa...\n");
                if (heap->elementos) free(heap->elementos);
                if (heap) free(heap);
                return 0;

            default:
                printf("Opción inválida, por favor ingrese una opción válida.\n");
        }

    } while(1);

    return 0;
}

// ===============================================
// === ABB ===
// ===============================================
void inicializarABB(ABB* abb) {
    abb->raiz = POS_VACIA;
    abb->tamano = 0;
    abb->siguiente_libre = 0;
    for (int i = 0; i < MAX_NODOS; i++) {
        abb->elementos[i].izquierdo = POS_VACIA;
        abb->elementos[i].derecho = POS_VACIA;
    }
}

int compararDrones(const Dron* d1, const Dron* d2) {
    int cmp = strcmp(d1->zona_entrega, d2->zona_entrega);
    if (cmp != 0) return cmp;
    return strcmp(d1->id_dron, d2->id_dron);
}

int insertarRecursivoABB(ABB* abb, int indice_actual, int nuevo_indice) {
    if (indice_actual == POS_VACIA) return nuevo_indice;

    int cmp = compararDrones(&abb->elementos[nuevo_indice], &abb->elementos[indice_actual]);
    if (cmp < 0) abb->elementos[indice_actual].izquierdo = insertarRecursivoABB(abb, abb->elementos[indice_actual].izquierdo, nuevo_indice);
    else if (cmp > 0) abb->elementos[indice_actual].derecho = insertarRecursivoABB(abb, abb->elementos[indice_actual].derecho, nuevo_indice);
    else {
        printf("Error: Ya existe un dron con esta zona e ID\n");
        return indice_actual;
    }
    return indice_actual;
}

int insertarDronABB(ABB* abb, Dron nuevo_dron) {
    if (abb->siguiente_libre >= MAX_NODOS) {
        printf("Error: ABB lleno\n");
        return 0;
    }
    int nuevo_indice = abb->siguiente_libre;
    abb->elementos[nuevo_indice] = nuevo_dron; 
    
    int raiz_retornada = insertarRecursivoABB(abb, abb->raiz, nuevo_indice);
    
    // Si la inserción fue exitosa (no se encontró duplicado), incrementamos contadores
    if (raiz_retornada == nuevo_indice || (abb->raiz != POS_VACIA && raiz_retornada == abb->raiz) || (abb->raiz == POS_VACIA && raiz_retornada == nuevo_indice)) {
        
        // Comprobar si realmente se insertó (no fue un duplicado)
        // La forma más simple de verificar la inserción es si la raíz anterior era nula
        // o si el nodo se colgó correctamente en algún lado.
        
        // Si el índice de inserción original no se ha reusado (no es duplicado)
        if (nuevo_indice == abb->siguiente_libre) {
            abb->raiz = raiz_retornada;
            abb->siguiente_libre++;
            abb->tamano++;
            printf("Dron registrado exitosamente\n");
            return 1;
        } else {
             // Ya se ha mostrado el error en insertarRecursivoABB
             return 0;
        }
    }
    return 0;
}

void buscarPorZonaRecursivo(const ABB* abb, int indice, const char* zona) {
    if (indice == POS_VACIA) return;
    const Dron* dron = &abb->elementos[indice];
    int cmp = strcmp(zona, dron->zona_entrega);

    if (cmp <= 0) buscarPorZonaRecursivo(abb, dron->izquierdo, zona);
    
    if (cmp == 0) mostrarDron(dron);
    
    if (cmp >= 0) buscarPorZonaRecursivo(abb, dron->derecho, zona);
}

void buscarDronesPorZona(const ABB* abb, const char* zona) {
    if (abb->raiz == POS_VACIA) { printf("No hay drones registrados\n"); return; }
    printf("Drones en la zona '%s':\n", zona);
    buscarPorZonaRecursivo(abb, abb->raiz, zona);
}

int buscarIndiceDron(const ABB* abb, const char* zona, const char* id) {
    int indice = abb->raiz;
    while (indice != POS_VACIA) {
        const Dron* nodo = &abb->elementos[indice];
        int cmp_zona = strcmp(zona, nodo->zona_entrega);
        if (cmp_zona == 0) {
            int cmp_id = strcmp(id, nodo->id_dron);
            if (cmp_id == 0) return indice;
            else if (cmp_id < 0) indice = nodo->izquierdo;
            else indice = nodo->derecho;
        } else if (cmp_zona < 0) indice = nodo->izquierdo;
        else indice = nodo->derecho;
    }
    return POS_VACIA;
}

int dronEnHeap(const MaxHeap* heap, int indice_dron) {
    for (int i = 0; i < heap->tamano; i++)
        if (heap->elementos[i].indice_dron == indice_dron) return 1;
    return 0;
}

// --- Eliminación de drones ---
int buscarMinimo(const ABB* abb, int indice) {
    while (abb->elementos[indice].izquierdo != POS_VACIA)
        indice = abb->elementos[indice].izquierdo;
    return indice;
}

int eliminarRecursivoABB(ABB* abb, int indice_actual, const char* zona, const char* id, MaxHeap* heap, int* exito) {
    if (indice_actual == POS_VACIA) return POS_VACIA;
    Dron* nodo = &abb->elementos[indice_actual];
    
    int cmp_zona = strcmp(zona, nodo->zona_entrega);
    int cmp_id = strcmp(id, nodo->id_dron);

    if (cmp_zona < 0 || (cmp_zona == 0 && cmp_id < 0)) {
        nodo->izquierdo = eliminarRecursivoABB(abb, nodo->izquierdo, zona, id, heap, exito);
    } else if (cmp_zona > 0 || (cmp_zona == 0 && cmp_id > 0)) {
        nodo->derecho = eliminarRecursivoABB(abb, nodo->derecho, zona, id, heap, exito);
    } else { 
        if (dronEnHeap(heap, indice_actual)) {
            printf("Error: El dron tiene una misión programada. No se puede eliminar.\n");
            *exito = 0;
            return indice_actual;
        }
        *exito = 1; 

        if (nodo->izquierdo == POS_VACIA) return nodo->derecho;
        if (nodo->derecho == POS_VACIA) return nodo->izquierdo;

        int min_derecha = buscarMinimo(abb, nodo->derecho);
        
        // Copiar el contenido del sucesor al nodo actual
        abb->elementos[indice_actual] = abb->elementos[min_derecha]; 
        
        // Eliminar el sucesor del subárbol derecho
        nodo->derecho = eliminarRecursivoABB(abb, nodo->derecho, abb->elementos[min_derecha].zona_entrega, abb->elementos[min_derecha].id_dron, heap, exito);

    }
    return indice_actual;
}

int eliminarDronABB(ABB* abb, const char* zona, const char* id, MaxHeap* heap) {
    int exito = 0;
    abb->raiz = eliminarRecursivoABB(abb, abb->raiz, zona, id, heap, &exito);
    
    if (exito) {
        abb->tamano--;
        printf("Dron eliminado correctamente\n");
        return 1;
    } else {
        return 0;
    }
}

// --- Recorrido inorden ---
void recorrerInordenRecursivo(const ABB* abb, int indice) {
    if (indice == POS_VACIA) return;
    const Dron* nodo = &abb->elementos[indice];
    recorrerInordenRecursivo(abb, nodo->izquierdo);
    mostrarDron(nodo);
    recorrerInordenRecursivo(abb, nodo->derecho);
}

void recorrerInordenABB(const ABB* abb) {
    if (abb->raiz == POS_VACIA) { printf("ABB vacío\n"); return; }
    recorrerInordenRecursivo(abb, abb->raiz);
}

// --- Filtrado ---
void listarFiltradoRecursivo(const ABB* abb, int indice, const char* filtro, int tipo_filtro) {
    if (indice == POS_VACIA) return;
    const Dron* nodo = &abb->elementos[indice];
    
    listarFiltradoRecursivo(abb, nodo->izquierdo, filtro, tipo_filtro);

    int mostrar = 0;
    if (tipo_filtro == 1 && strcmp(nodo->tipo_carga, filtro) == 0) {
        mostrar = 1;
    } else if (tipo_filtro == 2) {
        int min_bateria = atoi(filtro); 
        if (nodo->nivel_bateria >= min_bateria) mostrar = 1;
    }

    if (mostrar) mostrarDron(nodo);

    listarFiltradoRecursivo(abb, nodo->derecho, filtro, tipo_filtro);
}

void listarDronesFiltrados(const ABB* abb, const char* filtro, int tipo_filtro) {
    if (abb->raiz == POS_VACIA) { printf("No hay drones registrados\n"); return; }
    if (tipo_filtro == 1) printf("Drones con carga tipo '%s':\n", filtro);
    else printf("Drones con batería mínima de %s%%:\n", filtro);
    listarFiltradoRecursivo(abb, abb->raiz, filtro, tipo_filtro);
}

// ===============================================
// === HEAP ===
// ===============================================
MaxHeap* crearMaxHeap(int capacidad) {
    MaxHeap* heap = (MaxHeap*)malloc(sizeof(MaxHeap));
    if (!heap) return NULL;
    heap->elementos = (ElementoHeap*)malloc(sizeof(ElementoHeap) * capacidad);
    if (!heap->elementos) { free(heap); return NULL; }
    heap->capacidad = capacidad;
    heap->tamano = 0;
    return heap;
}

void insertarHeap(MaxHeap* heap, ElementoHeap nuevo_elemento) {
    if (heap->tamano >= heap->capacidad) redimensionarHeap(heap);
    heap->elementos[heap->tamano] = nuevo_elemento;
    heapifyUp(heap, heap->tamano);
    heap->tamano++;
}

ElementoHeap consultarMaxHeap(const MaxHeap* heap) {
    if (heap->tamano == 0) return (ElementoHeap){.indice_dron = POS_VACIA, .prioridad = POS_VACIA};
    return heap->elementos[0];
}

ElementoHeap extraerMaxHeap(MaxHeap* heap) {
    if (heap->tamano == 0) {
        printf("Error: Heap vacío, no se puede extraer la misión.\n");
        return (ElementoHeap){.indice_dron = POS_VACIA, .prioridad = POS_VACIA};
    }

    ElementoHeap max_elemento = heap->elementos[0];

    heap->tamano--;
    if (heap->tamano > 0) {
        heap->elementos[0] = heap->elementos[heap->tamano];
        heapifyDown(heap, 0);
    }
    
    return max_elemento;
}

void heapifyUp(MaxHeap* heap, int indice) {
    while (indice > 0) {
        int padre = (indice - 1) / 2;
        if (heap->elementos[indice].prioridad > heap->elementos[padre].prioridad) {
            ElementoHeap temp = heap->elementos[indice];
            heap->elementos[indice] = heap->elementos[padre];
            heap->elementos[padre] = temp;
            indice = padre;
        } else break;
    }
}

void heapifyDown(MaxHeap* heap, int indice) {
    int izquierda, derecha, mayor;
    while (1) {
        izquierda = 2 * indice + 1;
        derecha = 2 * indice + 2;
        mayor = indice;

        if (izquierda < heap->tamano && heap->elementos[izquierda].prioridad > heap->elementos[mayor].prioridad)
            mayor = izquierda;
        if (derecha < heap->tamano && heap->elementos[derecha].prioridad > heap->elementos[mayor].prioridad)
            mayor = derecha;
        if (mayor == indice) break;
        ElementoHeap temp = heap->elementos[indice];
        heap->elementos[indice] = heap->elementos[mayor];
        heap->elementos[mayor] = temp;
        indice = mayor;
    }
}

void redimensionarHeap(MaxHeap* heap) {
    int nueva_capacidad = heap->capacidad * 2;
    ElementoHeap* nuevos = (ElementoHeap*)realloc(heap->elementos, sizeof(ElementoHeap) * nueva_capacidad);
    if (!nuevos) {
        printf("Error crítico: No se pudo redimensionar el heap\n");
        exit(1);
    }
    heap->elementos = nuevos;
    heap->capacidad = nueva_capacidad;
    printf("Heap redimensionado a capacidad %d\n", nueva_capacidad);
}

// ===============================================
// === FUNCIONES DE PRUEBA CORREGIDAS ===
// ===============================================
void cargarDronesPrueba(ABB* abb) {
    // Valores numéricos sin '0' inicial para evitar interpretación octal
    Dron d1 = {"D1", "Com1", "ZonaA", "A1", 90, 20251216, 1400, "Paquete", POS_VACIA, POS_VACIA};
    Dron d2 = {"D2", "Com2", "ZonaB", "B1", 50, 20251216, 1500, "Carta", POS_VACIA, POS_VACIA};
    Dron d3 = {"D3", "Com1", "ZonaC", "C1", 75, 20251217, 1000, "Paquete", POS_VACIA, POS_VACIA};
    
    insertarDronABB(abb, d2); 
    insertarDronABB(abb, d1); 
    insertarDronABB(abb, d3);
    printf("Drones de prueba cargados correctamente con valores decimales.\n");
}
