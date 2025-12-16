# Colas (Queues)

## Descripción

Las colas son estructuras de datos que siguen el principio **FIFO** (First In, First Out - Primero en Entrar, Primero en Salir). Es como una fila de personas: la primera persona que llega es la primera que es atendida.

## Programas en esta carpeta

### cola_dinamica.c
Implementación de cola dinámica (lista enlazada) para gestionar procesos de CPU:
- Usa nodos enlazados para almacenar procesos
- Crece dinámicamente según necesidad
- Incluye estructura PROCESO con PID, nombre y tiempo de ejecución
- Permite: insertar procesos, eliminarlos (FIFO), consultar siguiente y mostrar cola completa

### cola_vectorial.c
Implementación de cola usando array estático:
- Tamaño fijo predefinido
- Más eficiente en acceso a memoria contigua
- Puede llenarse (overflow)
- Implementación circular para optimizar espacio

## Operaciones Principales

- `enqueue()` / `insertar()`: Añadir un elemento al final de la cola
- `dequeue()` / `eliminar()`: Extraer el elemento del frente de la cola
- `front()` / `consultar()`: Ver el elemento del frente sin extraerlo
- `isEmpty()`: Verificar si la cola está vacía
- `isFull()`: Verificar si la cola está llena (solo vectorial)

## Compilación

```bash
# Cola dinámica
gcc -o cola_dinamica cola_dinamica.c -Wall -Wextra

# Cola vectorial
gcc -o cola_vectorial cola_vectorial.c -Wall -Wextra
```

## Ejecución

```bash
./cola_dinamica
# o
./cola_vectorial
```

## Aplicaciones de las Colas

- Gestión de procesos en sistemas operativos
- Colas de impresión
- Atención al cliente
- Buffer de datos en comunicaciones
- Breadth-First Search (BFS) en grafos
- Manejo de solicitudes en servidores

## Conceptos Clave

- **Frente (front/inicio)**: Posición del primer elemento (próximo a salir)
- **Final (rear/fin)**: Posición del último elemento insertado
- **Cola Circular**: Optimización para reutilizar espacio en implementación vectorial
- **Overflow**: Intentar insertar en una cola llena
- **Underflow**: Intentar extraer de una cola vacía

## Diferencias entre Implementaciones

| Característica | Cola Vectorial | Cola Dinámica |
|----------------|----------------|---------------|
| Memoria | Fija, predefinida | Dinámica, crece según necesidad |
| Flexibilidad | Limitada por tamaño | Ilimitada (hasta memoria disponible) |
| Eficiencia | Acceso O(1) | Acceso O(1), pero más overhead por nodo |
| Complejidad | Más simple | Requiere manejo de punteros |
