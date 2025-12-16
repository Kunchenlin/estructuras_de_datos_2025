#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * @file metro.c
 * @brief Programa para modelar una red de metro utilizando grafos.
 *
 * Este programa permite crear, eliminar túneles entre estaciones de metro,
 * listar las conexiones de una estación, y realizar recorridos en anchura (BFS)
 * y en profundidad (DFS) sobre la red de metro.
 */

/** Número máximo de estaciones en la red */
#define MAX_ESTACIONES 6

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


// -- Defino los prototipos de las funciones -- 
void inicializarRed(RedMetro *red);
void crearTunel(RedMetro *red, int indice1, int indice2);
void eliminarTunel(RedMetro *red, int indice1, int indice2);
void listarConexiones(RedMetro *red, int indice);
void recorridoBFS(RedMetro *red, int inicio);
void recorridoDFS(RedMetro *red, int inicio);
void dfsRecursivo(RedMetro *red, int actual, bool visitado[]);
void mostrarMenu();

int main() {
    RedMetro red;
    int opcion;
    
    inicializarRed(&red);

    do {
        mostrarMenu();
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
            default:
                printf("Opción no válida. Intenta nuevamente.\n");
        }
    } while (opcion != 6);

    return 0;
}


/**
 * @brief Inicializa la red de metro con las estaciones predefinidas.
 *
 * Esta función configura las estaciones iniciales y los túneles existentes.
 *
 * @param red Puntero a la estructura de la red de metro.
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
 *
 * Solicita dos estaciones (por índice o nombre) y establece una conexión en
 * la matriz de adyacencia.
 *
 * @param red Puntero a la estructura de la red de metro.
 * @param indice1 Índice de la primera estación.
 * @param indice2 Índice de la segunda estación.
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
 *
 * Solicita dos estaciones y elimina la conexión entre ellas en la matriz
 * de adyacencia.
 *
 * @param red Puntero a la estructura de la red de metro.
 * @param indice1 Índice de la primera estación.
 * @param indice2 Índice de la segunda estación.
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
 *
 * Muestra las estaciones a las que está conectada una estación dada.
 *
 * @param red Puntero a la estructura de la red de metro.
 * @param indice Índice de la estación de la que listar las conexiones.
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
 *
 * Solicita la estación inicial y realiza un recorrido en anchura
 * mostrando el orden de visita.
 *
 * @param red Puntero a la estructura de la red de metro.
 * @param inicio Índice de la estación de inicio para BFS.
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
 *
 * Solicita la estación inicial y realiza un recorrido en profundidad
 * mostrando el orden de visita.
 *
 * @param red Puntero a la estructura de la red de metro.
 * @param inicio Índice de la estación de inicio para DFS.
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
 *
 * @param red Puntero a la estructura de la red de metro.
 * @param actual Índice de la estación actual.
 * @param visitado Arreglo para marcar las estaciones visitadas.
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
 *
 * El menú permite al usuario elegir entre crear túneles, eliminar túneles,
 * listar conexiones, realizar recorridos BFS/DFS o salir del programa.
 */
void mostrarMenu() {
    printf("\n--- Menú ---\n");
    printf("1. Crear túnel entre dos estaciones\n");
    printf("2. Eliminar túnel entre dos estaciones\n");
    printf("3. Listar conexiones de una estación\n");
    printf("4. Recorrido en Anchura (BFS)\n");
    printf("5. Recorrido en Profundidad (DFS)\n");
    printf("6. Salir\n");
    printf("Selecciona una opción: ");
}
