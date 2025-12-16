# Listas Enlazadas

## Descripción

Las listas enlazadas son estructuras de datos dinámicas donde cada elemento (nodo) contiene un dato y un puntero al siguiente nodo. A diferencia de los arrays, no requieren memoria contigua y pueden crecer dinámicamente.

## Programas en esta carpeta

### ejercicio_listas.c
Implementación completa de operaciones sobre listas enlazadas simples:
- Creación de listas
- Inserción al inicio, al final y de forma ordenada
- Eliminación de elementos específicos
- Búsqueda de elementos
- Recorrido y visualización de la lista
- Destrucción de la lista

## Operaciones Principales

- `insertarInicio()`: Insertar elemento al principio
- `insertarFinal()`: Insertar elemento al final
- `insertarOrdenado()`: Insertar manteniendo orden
- `eliminar()`: Eliminar un elemento específico
- `buscar()`: Buscar un elemento en la lista
- `mostrar()`: Recorrer y mostrar todos los elementos
- `destruir()`: Liberar memoria de todos los nodos

## Compilación

```bash
gcc -o ejercicio_listas ejercicio_listas.c -Wall -Wextra
```

## Ejecución

```bash
./ejercicio_listas
```

## Tipos de Listas

### Lista Simple (Simple Linked List)
- Cada nodo apunta al siguiente
- Recorrido en una sola dirección
- Más eficiente en memoria

### Lista Doble (Double Linked List)
- Cada nodo apunta al siguiente y al anterior
- Recorrido bidireccional
- Más flexible pero usa más memoria

### Lista Circular
- El último nodo apunta al primero
- Permite recorridos circulares

## Ventajas de las Listas Enlazadas

- Tamaño dinámico (no hay límite predefinido)
- Inserción/eliminación eficiente en O(1) si se tiene el puntero al nodo
- No requiere reorganización de elementos (como en arrays)
- No desperdicia memoria con espacios vacíos

## Desventajas

- No hay acceso directo (indexado) a elementos - O(n)
- Usa más memoria (por los punteros)
- No aprovecha la caché del procesador (memoria no contigua)
- Búsqueda siempre es lineal O(n)

## Aplicaciones

- Implementación de pilas y colas
- Gestión de memoria (free lists)
- Estructuras de datos complejas (grafos, árboles)
- Historial de navegación
- Listas de reproducción
- Editor de texto (undo/redo)

## Conceptos Clave

- **Nodo**: Estructura que contiene dato y puntero(s)
- **Cabeza (head)**: Primer nodo de la lista
- **Cola (tail)**: Último nodo de la lista
- **NULL**: Indica el final de la lista
- **Puntero**: Referencia a la dirección de memoria del siguiente nodo
