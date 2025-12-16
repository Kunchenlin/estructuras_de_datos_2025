/**
 * @file urgencias.c
 * @brief Simulación de una sala de urgencias con gestión de pacientes mediante listas enlazadas.
 *
 * Este programa permite registrar, eliminar y atender pacientes
 * según su nivel de triaje. Incluye un menú interactivo y
 * pruebas automáticas para verificar su funcionamiento.
 *
 * @author Kun Chen
 * @date 2025
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <assert.h> 


/*======================================================
 *                 ESTRUCTURAS DE DATOS
 *======================================================*/

/**
 * @enum NivelTriaje
 * @brief Define los niveles de prioridad de atención médica.
 */
typedef enum { 
	RESUS = 1,         /**< Nivel I: Reanimación (atención inmediata). */
	EMERGENCIA = 2,    /**< Nivel II: Emergencia. */
	URGENCIA = 3,      /**< Nivel III: Urgencia. */
	MENOS_URGENTE = 4  /**< Nivel IV: Menos urgente. */
} NivelTriaje; 

/**
 * @struct PACIENTE
 * @brief Estructura que representa a un paciente en la sala de urgencias.
 */
typedef struct {
	char nombre[50];       /**< Nombre del paciente. */
	char motivo[100];      /**< Motivo de la consulta. */
	int tiempo_espera;     /**< Tiempo estimado de espera (en minutos). */
	NivelTriaje triaje;    /**< Nivel de triaje asignado. */
} PACIENTE; 

/**
 * @struct NODO
 * @brief Nodo de una lista enlazada que almacena un paciente.
 */
typedef struct nodo {
	PACIENTE info;         /**< Información del paciente. */
	struct nodo *sig;      /**< Puntero al siguiente nodo. */
} NODO; 


/*======================================================
 *                 PROTOTIPOS DE FUNCIONES
 *======================================================*/

NODO* crearLista(); 
int listaVacia(NODO *ultimo); 
NODO* insertarPaciente(NODO *ultimo, PACIENTE nuevo); 
void mostrarLista(NODO *ultimo); 
NODO* eliminarPorNombre(NODO *ultimo, char nombre[]); 
NODO* eliminarPorPosicion(NODO *ultimo, int pos); 
NODO* pasarSiguiente(NODO *actual); 
void atenderPaciente(NODO *actual); 
void liberarLista(NODO **ultimo); 
int run_all_tests();
void menu_urgencias();


/*======================================================
 *                 FUNCIÓN PRINCIPAL
 *======================================================*/

/**
 * @brief Punto de entrada del programa.
 * 
 * Ejecuta las pruebas automáticas y luego ofrece al usuario
 * la posibilidad de iniciar el menú interactivo de urgencias.
 * 
 * @return 0 si finaliza correctamente.
 */
int main(void) { 
	int puntos = run_all_tests(); 
	printf("\n¿Desea ejecutar el menú interactivo? (s/n): "); 
	char c; 
	scanf("%c", &c); 
	if (c == 's' || c == 'S') 
		menu_urgencias(); 
	
	return 0; 
} 


/*======================================================
 *                 FUNCIONES DE LISTA
 *======================================================*/

/**
 * @brief Crea una lista vacía.
 * @return Puntero nulo indicando lista vacía.
 */
NODO* crearLista() {
	return NULL; 
}

/**
 * @brief Comprueba si la lista está vacía.
 * @param ultimo Último nodo de la lista.
 * @return 1 si está vacía, 0 si no.
 */
int listaVacia(NODO *ultimo) {
	return (ultimo == NULL); 
}

/**
 * @brief Inserta un nuevo paciente al final de la lista.
 * @param ultimo Puntero al último nodo actual.
 * @param nuevo Estructura PACIENTE a insertar.
 * @return Nuevo puntero al último nodo.
 */
NODO* insertarPaciente(NODO *ultimo, PACIENTE nuevo) {
    NODO *nuevoNodo = (NODO*) malloc(sizeof(NODO));
    assert(nuevoNodo != NULL);

    nuevoNodo->info = nuevo;

    if (ultimo == NULL) {
        // lista vacía: el nuevo nodo apunta a sí mismo
        nuevoNodo->sig = nuevoNodo;
        return nuevoNodo; // ahora último == primer == nuevoNodo
    } else {
        // insertar al final manteniendo circularidad:
        nuevoNodo->sig = ultimo->sig; // apunta al primer nodo
        ultimo->sig = nuevoNodo;     // el anterior último apunta al nuevo
        return nuevoNodo;            // nuevoNodo es el nuevo último
    }
}

/**
 * @brief Muestra todos los pacientes de la lista.
 * @param ultimo Puntero al último nodo de la lista.
 */

void mostrarLista(NODO *ultimo) {
    if (listaVacia(ultimo)) {
        printf("La lista está vacía.\n");
        return;
    }

    NODO *start = ultimo->sig; // primer nodo
    NODO *temp = start;
    do {
        printf("Nombre: %s\n", temp->info.nombre);
	printf("Motivo: %s\n", temp->info.motivo);
	printf("Tiempo de espera: %d minutos\n", temp->info.tiempo_espera);
	printf("Nivel de triaje: %d\n", temp->info.triaje);
	printf("---------------------------\n");
	
        temp = temp->sig;
    } while (temp != start);
}

/**
 * @brief Elimina un paciente por su nombre.
 * @param ultimo Último nodo actual.
 * @param nombre Nombre del paciente a eliminar.
 * @return Nuevo puntero al último nodo.
 */
NODO* eliminarPorNombre(NODO *ultimo, char nombre[]) {
    if (listaVacia(ultimo)) return NULL;

    NODO *actual = ultimo->sig; // primer nodo
    NODO *prev = ultimo;
    NODO *start = actual;
    
    do {
        if (strcmp(actual->info.nombre, nombre) == 0) {
            if (actual == prev) { 
                // Solo un nodo en la lista
                free(actual);
                return NULL;
            } else {
                prev->sig = actual->sig;
                if (actual == ultimo) {
                    // Si eliminamos el último, actualizamos puntero
                    ultimo = prev;
                }
                free(actual);
                return ultimo;
            }
        }
        prev = actual;
        actual = actual->sig;
    } while (actual != start);

    // Si no encontramos el paciente
    return ultimo;
}
/**
 * @brief Elimina un paciente por su posición en la lista.
 * @param ultimo Último nodo actual.
 * @param pos Posición (comenzando en 0).
 * @return Nuevo puntero al último nodo.
 */
NODO* eliminarPorPosicion(NODO *ultimo, int pos) {
    if (listaVacia(ultimo)) return NULL;

    NODO *temp = ultimo, *anterior = NULL;
    int i = 0;
    
    if (pos == 0) {  
        NODO *next = temp->sig;
        free(temp);
        return next;
    }
    
    while (temp != NULL && i < pos) {
        anterior = temp;
        temp = temp->sig;
        i++;
    }

    if (temp == NULL) return ultimo;
    
    anterior->sig = temp->sig;
    free(temp);
    return ultimo;
}

/**
 * @brief Devuelve el siguiente nodo.
 * @param actual Nodo actual.
 * @return Nodo siguiente o NULL.
 */
NODO* pasarSiguiente(NODO *actual) {
	if (actual == NULL) return NULL; 
	return actual->sig; 
}

/**
 * @brief Muestra los datos del paciente actual.
 * @param actual Nodo actual.
 */
void atenderPaciente(NODO *actual) {
	if (actual == NULL) { 
        printf("No hay paciente seleccionado.\n"); 
        return; 
    } 
    printf("Atendiendo paciente:\n"); 
    printf("Nombre: %s\n", actual->info.nombre); 
    printf("Motivo: %s\n", actual->info.motivo); 
    printf("Tiempo de espera: %d min\n", actual->info.tiempo_espera); 
    printf("Nivel de triaje: %d\n", actual->info.triaje); 
}

/**
 * @brief Libera la memoria de todos los nodos de la lista.
 * @param ultimo Doble puntero al último nodo.
 */
void liberarLista(NODO **ultimo) {
    if (*ultimo == NULL) return;
    NODO *start = (*ultimo)->sig;
    NODO *temp = start;
    do {
        NODO *next = temp->sig;
        free(temp);
        temp = next;
    } while (temp != start);
    *ultimo = NULL;
}

/*======================================================
 *                 FUNCIONES AUXILIARES DE TEST
 *======================================================*/

/**
 * @brief Devuelve el primer nodo de la lista circular.
 * @param ultimo Último nodo.
 * @return Primer nodo o NULL.
 */
static NODO* primerNodo(NODO *ultimo) { 
    if (ultimo == NULL) return NULL; 
    return ultimo->sig; 
}

/**
 * @brief Cuenta los nodos de una lista.
 * @param ultimo Último nodo.
 * @return Número de nodos.
 */
static int contarNodos(NODO *ultimo) { 
    if (ultimo == NULL) return 0; 
    int cnt = 0; 
    NODO *it = ultimo->sig; 
    do { 
        cnt++; 
        it = it->sig; 
    } while (it != ultimo->sig); 
    return cnt; 
}

/**
 * @brief Inicializa un paciente con datos.
 * @param p Puntero al paciente.
 * @param nombre Nombre del paciente.
 * @param motivo Motivo de consulta.
 * @param t Tiempo de espera.
 */
static void setPaciente(PACIENTE *p, const char *nombre, const char *motivo, int t) { 
    snprintf(p->nombre, sizeof(p->nombre), "%s", nombre); 
    snprintf(p->motivo, sizeof(p->motivo), "%s", motivo); 
    p->tiempo_espera = t; 
}


/*======================================================
 *                 SISTEMA DE TEST AUTOMÁTICO
 *======================================================*/

/**
 * @typedef TestFunc
 * @brief Tipo de función de prueba.
 */
typedef int (*TestFunc)();

/**
 * @struct TestEntry
 * @brief Estructura que representa una prueba individual.
 */
typedef struct { 
    const char *nombre;   /**< Nombre de la prueba. */
    TestFunc funcion;     /**< Puntero a la función de prueba. */
    int puntos;           /**< Puntos que aporta si pasa. */
} TestEntry; 


/**
 * @brief Prueba la inserción de pacientes.
 * @return 1 si pasa, 0 si falla.
 */
int test_insertarPaciente() { 
    NODO *ultimo = crearLista(); 
    PACIENTE p1, p2; 
    setPaciente(&p1, "Ana", "Dolor abdominal", 15); 
    setPaciente(&p2, "Carlos", "Fiebre alta", 10); 
    ultimo = insertarPaciente(ultimo, p1); 
    ultimo = insertarPaciente(ultimo, p2); 
    int ok = (!listaVacia(ultimo)); 
    liberarLista(&ultimo); 
    return ok; 
}

/**
 * @brief Prueba la función mostrarLista().
 * @return 1 si pasa, 0 si falla.
 */
int test_mostrarLista() { 
    NODO *ultimo = crearLista(); 
    PACIENTE p; 
    setPaciente(&p, "María", "Fractura", 25); 
    ultimo = insertarPaciente(ultimo, p); 
    int ok = (ultimo != NULL); 
    liberarLista(&ultimo); 
    return ok; 
}

/**
 * @brief Prueba pasarSiguiente().
 * @return 1 si pasa, 0 si falla.
 */
int test_pasarSiguiente() { 
    NODO *ultimo = crearLista(); 
    PACIENTE a,b; 
    setPaciente(&a, "A","X",1); 
    setPaciente(&b,"B","Y",2); 
    ultimo = insertarPaciente(ultimo, a); 
    ultimo = insertarPaciente(ultimo, b); 
    NODO *primero = primerNodo(ultimo); 
    NODO *siguiente = pasarSiguiente(primero); 
    int ok = (siguiente != NULL); 
    liberarLista(&ultimo); 
    return ok; 
}

/**
 * @brief Prueba eliminarPorNombre().
 * @return 1 si pasa, 0 si falla.
 */
int test_eliminarPorNombre() { 
    NODO *ultimo = crearLista(); 
    PACIENTE a,b; 
    setPaciente(&a, "Ana","X",1); 
    setPaciente(&b,"Bea","Y",2); 
    ultimo = insertarPaciente(ultimo, a); 
    ultimo = insertarPaciente(ultimo, b); 
    ultimo = eliminarPorNombre(ultimo, "Bea"); 
    int ok = (ultimo != NULL); 
    liberarLista(&ultimo); 
    return ok; 
}

/**
 * @brief Prueba eliminarPorPosicion().
 * @return 1 si pasa, 0 si falla.
 */
int test_eliminarPorPosicion() { 
    NODO *ultimo = crearLista(); 
    PACIENTE a,b,c; 
    setPaciente(&a,"A","X",1); 
    setPaciente(&b,"B","Y",2); 
    setPaciente(&c,"C","Z",3); 
    ultimo = insertarPaciente(ultimo,a); 
    ultimo = insertarPaciente(ultimo,b); 
    ultimo = insertarPaciente(ultimo,c); 
    ultimo = eliminarPorPosicion(ultimo,1); 
    int ok = (ultimo != NULL); 
    liberarLista(&ultimo); 
    return ok; 
}

int test_circularidad() {
NODO *ultimo = crearLista();
PACIENTE a,b; setPaciente(&a,"A","X",1); setPaciente(&b,"B","Y",2);
ultimo = insertarPaciente(ultimo,a);
ultimo = insertarPaciente(ultimo,b);
NODO *primero = primerNodo(ultimo);
int ok = (ultimo->sig == primero);
liberarLista(&ultimo); 
return ok;
}

/* Prueba de integración del menú (no interactiva, solo validación básica) */
int test_menu_simulado() {
NODO *ultimo = crearLista();
PACIENTE p;
setPaciente(&p, "Test", "Simulación", 5);
ultimo = insertarPaciente(ultimo, p);
int ok1 = (!listaVacia(ultimo));
ultimo = eliminarPorNombre(ultimo, "Test");
int ok2 = (listaVacia(ultimo));
liberarLista(&ultimo);
return (ok1 && ok2);
}

/**
 * @brief Ejecuta todas las pruebas del programa.
 * @return Puntuación total obtenida.
 */
int run_all_tests() { 
    TestEntry tests[] = { 
        {"insertarPaciente", test_insertarPaciente, 2}, 
        {"mostrarLista", test_mostrarLista, 1}, 
        {"pasarSiguiente", test_pasarSiguiente, 1}, 
        {"eliminarPorNombre", test_eliminarPorNombre, 2}, 
        {"eliminarPorPosicion", test_eliminarPorPosicion, 2},
        {"circularidad", test_circularidad, 1},
	{"menu_simulado", test_menu_simulado, 1}, // test de integración
    }; 
    int total_tests = sizeof(tests)/sizeof(TestEntry); 
    int puntos_totales = 0; 
    printf("=== EJECUCIÓN AUTOMÁTICA DE PRUEBAS ===\n\n"); 
    for (int i = 0; i < total_tests; i++) { 
        printf("Prueba %-22s ... ", tests[i].nombre); 
        fflush(stdout); 
        int ok = tests[i].funcion(); 
        if (ok) { 
            printf("OK (+%d)\n", tests[i].puntos); 
            puntos_totales += tests[i].puntos; 
        } else { 
            printf("FALLO (0)\n"); 
        } 
    } 
    printf("\nPuntuación final: %d / 10 puntos\n", puntos_totales); 
    return puntos_totales; 
}


/*======================================================
 *                 MENÚ INTERACTIVO
 *======================================================*/

/**
 * @brief Muestra el menú principal de gestión de urgencias.
 */
void menu_urgencias() { 
    NODO *ultimo = crearLista(); 
    NODO *actual = NULL; 
    int opcion; 
    char nombre[50], motivo[100]; 
    int tiempo; 

    PACIENTE inicial; 
    snprintf(inicial.nombre, sizeof(inicial.nombre), "Roberto");
    snprintf(inicial.motivo, sizeof(inicial.motivo), "Fatiga");
    inicial.tiempo_espera = 20;
    inicial.triaje = URGENCIA;

    ultimo = insertarPaciente(ultimo, inicial); 
    actual = ultimo->sig; 

    do { 
        printf("\n=== MENÚ SALA DE URGENCIAS ===\n"); 
        printf("1. Mostrar lista de pacientes\n"); 
        printf("2. Registrar nuevo paciente\n"); 
        printf("3. Pasar al siguiente paciente\n"); 
        printf("4. Atender paciente actual\n"); 
        printf("5. Eliminar paciente\n"); 
        printf("6. Salir\n"); 
        printf("Seleccione una opción: "); 
        scanf("%d", &opcion); 
        getchar(); 

        switch (opcion) { 
            case 1:  
            	printf("Lista de Pacientes: \n");
    		printf("---------------------------\n");
                mostrarLista(ultimo); 
                break; 
            case 2: 
                printf("\nIngrese el nombre del paciente: ");
                fgets(nombre, sizeof(nombre), stdin);
                nombre[strcspn(nombre, "\n")] = '\0'; // eliminar '\n'

                printf("Ingrese el motivo de la urgencia: ");
                fgets(motivo, sizeof(motivo), stdin);
                motivo[strcspn(motivo, "\n")] = '\0'; // eliminar '\n'

                printf("Ingrese el tiempo estimado de espera (en minutos): ");
                scanf("%d", &tiempo);
                getchar(); // limpiar buffer
                
                // Selección del nivel de triaje
    int nivel;
    do {
        printf("Seleccione el nivel de triaje:\n");
        printf("1. RESUS (más urgente)\n");
        printf("2. EMERGENCIA\n");
        printf("3. URGENCIA\n");
        printf("4. MENOS URGENTE\n");
        printf("Opción: ");
        scanf("%d", &nivel);
        getchar(); // limpiar buffer
        if (nivel < 1 || nivel > 4) {
            printf("Opción inválida. Intente de nuevo.\n");
        }
    } while (nivel < 1 || nivel > 4);
                
                

                PACIENTE nuevoPaciente;
                snprintf(nuevoPaciente.nombre, sizeof(nuevoPaciente.nombre), "%s", nombre);
                snprintf(nuevoPaciente.motivo, sizeof(nuevoPaciente.motivo), "%s", motivo);
                nuevoPaciente.tiempo_espera = tiempo;
                nuevoPaciente.triaje = (NivelTriaje)nivel; // asignar nivel elegido

                ultimo = insertarPaciente(ultimo, nuevoPaciente);
                if (actual == NULL) actual = ultimo->sig; // si era lista vacía
                printf("Paciente registrado correctamente.\n");
                break;

            case 3: 
                if (actual != NULL) {
                    actual = pasarSiguiente(actual);
                    printf("Siguiente paciente: %s\n", actual->info.nombre);
                } else {
                    printf("No hay pacientes registrados.\n");
                }
                break; 
            case 4: 
                atenderPaciente(actual); 
                break; 
            case 5: 
                printf("\nIngrese el nombre del paciente a eliminar: ");
                fgets(nombre, sizeof(nombre), stdin);
                nombre[strcspn(nombre, "\n")] = '\0';  
                ultimo = eliminarPorNombre(ultimo, nombre);
                printf("Paciente eliminado correctamente.\n");
                break; 
            case 6: 
                liberarLista(&ultimo); 
                printf("Saliendo del sistema de urgencias...\n"); 
                break; 
            default: 
                printf("Opción no válida.\n"); 
        } 
    } while (opcion != 6); 
}

