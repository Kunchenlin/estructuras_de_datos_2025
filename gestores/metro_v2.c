#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h> 

/**
 * @file metro.c
 * @brief Programa para modelar una red de metro utilizando grafos.
 */

/** Número máximo de estaciones en la red */
#define MAX_ESTACIONES 6
#define INF INT_MAX 

// Estructuras del Grafo Original (Matriz de Adyacencia)
// ----------------------------------------------------
/** Estructura que representa una estación de metro */
typedef struct {
    int indice;             /**< Índice único de la estación */
    char nombre[50];        /**< Nombre de la estación */
} Estacion;

/** Estructura que representa un grafo mediante una matriz de adyacencia */
typedef struct {
    int matriz[MAX_ESTACIONES][MAX_ESTACIONES]; /**< Matriz de adyacencia */
    Estacion estaciones[MAX_ESTACIONES];        /**< Arreglo de estaciones */
} RedMetro;


// -- Defino los prototipos de las funciones (Originales) -- 
void inicializarRed(RedMetro *red);
void crearTunel(RedMetro *red, int indice1, int indice2);
void eliminarTunel(RedMetro *red, int indice1, int indice2);
void listarConexiones(RedMetro *red, int indice);
void recorridoBFS(RedMetro *red, int inicio);
void recorridoDFS(RedMetro *red, int inicio);
void dfsRecursivo(RedMetro *red, int actual, bool visitado[]);
void mostrarMenu();
int main_grafo();


int main() {
    return main_grafo();
}

int main_grafo() {
    RedMetro red;
    int opcion;
    
    inicializarRed(&red);

    do {
        mostrarMenu();
        printf("7. [TEÓRICO] Probar Dijkstra con Lista de Adyacencia (Ver código en comentarios)\n");
        printf("Selecciona una opción: ");
        scanf("%d", &opcion);
        
        switch (opcion) {
            case 1: {
                int est1, est2;
                printf("Introduce el índice de las dos estaciones para crear el túnel: ");
                scanf("%d %d", &est1, &est2);
                crearTunel(&red, est1, est2);
                break;
            }
            case 2: {
                int est1, est2;
                printf("Introduce el índice de las dos estaciones para eliminar el túnel: ");
                scanf("%d %d", &est1, &est2);
                eliminarTunel(&red, est1, est2);
                break;
            }
            case 3: {
                int est;
                printf("Introduce el índice de la estación para listar conexiones: ");
                scanf("%d", &est);
                listarConexiones(&red, est);
                break;
            }
            case 4: {
                int inicio;
                printf("Introduce el índice de la estación inicial para BFS: ");
                scanf("%d", &inicio);
                recorridoBFS(&red, inicio);
                break;
            }
            case 5: {
                int inicio;
                printf("Introduce el índice de la estación inicial para DFS: ");
                scanf("%d", &inicio);
                recorridoDFS(&red, inicio);
                break;
            }
            case 6:
                printf("Saliendo del programa.\n");
                break;
            case 7: {
                printf("--- Implementación Faltante: Dijkstra con Lista de Adyacencia ---\n");
                printf("La implementación completa (estructuras y funciones) se encuentra al final de este archivo en un bloque comentado.\n");
                break;
            }
            default:
                printf("Opción no válida. Intenta nuevamente.\n");
        }
    } while (opcion != 6); 

    return 0;
}


// Implementación Original (Matriz de Adyacencia)
// ----------------------------------------------------

/**
 * @brief Inicializa la red de metro con las estaciones predefinidas.
 */
void inicializarRed(RedMetro *red)  {
    // Inicializo las estaciones 
    strcpy(red->estaciones[0].nombre, "Sol");
    red->estaciones[0].indice = 0;
    
    strcpy(red->estaciones[1].nombre, "Gran Vía");
    red->estaciones[1].indice = 1;
    
    strcpy(red->estaciones[2].nombre, "Tribunal");
    red->estaciones[2].indice = 2;
    
    strcpy(red->estaciones[3].nombre, "Alonso Martínez");
    red->estaciones[3].indice = 3;
    
    strcpy(red->estaciones[4].nombre, "Bilbao");
    red->estaciones[4].indice = 4;
    
    strcpy(red->estaciones[5].nombre, "Quevedo");
    red->estaciones[5].indice = 5;
    
    //Inicializo la matriz de adyacencia a 0 (Sin tuneles) 
    for (int i = 0; i < MAX_ESTACIONES; i++){ 
    	for (int j = 0; j < MAX_ESTACIONES; j++) { 
    		red->matriz[i][j] = 0; 
    	}
    }
    
    //Establece las conexiones predefinidas.
    red->matriz[0][1] = red->matriz[1][0] = 1; // Sol - Gran Vía
    red->matriz[1][2] = red->matriz[2][1] = 1; // Gran Vía - Tribunal
    red->matriz[2][4] = red->matriz[4][2] = 1; // Tribunal - Bilbao
    red->matriz[4][5] = red->matriz[5][4] = 1; // Bilbao - Quevedo
    red->matriz[3][2] = red->matriz[2][3] = 1; // Alonso Martínez - Tribunal
}


/**
 * @brief Crea un túnel entre dos estaciones.
 */
void crearTunel(RedMetro *red, int indice1, int indice2){ 
	if (indice1 < 0 || indice1 >= MAX_ESTACIONES || indice2 < 0 || indice2 >= MAX_ESTACIONES) { 
		printf("Índices de estaciones inválidos. \n"); 
		return; 
	} 
	
	//Establecer el túnel (conexión bidireccional) 
	red->matriz[indice1][indice2] = 1; 
	red->matriz[indice2][indice1] = 1; 
	
	printf("Tunel creado entre %s y %s. \n", red->estaciones[indice1].nombre, red->estaciones[indice2].nombre); 
	
}
	

/**
 * @brief Elimina un túnel entre dos estaciones.
 */
void eliminarTunel(RedMetro *red, int indice1, int indice2) { 
	if (indice1 < 0 || indice1 >= MAX_ESTACIONES || indice2 < 0 || indice2 >= MAX_ESTACIONES) { 
		printf("Índices de estaciones inválidos. \n"); 
		return; 
	} 
	
	red->matriz[indice1][indice2] = 0; 
	red->matriz[indice2][indice1] = 0; 
	
	printf("Tunel eliminado entre %s y %s. \n", red->estaciones[indice1].nombre, red->estaciones[indice2].nombre); 
	
} 

/**
 * @brief Lista las estaciones conectadas a una estación.
 */
void listarConexiones(RedMetro *red, int indice){ 
	// Verifica si el índice es válido
	if (indice < 0 || indice >= MAX_ESTACIONES) { 
		fprintf(stderr, "Indice de estacion invalido. \n"); 
		return; 
	} 
	
	printf("Estaciones conectadas a %s: \n", red->estaciones[indice].nombre);
	
	// Listo las estaciones conectadas 
	for (int i = 0; i < MAX_ESTACIONES; i++) { 
		if (red->matriz[indice][i] == 1) { 
			printf("- %s \n", red->estaciones[i].nombre); 
		} 
	}
}

/**
 * @brief Realiza un recorrido en anchura (BFS) desde una estación.
 */
void recorridoBFS(RedMetro *red, int inicio){ 
    // Verificar si el índice es válido
    if (inicio < 0 || inicio >= MAX_ESTACIONES) {
        printf("Índice de estación inválido.\n");
        return;
    }

    bool visitado[MAX_ESTACIONES] = {false};
    int cola[MAX_ESTACIONES];
    int frente = 0, fin = 0;

    visitado[inicio] = true;
    cola[fin++] = inicio;

    printf("Recorrido BFS desde %s:\n", red->estaciones[inicio].nombre);

    while (frente != fin) {
        int actual = cola[frente++];
        printf("- %s\n", red->estaciones[actual].nombre);

        // Explorar las estaciones adyacentes
        for (int i = 0; i < MAX_ESTACIONES; i++) {
            if (red->matriz[actual][i] == 1 && !visitado[i]) {
                visitado[i] = true;
                cola[fin++] = i;
            }
        }
    }
}

/**
 * @brief Realiza un recorrido en profundidad (DFS) desde una estación.
 */
void recorridoDFS(RedMetro *red, int inicio) {
    // Verificar si el índice es válido
    if (inicio < 0 || inicio >= MAX_ESTACIONES) {
        printf("Índice de estación inválido.\n");
        return;
    }

    bool visitado[MAX_ESTACIONES] = {false};
    printf("Recorrido DFS desde %s:\n", red->estaciones[inicio].nombre);

    // Llamada recursiva para realizar el recorrido DFS
    dfsRecursivo(red, inicio, visitado);
}

/**
 * @brief Función recursiva auxiliar para realizar DFS.
 */
void dfsRecursivo(RedMetro *red, int actual, bool visitado[]) {
    visitado[actual] = true;
    printf("- %s\n", red->estaciones[actual].nombre);

    // Recorrer las estaciones adyacentes
    for (int i = 0; i < MAX_ESTACIONES; i++) {
        if (red->matriz[actual][i] == 1 && !visitado[i]) {
            dfsRecursivo(red, i, visitado);
        }
    }
}

/**
 * @brief Muestra el menú de opciones al usuario.
 */
void mostrarMenu() {
    printf("\n--- Menú ---\n");
    printf("1. Crear túnel entre dos estaciones\n");
    printf("2. Eliminar túnel entre dos estaciones\n");
    printf("3. Listar conexiones de una estación\n");
    printf("4. Recorrido en Anchura (BFS)\n");
    printf("5. Recorrido en Profundidad (DFS)\n");
    printf("6. Salir\n");
}


// IMPLEMENTACIÓN FALTANTE: Grafo Ponderado con Lista de Adyacencia y Dijkstra
// -------------------------------------------------------------------------------------------------------------------------------------
/*
// Estructura de la Lista de Adyacencia (Nodo que representa un arco)
typedef struct Nodo {
    int destino;
    int peso; // El valor asociado a la arista (coste o tiempo)
    struct Nodo *sig;
} Nodo_Lista;

// Estructura del Grafo (Array de Listas)
typedef struct {
    int numVertices;
    char nombres[MAX_ESTACIONES][50];
    Nodo_Lista *listaAdy[MAX_ESTACIONES]; // Array de punteros a listas
} RedMetroLista;

// -- Prototypes para el bloque faltante --
void inicializarRedLista(RedMetroLista *red);
void crearArco(RedMetroLista *red, int origen, int destino, int peso);
int minDistance(int dist[], int visitado[], int n);
void dijkstra(RedMetroLista *red, int inicio, int fin);
void liberarLista(Nodo_Lista *cabeza);
void liberarRedLista(RedMetroLista *red);

// Implementación de Dijkstra y auxiliares (adaptado al array de listas)

int minDistance(int dist[], int visitado[], int n) {
    int min = INF, min_index = -1;
    for (int v = 0; v < n; v++) {
        if (visitado[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(RedMetroLista *red, int inicio, int fin) {
    if (inicio < 0 || inicio >= red->numVertices || fin < 0 || fin >= red->numVertices) return;
    
    int dist[MAX_ESTACIONES];
    int visitado[MAX_ESTACIONES] = {0}; 
    int padre[MAX_ESTACIONES];

    for (int i = 0; i < red->numVertices; i++) {
        dist[i] = INF;
        padre[i] = -1;
    }
    dist[inicio] = 0; 

    for (int count = 0; count < red->numVertices - 1; count++) {
        int u = minDistance(dist, visitado, red->numVertices);
        if (u == -1 || dist[u] == INF) break; 
        
        visitado[u] = 1;

        Nodo_Lista *actual = red->listaAdy[u];
        while (actual != NULL) {
            int v = actual->destino;
            int peso = actual->peso;

            // Relajación de arista: dist[u] + peso < dist[v]
            if (visitado[v] == 0 && dist[u] != INF && dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
                padre[v] = u;
            }
            actual = actual->sig;
        }
    }
    
    if (dist[fin] == INF) {
        printf("La estación %s es inalcanzable desde %s.\n", red->nombres[fin], red->nombres[inicio]);
    } else {
        printf("Camino más corto de %s a %s: %d min\n", red->nombres[inicio], red->nombres[fin], dist[fin]);
    }
}

// Implementación de Lista de Adyacencia

void inicializarRedLista(RedMetroLista *red) {
    red->numVertices = MAX_ESTACIONES;
    char temp_nombres[MAX_ESTACIONES][50] = {"Sol", "Gran Vía", "Tribunal", "Alonso Martínez", "Bilbao", "Quevedo"};
    for(int i = 0; i < MAX_ESTACIONES; i++) {
        strcpy(red->nombres[i], temp_nombres[i]);
        red->listaAdy[i] = NULL;
    }
    // crearArco(red, 0, 1, 5); // Ejemplo de conexión
}

void crearArco(RedMetroLista *red, int origen, int destino, int peso) {
    if (origen < 0 || origen >= red->numVertices || destino < 0 || destino >= red->numVertices) return;

    // Función auxiliar para insertar en la cabeza de una lista (O(1))
    void _insertar(int u, int v, int w) {
        Nodo_Lista *nuevo = (Nodo_Lista *)malloc(sizeof(Nodo_Lista));
        if (!nuevo) { fprintf(stderr, "Error de asignación de memoria.\n"); exit(1); }
        nuevo->destino = v;
        nuevo->peso = w;
        nuevo->sig = red->listaAdy[u];
        red->listaAdy[u] = nuevo;
    }

    _insertar(origen, destino, peso);
    _insertar(destino, origen, peso); // Bidireccional
}

void liberarLista(Nodo_Lista *cabeza) {
    Nodo_Lista *actual = cabeza;
    while (actual != NULL) {
        Nodo_Lista *temp = actual;
        actual = actual->sig;
        free(temp);
    }
}

void liberarRedLista(RedMetroLista *red) {
    for (int i = 0; i < red->numVertices; i++) {
        liberarLista(red->listaAdy[i]);
    }
}
*/