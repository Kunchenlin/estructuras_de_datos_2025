/**==================================================================
 * @file cola_vectorial.c
 * @brief Simulación de una cola circular para gestionar procesos.
 *
 * Este programa implementa una cola circular para almacenar procesos con
 * un identificador único (PID), nombre y tiempo de ejecución.
 * 
 * Funciones principales:
 * - Mostrar la cola de procesos.
 * - Insertar nuevos procesos en la cola.
 * - Ejecutar (desencolar) procesos.
 * - Consultar el próximo proceso sin eliminarlo.
 * 
 * La cola tiene un tamaño máximo definido por la constante MAX, y maneja
 * los casos de cola vacía y llena.
 *
 * @author Kun Chen
 * @date 05-Nov-2025
 ==================================================================*/



#include <stdio.h>
#include <string.h>

#define MAX 10 // Tamaño máximo de la cola (número máximo de procesos)

//==================================================================
// ESTRUCTURA DE DATOS
//==================================================================

// ======= Estructura del proceso =======
// Representa la información básica de cada proceso
typedef struct {
    int pid; // Identificador único del proceso (autonumérico)
    char nombre[30]; // Nombre del proceso (texto sin espacios)
    int tiempo_ejecucion; // Tiempo de CPU que necesita el proceso (en segundos)
} PROCESO;

// ======= Estructura de la cola circular =======
// La cola almacena procesos en un vector de tamaño fijo.
// Se controla mediante tres variables:
// - front: índice del primer elemento (salida de la cola)
// - rear: índice de la siguiente posición libre (entrada de la cola)
// - num_elem: número actual de elementos en la cola
typedef struct {
    PROCESO datos[MAX]; // Array que contiene los procesos
    int front; // Índice del primer elemento (frente)
    int rear; // Índice de la siguiente posición libre (final)
    int num_elem; // Número de elementos actualmente en la cola
} COLA;

//==================================================================
// PROTOTIPO DE FUNCIONES
//==================================================================
void crearCola(COLA *c);
int colaVacia(COLA c);
int colaLlena(COLA c);
void encolar(COLA *c, PROCESO nuevo);
void desencolar(COLA *c, PROCESO *atendido);
PROCESO primero(COLA c);
void mostrarCola(COLA c);

//==================================================================
// PROGRAMA PRINCIPAL
//==================================================================
int main() {
    COLA cola; // Declaramos una cola de procesos
    crearCola(&cola); // Inicializamos la cola vacía
    int opcion; // Variable para el menú
    PROCESO p; // Variable auxiliar para leer o mostrar procesos
    static int contadorPID = 1; // Contador para asignar PID automáticamente
    
    do {
        // Menú principal con las operaciones disponibles
        printf("\n=== COLA DE PROCESOS (VECTORIAL CIRCULAR) ===\n");
        printf("1. Mostrar cola\n");
        printf("2. Insertar nuevo proceso\n");
        printf("3. Ejecutar proceso (Desencolar)\n");
        printf("4. Consultar próximo proceso\n");
        printf("5. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        // Selección según la opción introducida
        switch(opcion) {
            // ======= Opción 1: Mostrar la cola =======
            case 1:
                mostrarCola(cola);
                break;

            // ======= Opción 2: Insertar un nuevo proceso =======
            case 2:
                if (!colaLlena(cola)) {
                    // Se crea un nuevo proceso con datos introducidos por el usuario
                    p.pid = contadorPID++; // Asignación automática del PID
                    printf("Nombre del proceso: ");
                    scanf("%s", p.nombre);
                    printf("Tiempo de ejecución (s): ");
                    scanf("%d", &p.tiempo_ejecucion);
                    // Insertamos el proceso al final de la cola
                    encolar(&cola, p);
                } else {
                    // No hay espacio disponible en la cola
                    printf("Error: cola llena.\n");
                }
                break;

            // ======= Opción 3: Ejecutar proceso (eliminar del frente) =======
            case 3:
                if (!colaVacia(cola)) {
                    // Se desencola el primer proceso y se guarda en 'p'
                    desencolar(&cola, &p);
                    printf("Ejecutando proceso PID=%d (%s) [%d s]\n",
                        p.pid, p.nombre, p.tiempo_ejecucion);
                } else {
                    printf("Cola vacía.\n");
                }
                break;

            // ======= Opción 4: Consultar el próximo proceso (sin eliminarlo) =======
            case 4:
                if (!colaVacia(cola)) {
                    // Se obtiene el proceso del frente sin eliminarlo
                    p = primero(cola);
                    printf("Próximo proceso: PID=%d (%s)\n", p.pid, p.nombre);
                } else {
                    printf("Cola vacía.\n");
                }
                break;

            // ======= Opción 5: Salir del programa =======
            case 5:
                printf("Fin del programa.\n");
                break;

            // ======= Opción incorrecta =======
            default: 
                printf("Opción no válida.\n");
        }
    } while(opcion != 5); // Repite hasta que el usuario elija salir

    return 0;
}

//==================================================================
// IMPLEMENTACIÓN DE FUNCIONES
//==================================================================

void crearCola(COLA *c) {
    // Inicializa los índices y el contador para representar una cola vacía
    // front y rear comienzan en 0, y num_elem en 0
    c->front = 0;
    c->rear = 0;
    c->num_elem = 0;
}

int colaVacia(COLA c) {
    // Devuelve 1 (true) si la cola está vacía, es decir, num_elem == 0
    // Devuelve 0 (false) en caso contrario
    if (c.front == c.rear) {
    	return 1; 
    } else { 
    	return 0; 
    } 
}

int colaLlena(COLA c) {
    // Devuelve 1 (true) si la cola está llena, es decir, num_elem == MAX
    // Devuelve 0 (false) en caso contrario
    if (((c.rear + 1 ) % MAX) == c.front) {
    	return 1; 
    } else { 
    	return 0; 
    } 
}

void encolar(COLA *c, PROCESO nuevo) {
    // Inserta un nuevo proceso en la cola circular
    // 1. Comprobar que no esté llena
    // 2. Insertar el nuevo proceso en la posición 'rear'
    // 3. Actualizar rear = (rear + 1) % MAX
    // 4. Incrementar num_elem
    if (colaLlena(*c)) {
    	fprintf(stderr, "ERROR: Cola llena\n"); 
    } else { 
        c->datos[c->rear] = nuevo;
        c->rear = (c->rear + 1) % MAX;
        c->num_elem++;
    }
}

void desencolar(COLA *c, PROCESO *atendido) {
    // Elimina el proceso situado en 'front' (el primero en entrar)
    // 1. Comprobar que no esté vacía
    // 2. Copiar el elemento de 'front' en *atendido
    // 3. Actualizar front = (front + 1) % MAX
    // 4. Decrementar num_elem
    if (colaVacia(*c)) {
    	fprintf(stderr, "ERROR: Cola vacia\n"); 
    } else {  
        *atendido = c->datos[c->front];
        c->front = (c->front + 1) % MAX;
        c->num_elem--;
    }
}

PROCESO primero(COLA c) {
    // Devuelve el proceso del frente sin eliminarlo
    if (colaVacia(c)) {
        fprintf(stderr, "ERROR: Cola vacía\n");
        // Devolver un proceso con PID=-1 como valor de error
        PROCESO p = {-1, "NULL", 0};
        return p;
    }
    return c.datos[c.front];
}

void mostrarCola(COLA c) {
    // Recorre la cola circular mostrando los procesos en orden FIFO
    // Utilizar un índice auxiliar para avanzar (front + i) % MAX
    // Mostrar PID, nombre y tiempo de cada proceso
    if (colaVacia(c)) {
        printf("Cola vacía.\n");
        return;
    }

    printf("Procesos en cola:\n");
    for (int i = 0; i < c.num_elem; i++) {
        int idx = (c.front + i) % MAX;
        printf("PID: %d | Nombre: %s | Tiempo: %d segundos\n",
            c.datos[idx].pid, c.datos[idx].nombre, c.datos[idx].tiempo_ejecucion);
    }
}

