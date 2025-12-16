# Estructuras de Datos 2025

Este repositorio contiene código de ejemplo y ejercicios para el curso de Estructuras de Datos 2025. Los programas están implementados en lenguaje C y cubren los conceptos fundamentales de estructuras de datos.

## 📚 Contenido del Repositorio

### 📂 Estructura de Directorios

```
estructuras_de_datos_2025/
├── pilas/              # Implementaciones de pilas (stacks)
├── colas/              # Implementaciones de colas (queues)
├── listas/             # Listas enlazadas y operaciones
├── busqueda/           # Algoritmos de búsqueda
├── ordenacion/         # Algoritmos de ordenación
├── gestores/           # Sistemas de gestión completos
├── utilidades/         # Archivos de utilidad y bibliotecas
├── examen_01.c         # Ejercicio de examen 1
└── Ejercicio-Final-V.2.0.c  # Ejercicio final versión 2.0
```

### 📑 Descripción de Programas

#### Pilas (Stacks) - LIFO
- **pilas/pilas.c**: Implementación y comparación de pilas vectoriales (estáticas) y dinámicas (listas enlazadas)

#### Colas (Queues) - FIFO
- **colas/cola_dinamica.c**: Cola dinámica para gestión de procesos de CPU
- **colas/cola_vectorial.c**: Implementación de cola usando array estático

#### Listas Enlazadas
- **listas/ejercicio_listas.c**: Operaciones completas sobre listas enlazadas (inserción, eliminación, búsqueda)

#### Búsqueda
- **busqueda/busqueda_enteros.c**: Búsqueda en arrays de enteros
- **busqueda/busqueda_struct.c**: Búsqueda en arrays de estructuras
- **busqueda/buscar_titulo_lineal.c**: Búsqueda lineal de títulos en biblioteca
- **busqueda/ordenacion_binaria.c**: Implementación de búsqueda binaria

#### Ordenación
- **ordenacion/anio_mezcla_natural.c**: Ordenación por mezcla natural
- **ordenacion/ordenar_anio_mezcla_natural.c**: Ordenación de libros por año usando mezcla natural
- **ordenacion/ordenar_precio_insercion.c**: Ordenación de libros por precio usando inserción

#### Gestores de Sistemas
- **gestores/gestor_drones.c**: Sistema de gestión de drones
- **gestores/gestor_trenes.c**: Sistema de gestión de trenes
- **gestores/gestor_vuelo.c**: Sistema de gestión de vuelos (versión 1)
- **gestores/gestor_vuelo_v2.c**: Sistema de gestión de vuelos (versión 2)
- **gestores/metro.c**: Sistema de gestión de metro (versión 1)
- **gestores/metro_v2.c**: Sistema de gestión de metro (versión 2)

#### Utilidades
- **utilidades/common.h**: Archivo de cabecera con definiciones comunes para biblioteca
- **utilidades/crear_txt_biblioteca.c**: Crea archivo de texto con datos de biblioteca
- **utilidades/text_a_bin_biblioteca.c**: Convierte archivo de texto a binario
- **utilidades/ver_bin_biblioteca.c**: Visualiza contenido de archivo binario
- **utilidades/listar_prestado.c**: Lista libros prestados de la biblioteca

#### Ejercicios de Examen
- **examen_01.c**: Primer ejercicio de examen
- **Ejercicio-Final-V.2.0.c**: Ejercicio final versión 2.0

## 🔧 Compilación y Ejecución

### Requisitos
- Compilador GCC (GNU Compiler Collection)
- Sistema operativo: Linux, macOS, o Windows (con MinGW/Cygwin)

### Compilar un programa

```bash
gcc -o nombre_programa nombre_programa.c -Wall -Wextra
```

Ejemplo:
```bash
gcc -o pilas pilas/pilas.c -Wall -Wextra
```

### Ejecutar un programa

```bash
./nombre_programa
```

Ejemplo:
```bash
./pilas
```

### Compilar programas que usan common.h

```bash
gcc -o programa programa.c -I./utilidades -Wall -Wextra
```

Ejemplo:
```bash
gcc -o buscar busqueda/buscar_titulo_lineal.c -I./utilidades -Wall -Wextra
```

## 📖 Temas Cubiertos

1. **Pilas (Stacks)**
   - Implementación vectorial (estática)
   - Implementación dinámica (lista enlazada)
   - Operaciones: push, pop, top, isEmpty

2. **Colas (Queues)**
   - Implementación vectorial
   - Implementación dinámica
   - Operaciones: enqueue, dequeue, front, isEmpty

3. **Listas Enlazadas**
   - Listas simples
   - Inserción (al inicio, al final, ordenada)
   - Eliminación de elementos
   - Búsqueda y recorrido

4. **Algoritmos de Búsqueda**
   - Búsqueda lineal
   - Búsqueda binaria
   - Búsqueda en estructuras complejas

5. **Algoritmos de Ordenación**
   - Ordenación por inserción
   - Ordenación por mezcla natural (merge sort)

6. **Gestión de Archivos**
   - Archivos de texto
   - Archivos binarios
   - Conversión entre formatos

7. **Aplicaciones Prácticas**
   - Sistemas de gestión de bibliotecas
   - Gestión de vuelos, trenes y drones
   - Sistemas de metro

## 👨‍💻 Uso para Estudiantes

Este repositorio está diseñado como material de aprendizaje. Se recomienda:

1. **Leer el código**: Cada archivo está documentado con comentarios explicativos
2. **Compilar y ejecutar**: Prueba cada programa para entender su funcionamiento
3. **Modificar**: Experimenta modificando el código para practicar
4. **Estudiar por temas**: Navega por las carpetas según el tema que estés estudiando

## 📝 Notas

- Los programas incluyen validación de entrada de usuario
- Se usan estructuras de datos estándar de C
- Los comentarios están en español para facilitar la comprensión
- Cada programa es independiente y puede compilarse por separado

## 📧 Contribuciones

Este es un repositorio educativo. Los estudiantes pueden:
- Reportar errores encontrados
- Sugerir mejoras en el código
- Añadir nuevos ejemplos de estructuras de datos

---

**Curso**: Estructuras de Datos 2025  
**Lenguaje**: C  
**Última actualización**: Diciembre 2025