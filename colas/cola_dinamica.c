/**
 * @file cola_dinamica.c
 * @brief Implementación de una cola dinámica para gestionar procesos de CPU.
 *
 * El programa permite insertar procesos, eliminarlos en orden FIFO,
 * consultar el proceso siguiente y mostrar el estado completo de la cola.
 * @author Kun Chen
 * @date 05-Nov-2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==========================================================
// ========== DEFINICIÓN DE ESTRUCTURAS DE DATOS =========
// ==========================================================

/**
 * @struct PROCESO
 * @brief Representa un proceso en el sistema.
 *
 * Contiene información básica como identificador único,
 * nombre y tiempo estimado de ejecución.
 */
typedef struct {
    int pid;               /**< Identificador único del proceso */
    char nombre[30];       /**< Nombre del proceso */
    int tiempo_ejecucion;  /**< Tiempo estimado de CPU que necesita */
} PROCESO;

/**
 * @struct NODO
 * @brief Nodo de la lista enlazada que forma la cola.
 *
 * Cada nodo almacena un proceso y un puntero al siguiente nodo.
 */
typedef struct nodo {
    PROCESO dato;         /**< Proceso almacenado */
    struct nodo *sig;     /**< Puntero al siguiente nodo */
} NODO;

/**
 * @struct COLA
 * @brief Representa una cola dinámica de procesos.
 *
 * Utiliza punteros al inicio (frente) y al final (último elemento).
 */
typedef struct {
    NODO *inicio; /**< Primer nodo de la cola */
    NODO *fin;    /**< Último nodo de la cola */
} COLA;

// ==========================================================
// ========== CABECERAS DE LAS FUNCIONES ====================
// ==========================================================

/**
 * @brief Inicializa una cola dinámica dejándola vacía.
 * @param c Puntero a la cola a inicializar.
 */
void crearCola(COLA *c);

/**
 * @brief Indica si la cola está vacía.
 * @param c Cola a comprobar.
 * @return 1 si la cola está vacía, 0 en caso contrario.
 */
int colaVacia(COLA c);

/**
 * @brief Inserta un nuevo proceso al final de la cola.
 * @param c Puntero a la cola.
 * @param nuevo Proceso a insertar.
 */
void encolar(COLA *c, PROCESO nuevo);

/**
 * @brief Elimina el proceso al frente de la cola.
 * @param c Puntero a la cola.
 * @param atendido Puntero donde se almacenará el proceso eliminado.
 */
void desencolar(COLA *c, PROCESO *atendido);

/**
 * @brief Consulta el primer proceso de la cola sin eliminarlo.
 * @param c Cola de la cual obtener el proceso.
 * @return El proceso en el frente o un proceso con PID=-1 si está vacía.
 */
PROCESO primero(COLA c);

/**
 * @brief Muestra todos los procesos de la cola en orden FIFO.
 * @param c Cola a mostrar.
 */
void mostrarCola(COLA c);

/**
 * @brief Libera toda la memoria dinámica asociada a la cola.
 * @param c Puntero a la cola.
 */
void liberarCola(COLA *c);

// ==========================================================
// ========== FUNCIÓN PRINCIPAL DEL PROGRAMA ================
// ==========================================================

/**
 * @brief Función principal del programa.
 *
 * Presenta un menú interactivo que permite gestionar una cola de procesos:
 * - Mostrar cola<br>
 * - Insertar proceso<br>
 * - Desencolar proceso<br>
 * - Consultar el primero<br>
 * - Vaciar cola<br>
 *
 * @return 0 si el programa termina correctamente.
 */
int main() {
    COLA cola;
    crearCola(&cola);
    int opcion;
    PROCESO p;
    static int contadorPID = 1;
    
    do {
        printf("\n=== COLA DE PROCESOS (DINÁMICA) ===\n");
        printf("1. Mostrar cola\n");
        printf("2. Insertar nuevo proceso\n");
        printf("3. Ejecutar proceso (Desencolar)\n");
        printf("4. Consultar próximo proceso\n");
        printf("5. Vaciar cola\n");
        printf("6. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                mostrarCola(cola);
                break;

            case 2:
                p.pid = contadorPID++;
                printf("Nombre del proceso: ");
                scanf("%s", p.nombre);
                printf("Tiempo de ejecución (s): ");
                scanf("%d", &p.tiempo_ejecucion);
                encolar(&cola, p);
                break;

            case 3:
                if (!colaVacia(cola)) {
                    desencolar(&cola, &p);
                    printf("Ejecutando proceso PID=%d (%s) [%d s]\n",
                           p.pid, p.nombre, p.tiempo_ejecucion);
                } else {
                    printf("Cola vacía.\n");
                }
                break;

            case 4:
                if (!colaVacia(cola)) {
                    p = primero(cola);
                    printf("Próximo proceso: PID=%d (%s)\n", p.pid, p.nombre);
                } else {
                    printf("Cola vacía.\n");
                }
                break;

            case 5:
                liberarCola(&cola);
                printf("Cola vaciada correctamente.\n");
                break;

            case 6:
                liberarCola(&cola);
                printf("Fin del programa.\n");
                break;

            default:
                printf("Opción no válida.\n");
        }
    } while (opcion != 6);

    return 0;
}

// ==========================================================
// ========== IMPLEMENTACIÓN DE FUNCIONES ===================
// ==========================================================

void crearCola(COLA *c) {
    c->inicio = NULL;
    c->fin = NULL;
}

int colaVacia(COLA c) {
    return c.inicio == NULL;
}

void encolar(COLA *c, PROCESO nuevo) {
    NODO *nuevoNodo = (NODO *)malloc(sizeof(NODO));
    nuevoNodo->dato = nuevo;
    nuevoNodo->sig = NULL;

    if (c->fin == NULL) {
        c->inicio = nuevoNodo;
    } else {
        c->fin->sig = nuevoNodo;
    }

    c->fin = nuevoNodo;
}

void desencolar(COLA *c, PROCESO *atendido) {
    if (colaVacia(*c)) {
        fprintf(stderr, "ERROR: Cola vacía.\n");
        return;
    }

    NODO *aux = c->inicio;
    *atendido = aux->dato;
    c->inicio = aux->sig;

    if (c->inicio == NULL) {
        c->fin = NULL;
    }

    free(aux);
}

PROCESO primero(COLA c) {
    if (colaVacia(c)) {
        fprintf(stderr, "ERROR: Cola vacía.\n");
        PROCESO p = {-1, "NULL", 0};
        return p;
    }

    return c.inicio->dato;
}

void mostrarCola(COLA c) {
    if (colaVacia(c)) {
        printf("Cola vacía.\n");
        return;
    }
    
    NODO *aux = c.inicio;
    while (aux != NULL) {
        printf("PID: %d | Nombre: %s | Tiempo: %d segundos\n",
               aux->dato.pid, aux->dato.nombre, aux->dato.tiempo_ejecucion);
        aux = aux->sig;
    }
}

void liberarCola(COLA *c) {
    while (c->inicio != NULL) {
        NODO *aux = c->inicio;
        c->inicio = c->inicio->sig;
        free(aux);
    }
    c->fin = NULL;
}

