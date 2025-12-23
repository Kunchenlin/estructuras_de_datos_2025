# Algoritmos de Ordenación

## Descripción

Los algoritmos de ordenación organizan elementos de una colección según un criterio específico (numérico, alfabético, etc.). Son fundamentales para optimizar búsquedas y presentación de datos.

## Programas en esta carpeta

### anio_mezcla_natural.c
Implementación del algoritmo de ordenación por mezcla natural:
- Variante del merge sort
- Aprovecha secuencias ya ordenadas en los datos
- Divide archivos y los mezcla ordenadamente

### ordenar_anio_mezcla_natural.c
Aplicación práctica: ordenar libros por año de publicación:
- Usa el algoritmo de mezcla natural
- Trabaja con la estructura Libro
- Ordena por campo anioPublicacion

### ordenar_precio_insercion.c
Ordenación de libros por precio usando inserción:
- Algoritmo de ordenación por inserción
- Ordena por campo precio
- Comparación de valores decimales (double)

## Algoritmos Implementados

### 1. Ordenación por Inserción (Insertion Sort)
**Complejidad**: 
- Mejor caso: O(n) - datos ya ordenados
- Caso promedio: O(n²)
- Peor caso: O(n²) - datos en orden inverso

**Funcionamiento:**
- Recorre el array elemento por elemento
- Cada elemento se inserta en su posición correcta
- Similar a ordenar cartas en la mano

**Ventajas:**
- Simple de implementar
- Eficiente para conjuntos pequeños
- Estable (mantiene orden relativo)
- Ordenación en el lugar (in-place)
- Eficiente para datos casi ordenados

**Desventajas:**
- Lenta para grandes conjuntos de datos
- Complejidad cuadrática

### 2. Ordenación por Mezcla Natural (Natural Merge Sort)
**Complejidad**: O(n log n) en todos los casos

**Funcionamiento:**
- Divide los datos en secuencias naturalmente ordenadas
- Mezcla estas secuencias de dos en dos
- Repite hasta tener una sola secuencia ordenada
- Variante inteligente del merge sort clásico

**Ventajas:**
- Complejidad O(n log n) garantizada
- Aprovecha orden existente en los datos
- Estable
- Predecible en rendimiento

**Desventajas:**
- Requiere espacio adicional O(n)
- Más complejo de implementar
- Overhead de división y mezcla

## Compilación

```bash
# Mezcla natural genérica
gcc -o anio_mezcla anio_mezcla_natural.c -Wall -Wextra

# Ordenar libros por año (requiere common.h)
gcc -o ordenar_anio ordenar_anio_mezcla_natural.c -I../utilidades -Wall -Wextra

# Ordenar libros por precio (requiere common.h)
gcc -o ordenar_precio ordenar_precio_insercion.c -I../utilidades -Wall -Wextra
```

## Ejecución

```bash
./anio_mezcla
./ordenar_anio
./ordenar_precio
```

## Comparación de Algoritmos

| Algoritmo | Mejor Caso | Promedio | Peor Caso | Espacio | Estable |
|-----------|------------|----------|-----------|---------|---------|
| Inserción | O(n) | O(n²) | O(n²) | O(1) | Sí |
| Mezcla Natural | O(n log n) | O(n log n) | O(n log n) | O(n) | Sí |
| Quick Sort | O(n log n) | O(n log n) | O(n²) | O(log n) | No |
| Heap Sort | O(n log n) | O(n log n) | O(n log n) | O(1) | No |
| Bubble Sort | O(n) | O(n²) | O(n²) | O(1) | Sí |
| Selection Sort | O(n²) | O(n²) | O(n²) | O(1) | No |

## Aplicaciones

- Preparar datos para búsqueda binaria
- Ordenar resultados de consultas en bases de datos
- Ranking y clasificaciones
- Procesamiento de archivos grandes
- Organización de bibliotecas y catálogos
- Análisis de datos

## Conceptos Clave

- **Estabilidad**: Mantiene el orden relativo de elementos iguales
- **In-place**: Ordena usando O(1) espacio adicional
- **Divide y vencerás**: Divide el problema en subproblemas más pequeños
- **Comparaciones**: Número de veces que se comparan elementos
- **Intercambios**: Número de veces que se mueven elementos

## Cuándo usar cada algoritmo

**Usa Inserción cuando:**
- Conjunto de datos pequeño (< 50 elementos)
- Datos casi ordenados
- Necesitas implementación simple
- Espacio es limitado

**Usa Mezcla Natural cuando:**
- Conjunto de datos grande
- Necesitas garantía de O(n log n)
- Estabilidad es importante
- Datos tienen secuencias ya ordenadas
- Rendimiento predecible es crítico

## Criterios de Ordenación

Los programas en esta carpeta ordenan libros por:
- **Año de publicación** (entero): orden cronológico
- **Precio** (double): orden numérico decimal

Se pueden modificar fácilmente para ordenar por:
- Título (alfabético)
- Autor (alfabético)
- Estado de préstamo
- Múltiples criterios combinados
