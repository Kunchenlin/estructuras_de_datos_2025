# Pilas (Stacks)

## Descripción

Las pilas son estructuras de datos que siguen el principio **LIFO** (Last In, First Out - Último en Entrar, Primero en Salir). Es como una pila de platos: el último plato que colocas encima es el primero que puedes retirar.

## Programas en esta carpeta

### pilas.c
Implementación completa y comparación de dos tipos de pilas:

1. **Pila Vectorial (Estática)**
   - Implementada con arrays de tamaño fijo
   - Más eficiente en memoria cuando el tamaño máximo es conocido
   - Puede llenarse (overflow)

2. **Pila Dinámica (Lista Enlazada)**
   - Implementada con nodos enlazados
   - Crece dinámicamente según necesidad
   - Más flexible pero usa más memoria por nodo

## Operaciones Principales

- `push()`: Añadir un elemento a la cima de la pila
- `pop()`: Extraer el elemento de la cima
- `top()` / `peek()`: Consultar el elemento de la cima sin extraerlo
- `isEmpty()`: Verificar si la pila está vacía
- `isFull()`: Verificar si la pila está llena (solo para pila vectorial)

## Compilación

```bash
gcc -o pilas pilas.c -Wall -Wextra
```

## Ejecución

```bash
./pilas
```

## Aplicaciones de las Pilas

- Gestión de turnos (LIFO)
- Evaluación de expresiones matemáticas
- Historial de navegación (botón "atrás")
- Deshacer/Rehacer en editores
- Llamadas a funciones recursivas (call stack)

## Conceptos Clave

- **Cima (top)**: Posición del último elemento insertado
- **Overflow**: Intentar insertar en una pila llena
- **Underflow**: Intentar extraer de una pila vacía
