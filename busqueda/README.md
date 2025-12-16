# Algoritmos de Búsqueda

## Descripción

Los algoritmos de búsqueda permiten localizar elementos específicos dentro de estructuras de datos. La eficiencia de cada algoritmo depende del tipo de datos y si están ordenados o no.

## Programas en esta carpeta

### busqueda_enteros.c
Búsqueda en arrays de enteros:
- Búsqueda lineal simple
- Implementación básica para datos numéricos

### busqueda_struct.c
Búsqueda en arrays de estructuras:
- Búsqueda en estructuras complejas (registros)
- Comparación de campos específicos
- Manejo de tipos de datos compuestos

### buscar_titulo_lineal.c
Búsqueda lineal de títulos en biblioteca:
- Búsqueda de libros por título
- Comparación de cadenas de texto
- Aplicación práctica en sistema de biblioteca

### ordenacion_binaria.c
Implementación de búsqueda binaria:
- Requiere datos ordenados previamente
- Complejidad O(log n)
- Divide y vencerás (divide and conquer)

## Tipos de Búsqueda

### 1. Búsqueda Lineal (Linear Search)
**Complejidad**: O(n)
- Recorre secuencialmente todos los elementos
- No requiere datos ordenados
- Útil para conjuntos pequeños o búsquedas poco frecuentes

**Ventajas:**
- Simple de implementar
- Funciona con datos no ordenados
- No requiere preprocessing

**Desventajas:**
- Lenta para grandes cantidades de datos
- Examina todos los elementos en el peor caso

### 2. Búsqueda Binaria (Binary Search)
**Complejidad**: O(log n)
- Requiere datos ordenados
- Divide el espacio de búsqueda a la mitad en cada iteración
- Muy eficiente para grandes conjuntos de datos

**Ventajas:**
- Muy rápida en datos ordenados
- Escala bien con grandes volúmenes

**Desventajas:**
- Requiere datos ordenados previamente
- Más compleja de implementar
- No útil si los datos cambian frecuentemente

## Compilación

```bash
# Búsqueda en enteros
gcc -o busqueda_enteros busqueda_enteros.c -Wall -Wextra

# Búsqueda en estructuras
gcc -o busqueda_struct busqueda_struct.c -Wall -Wextra

# Búsqueda de títulos (requiere common.h)
gcc -o buscar_titulo buscar_titulo_lineal.c -I../utilidades -Wall -Wextra

# Búsqueda binaria
gcc -o ordenacion_binaria ordenacion_binaria.c -Wall -Wextra
```

## Ejecución

```bash
./busqueda_enteros
./busqueda_struct
./buscar_titulo
./ordenacion_binaria
```

## Comparación de Complejidades

| Algoritmo | Mejor Caso | Caso Promedio | Peor Caso | Datos Ordenados |
|-----------|------------|---------------|-----------|-----------------|
| Lineal | O(1) | O(n) | O(n) | No necesario |
| Binaria | O(1) | O(log n) | O(log n) | Requerido |

## Aplicaciones

- Búsqueda en bases de datos
- Sistemas de bibliotecas
- Diccionarios y mapas
- Autocompletado
- Verificación de existencia de elementos
- Índices en sistemas de archivos

## Conceptos Clave

- **Complejidad temporal**: Tiempo que toma el algoritmo en función del tamaño de entrada
- **Búsqueda exitosa**: El elemento se encuentra en la estructura
- **Búsqueda fallida**: El elemento no existe
- **Mejor caso**: Elemento en la primera posición (lineal) o al centro (binaria)
- **Peor caso**: Elemento al final o no existe
- **Invariante**: Condición que se mantiene en cada iteración

## Cuándo usar cada algoritmo

**Usa Búsqueda Lineal cuando:**
- Los datos no están ordenados
- El conjunto de datos es pequeño (< 100 elementos)
- Las búsquedas son infrecuentes
- Simplicidad es más importante que velocidad

**Usa Búsqueda Binaria cuando:**
- Los datos están o pueden ordenarse
- El conjunto de datos es grande
- Las búsquedas son frecuentes
- El rendimiento es crítico
