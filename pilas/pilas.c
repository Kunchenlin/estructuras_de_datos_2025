/**
 * @file pilas.c
 * @brief Programa que implementa y compara dos versiones de una pila (Stack): 
 * una vectorial (estática) y una dinámica (lista enlazada), que almacenan 
 * cadenas de texto (nombres).
 * El programa gestiona una 'lista de turnos' siguiendo el principio LIFO 
 * (Last In, First Out).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definiciones para la Pila Vectorial y Dinámica (MAXLEN)
#define MAX 10      /**< @brief Número máximo de palabras que caben en la pila vectorial. */
#define MAXLEN 30   /**< @brief Longitud máxima de cada palabra (incluyendo '\0'). */

/* ======== ESTRUCTURA DE LA PILA VECTORIAL ======== */

/**
 * @brief Estructura para representar una pila vectorial (estática).
 */
typedef struct {
    char data[MAX][MAXLEN]; /**< @brief Array bidimensional para almacenar las palabras. */
    int top;                /**< @brief Índice de la cima de la pila. top = -1 indica pila vacía. */
} StackVector;

/* ======== FUNCIONES BÁSICAS DE LA PILA VECTORIAL (ESTÁTICA) ======== */

/**
 * @brief Inicializa una pila vectorial vacía.
 * @param s Puntero a la estructura StackVector a inicializar.
 */
void createStackVector(StackVector *s) {
    s->top = -1; // La cima apunta a -1 cuando está vacía.
}

/**
 * @brief Comprueba si la pila vectorial está vacía.
 * @param s Estructura StackVector.
 * @return 1 si la pila está vacía (top == -1), 0 en caso contrario.
 */
int isEmptyVector(StackVector s) {
    return (s.top == -1); // Vacía si top es -1.
}

/**
 * @brief Comprueba si la pila vectorial está llena.
 * @param s Estructura StackVector.
 * @return 1 si la pila está llena (top == MAX - 1), 0 en caso contrario.
 */
int isFullVector(StackVector s) {
    return (s.top == MAX - 1); // Llena si top está en el último índice válido (MAX - 1).
}

/**
 * @brief Inserta una palabra en la cima de la pila vectorial (operación Push).
 * @param s Puntero a la estructura StackVector.
 * @param word Palabra (cadena de texto) a insertar.
 */
void pushVector(StackVector *s, const char *word) {
    if (isFullVector(*s)) {
        printf("Error: la pila esta llena. No se puede insertar: %s\n", word); // Error si está llena.
    } else {
        s->top++; // Incrementa el índice de la cima.
        // Copia la palabra en la nueva posición de la cima, asegurando la terminación
        strncpy(s->data[s->top], word, MAXLEN - 1); // Copia la palabra.
        s->data[s->top][MAXLEN - 1] = '\0'; // Asegura terminación de cadena.
        printf("Se inserto: %s\n", word);
    }
}

/**
 * @brief Elimina el elemento superior de la pila vectorial (operación Pop) y lo devuelve.
 * @param s Puntero a la estructura StackVector.
 * @param out_word Puntero a una cadena donde se copiará la palabra eliminada.
 * @return 1 si la operación fue exitosa, 0 si la pila estaba vacía.
 */
int popVector(StackVector *s, char *out_word) {
    if (isEmptyVector(*s)) {
        printf("Error: la pila esta vacia.\n"); // Error si está vacía.
        return 0;
    } else {
        // Copia la palabra de la cima al buffer de salida
        strncpy(out_word, s->data[s->top], MAXLEN - 1); // Copia la palabra.
        out_word[MAXLEN - 1] = '\0'; // Asegura terminación de cadena.
        s->top--; // Decrementa el índice de la cima.
        return 1;
    }
}

/**
 * @brief Consulta el elemento superior de la pila vectorial sin eliminarlo (operación Peek).
 * @param s Estructura StackVector.
 * @param out_word Puntero a una cadena donde se copiará la palabra de la cima.
 * @return 1 si la operación fue exitosa, 0 si la pila estaba vacía.
 */
int peekVector(StackVector s, char *out_word) {
    if (isEmptyVector(s)) {
        printf("La pila esta vacia.\n"); // Error si está vacía.
        return 0;
    } else {
        // Solo lee el contenido de data[top].
        strncpy(out_word, s.data[s.top], MAXLEN - 1);
        out_word[MAXLEN - 1] = '\0'; // Asegura terminación de cadena.
        return 1;
    }
}

/**
 * @brief Muestra todo el contenido de la pila vectorial, desde la cima hasta el fondo.
 * @param s Estructura StackVector.
 */
void printStackVector(StackVector s) {
    if (isEmptyVector(s)) {
        printf("La pila esta vacia.\n"); // Mensaje si está vacía.
    } else {
        printf("Contenido de la pila (de arriba a abajo):\n");
        // Recorre el array desde top hasta 0.
        for (int i = s.top; i >= 0; i--) {
            printf("| %s |\n", s.data[i]);
        }
    }
}

/* ======== ESTRUCTURAS DE LA PILA DINÁMICA ======== */

/**
 * @brief Estructura para un nodo de la pila dinámica.
 */
typedef struct Node {
    char data[MAXLEN];      /**< @brief Palabra almacenada en el nodo. */
    struct Node *next;      /**< @brief Puntero al siguiente nodo (el que está debajo). */
} Node;

/**
 * @brief Estructura para representar una pila dinámica (lista enlazada).
 */
typedef struct {
    Node *top;              /**< @brief Puntero al nodo superior (cima), o NULL si la pila está vacía. */
} StackDynamic;


/* ======== FUNCIONES BÁSICAS DE LA PILA DINÁMICA (LISTA ENLAZADA) ======== */

/**
 * @brief Inicializa una pila dinámica vacía.
 * @param s Puntero a la estructura StackDynamic a inicializar.
 */
void createStackDynamic(StackDynamic *s) {
    s->top = NULL; // top apunta a NULL cuando está vacía.
}

/**
 * @brief Comprueba si la pila dinámica está vacía.
 * @param s Estructura StackDynamic.
 * @return 1 si la pila está vacía (top es NULL), 0 en caso contrario.
 */
int isEmptyDynamic(StackDynamic s) {
    return (s.top == NULL); // Vacía si top es NULL.
}

/**
 * @brief Inserta una palabra en la cima de la pila dinámica (operación Push).
 * @param s Puntero a la estructura StackDynamic.
 * @param word Palabra (cadena de texto) a insertar.
 */
void pushDynamic(StackDynamic *s, const char *word) {
    // 1. Crear un nuevo nodo con malloc().
    Node *newNode = (Node *)malloc(sizeof(Node)); 
    if (newNode == NULL) {
        printf("Error: No hay memoria disponible.\n"); // Control de error de asignación de memoria
        return;
    }
    
    // 2. Guardar la palabra en data.
    strncpy(newNode->data, word, MAXLEN - 1);
    newNode->data[MAXLEN - 1] = '\0';
    
    // 3. Hacer que next del nuevo nodo apunte al nodo que antes era la cima.
    newNode->next = s->top;
    
    // 4. Actualizar top para que apunte al nuevo nodo.
    s->top = newNode;
    
    printf("Se inserto: %s\n", word);
}

/**
 * @brief Elimina el nodo superior de la pila dinámica (operación Pop) y devuelve su contenido.
 * @param s Puntero a la estructura StackDynamic.
 * @param out_word Puntero a una cadena donde se copiará la palabra eliminada.
 * @return 1 si la operación fue exitosa, 0 si la pila estaba vacía.
 */
int popDynamic(StackDynamic *s, char *out_word) {
    // 1. Verificar si la pila está vacía.
    if (isEmptyDynamic(*s)) {
        printf("Error: la pila esta vacia.\n");
        return 0;
    } else {
        Node *temp = s->top; // Guardamos el puntero al nodo a eliminar
        
        // 2. Guardar el dato del nodo superior.
        strncpy(out_word, temp->data, MAXLEN - 1);
        out_word[MAXLEN - 1] = '\0';
        
        // 3. Mover el puntero top al siguiente nodo.
        s->top = temp->next;
        
        // 4. Liberar la memoria del nodo eliminado con free().
        free(temp);
        
        return 1;
    }
}

/**
 * @brief Consulta el elemento superior de la pila dinámica sin eliminarlo (operación Peek).
 * @param s Estructura StackDynamic.
 * @param out_word Puntero a una cadena donde se copiará la palabra de la cima.
 * @return 1 si la operación fue exitosa, 0 si la pila estaba vacía.
 */
int peekDynamic(StackDynamic s, char *out_word) {
    // Se accede al nodo apuntado por top, pero no se modifica nada.
    if (isEmptyDynamic(s)) {
        printf("La pila esta vacia.\n"); // Mensaje si está vacía
        return 0;
    } else {
        strncpy(out_word, s.top->data, MAXLEN - 1);
        out_word[MAXLEN - 1] = '\0';
        return 1;
    }
}

/**
 * @brief Muestra todo el contenido de la pila dinámica, desde la cima hasta el fondo.
 * @param s Estructura StackDynamic.
 */
void printStackDynamic(StackDynamic s) {
    if (isEmptyDynamic(s)) {
        printf("La pila esta vacia.\n");
    } else {
        printf("Contenido de la pila (de arriba a abajo):\n");
        Node *aux = s.top; // Puntero auxiliar para recorrer la lista
        // Recorre la lista de nodos desde top hasta NULL.
        while (aux != NULL) {
            printf("| %s |\n", aux->data);
            aux = aux->next; // Avanza al siguiente nodo
        }
    }
}

/**
 * @brief Libera toda la memoria de los nodos de la pila dinámica (operación Clear).
 * @param s Puntero a la estructura StackDynamic.
 */
void clearStackDynamic(StackDynamic *s) {
    Node *temp;
    // Recorre la pila nodo a nodo, liberando con free() cada uno de ellos.
    while (s->top != NULL) {
        temp = s->top; // Guardamos el nodo actual
        s->top = s->top->next; // top pasa al siguiente nodo
        free(temp); // Liberamos la memoria del nodo guardado
    }
    printf("Pila dinamica liberada/vaciada.\n");
}


/* ======== FUNCIONES AUXILIARES PARA EL MENÚ Y PRUEBAS ======== */

/**
 * @brief Función auxiliar para llenar una pila vectorial con nombres de prueba.
 * @param s Puntero a la estructura StackVector.
 * @param count Número de nombres a intentar apilar.
 */
void fillStackVectorWithNames(StackVector *s, int count) {
    const char *names[] = {
        "Ana", "Luis", "Marta", "Pablo", "Sofia", "Carlos", "Elena", "Raul", "Lucia", "Andres",
        "Clara", "Javier", "Paula", "David", "Maria", "Hugo", "Irene", "Sergio", "Nuria", "Alberto",
        "Eva", "Jorge", "Sandra", "Manuel", "Beatriz", "Diego", "Laura", "Carmen", "Antonio", "Sara"
    };
    int totalNames = sizeof(names) / sizeof(names[0]);
    
    printf("\nIntentando insertar %d nombres en la pila (MAX = %d):\n", count, MAX);
    int namesToInsert = (count < totalNames) ? count : totalNames;

    for (int i = 0; i < namesToInsert; i++) {
        pushVector(s, names[i]);
    }
}

/**
 * @brief Función auxiliar para llenar una pila dinámica con nombres de prueba.
 * @param s Puntero a la estructura StackDynamic.
 * @param count Número de nombres a intentar apilar.
 */
void fillStackDynamicWithNames(StackDynamic *s, int count) {
    const char *names[] = {
        "Ana", "Luis", "Marta", "Pablo", "Sofia", "Carlos", "Elena", "Raul", "Lucia", "Andres",
        "Clara", "Javier", "Paula", "David", "Maria", "Hugo", "Irene", "Sergio", "Nuria", "Alberto",
        "Eva", "Jorge", "Sandra", "Manuel", "Beatriz", "Diego", "Laura", "Carmen", "Antonio", "Sara"
    };
    int totalNames = sizeof(names) / sizeof(names[0]);
    
    printf("\nInsertando %d nombres en la pila dinamica:\n", count);
    int namesToInsert = (count < totalNames) ? count : totalNames;

    for (int i = 0; i < namesToInsert; i++) {
        pushDynamic(s, names[i]);
    }
}

/**
 * @brief Muestra el menú de opciones para la pila vectorial.
 */
void menuVectorial() {
    StackVector stack;
    createStackVector(&stack); // Inicializa la pila
    int opcion;
    char palabra[MAXLEN];

    do {
        printf("\n=== PILA VECTORIAL ===\n");
        printf("1. Push (insertar palabra)\n");
        printf("2. Pop (sacar palabra)\n");
        printf("3. Peek (ver cima)\n");
        printf("4. Mostrar pila\n");
        printf("5. Llenar con 10 nombres (Prueba MAX)\n");
        printf("6. Llenar con 20 nombres (Prueba desbordamiento)\n");
        printf("7. Volver al menu principal\n");
        printf("Opcion: ");
        if (scanf("%d", &opcion) != 1) {
             opcion = 0; // Opción no válida
        }
        while (getchar() != '\n'); // Limpiar salto de línea

        switch (opcion) {
            case 1:
                printf("Introduce una palabra: ");
                // fgets para leer la línea completa, incluyendo espacios
                if (fgets(palabra, MAXLEN, stdin)) {
                    // Quitar el salto de línea al final de la cadena
                    palabra[strcspn(palabra, "\n")] = '\0';
                    pushVector(&stack, palabra);
                }
                break;
            case 2:
                if (popVector(&stack, palabra))
                    printf("Se desapilo: %s\n", palabra);
                break;
            case 3:
                if (peekVector(stack, palabra))
                    printf("Elemento en la cima: %s\n", palabra);
                break;
            case 4:
                printStackVector(stack);
                break;
            case 5:
                fillStackVectorWithNames(&stack, 10); // Prueba 1: Llenar hasta MAX
                break;
            case 6:
                fillStackVectorWithNames(&stack, 20); // Prueba 2: Intentar desbordamiento (20 > MAX=10)
                break;
            case 7:
                printf("Volviendo al menu principal.\n");
                break;
            default:
                printf("Opcion no valida.\n");
        }
    } while (opcion != 7);
}

/**
 * @brief Muestra el menú de opciones para la pila dinámica.
 */
void menuDinamica() {
    StackDynamic stack;
    createStackDynamic(&stack); // Inicializa la pila
    int opcion;
    char palabra[MAXLEN];

    do {
        printf("\n=== PILA DINÁMICA ===\n");
        printf("1. Push (insertar palabra)\n");
        printf("2. Pop (sacar palabra)\n");
        printf("3. Peek (ver cima)\n");
        printf("4. Mostrar pila\n");
        printf("5. Llenar con 20 nombres (Prueba crecimiento dinámico)\n");
        printf("6. Vaciar pila (Clear)\n");
        printf("7. Volver al menu principal\n");
        printf("Opcion: ");
        if (scanf("%d", &opcion) != 1) {
             opcion = 0; // Opción no válida
        }
        while (getchar() != '\n'); // Limpiar salto de línea

        switch (opcion) {
            case 1:
                printf("Introduce una palabra: ");
                if (fgets(palabra, MAXLEN, stdin)) {
                    palabra[strcspn(palabra, "\n")] = '\0';
                    pushDynamic(&stack, palabra);
                }
                break;
            case 2:
                if (popDynamic(&stack, palabra))
                    printf("Se desapilo: %s\n", palabra);
                break;
            case 3:
                if (peekDynamic(stack, palabra))
                    printf("Elemento en la cima: %s\n", palabra);
                break;
            case 4:
                printStackDynamic(stack);
                break;
            case 5:
                fillStackDynamicWithNames(&stack, 20); // Prueba: Llenar con 20 nombres (no hay límite de MAX)
                break;
            case 6:
                clearStackDynamic(&stack);
                break;
            case 7:
                printf("Volviendo al menu principal.\n");
                break;
            default:
                printf("Opcion no valida.\n");
        }
    } while (opcion != 7);
    
    // Al salir, liberar memoria si queda algo.
    if (!isEmptyDynamic(stack)) {
        clearStackDynamic(&stack);
    }
}

/* ======== PROGRAMA PRINCIPAL ======== */

/**
 * @brief Función principal del programa. Muestra el menú principal y gestiona la elección
 * entre la pila vectorial y la pila dinámica.
 * @return 0 al finalizar.
 */
int main() {
    int opcion;

    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Pila vectorial\n");
        printf("2. Pila dinamica\n");
        printf("3. Salir\n");
        printf("Opcion: ");
        
        // Manejo de entrada robusto para evitar bucles infinitos con scanf
        if (scanf("%d", &opcion) != 1) {
            opcion = 0; // Marca como opción no válida
        }
        while (getchar() != '\n'); // Limpiar el buffer de entrada (incluyendo el salto de línea)

        if (opcion == 1) {
            menuVectorial();
        } else if (opcion == 2) {
            menuDinamica();
        } else if (opcion == 3) {
            printf("Fin del programa.\n");
        } else {
            printf("Opcion no valida.\n");
        }
    } while (opcion != 3);

    return 0;
}
