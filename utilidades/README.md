# Utilidades y Bibliotecas

## Descripción

Esta carpeta contiene archivos auxiliares, bibliotecas compartidas y programas de utilidad que dan soporte a otros programas del repositorio.

## Archivos en esta carpeta

### common.h
**Archivo de cabecera compartido** con definiciones comunes para el sistema de biblioteca:

**Contenido:**
- Constantes globales (`CAD_TITULO`, `CAD_AUTOR`, `NOMBRE_ARCHIVO`)
- Estructura `Libro` con campos:
  - `titulo`: Título del libro (81 caracteres)
  - `autor`: Nombre del autor (51 caracteres)
  - `anioPublicacion`: Año de publicación (entero)
  - `prestado`: Estado del préstamo ("Si" o "No")
  - `precio`: Precio en euros (double)
- Función auxiliar `chomp()`: Elimina saltos de línea de cadenas

**Uso:**
```c
#include "common.h"
```

**Compilar programas que usan common.h:**
```bash
gcc -o programa programa.c -I/ruta/a/utilidades -Wall -Wextra
```

### crear_txt_biblioteca.c
Programa para crear archivo de texto con datos de biblioteca:
- Genera archivo `libros_biblioteca.txt`
- Formato: campos separados por delimitadores
- Datos de ejemplo para pruebas
- Base para conversión a binario

**Uso:**
```bash
gcc -o crear_txt crear_txt_biblioteca.c -I. -Wall -Wextra
./crear_txt
```

### text_a_bin_biblioteca.c
Conversor de archivo de texto a binario:
- Lee `libros_biblioteca.txt`
- Genera `libros_biblioteca.bin`
- Formato binario para acceso rápido
- Usa estructura `Libro` de common.h

**Flujo:**
1. Lee línea por línea el archivo de texto
2. Parsea cada campo
3. Crea estructura Libro
4. Escribe en formato binario

**Uso:**
```bash
gcc -o text_to_bin text_a_bin_biblioteca.c -I. -Wall -Wextra
./text_to_bin
```

### ver_bin_biblioteca.c
Visualizador de archivo binario:
- Lee `libros_biblioteca.bin`
- Muestra contenido en formato legible
- Útil para verificar datos
- Debugging de archivos binarios

**Uso:**
```bash
gcc -o ver_bin ver_bin_biblioteca.c -I. -Wall -Wextra
./ver_bin
```

### listar_prestado.c
Lista libros según estado de préstamo:
- Lee archivo binario de biblioteca
- Filtra por estado "prestado"
- Muestra libros prestados o disponibles
- Ejemplo de consulta específica

**Uso:**
```bash
gcc -o listar listar_prestado.c -I. -Wall -Wextra
./listar
```

## Flujo de Trabajo Típico

```
1. crear_txt_biblioteca.c     →  Crear archivo de texto
                                  (libros_biblioteca.txt)
                                  
2. text_a_bin_biblioteca.c    →  Convertir a binario
                                  (libros_biblioteca.bin)
                                  
3. ver_bin_biblioteca.c       →  Verificar contenido
                                  (visualizar datos)
                                  
4. listar_prestado.c          →  Consultar datos específicos
                                  (filtrar prestados)
```

## Archivos de Datos Generados

- **libros_biblioteca.txt**: Archivo de texto plano con datos de libros
- **libros_biblioteca.bin**: Archivo binario con registros de estructura Libro

**Nota**: Estos archivos son generados por los programas y no deben incluirse en el repositorio (usar .gitignore)

## Ventajas de Archivos Binarios

Comparado con archivos de texto:
- **Acceso más rápido**: Lectura directa de estructuras
- **Menor tamaño**: No hay espacios ni delimitadores
- **Acceso aleatorio**: Fácil posicionamiento con fseek()
- **Tipo seguro**: Estructura definida

## Desventajas de Archivos Binarios

- **No legibles**: No se pueden abrir con editor de texto
- **Dependencia de arquitectura**: Problemas de portabilidad
- **Difícil depuración**: Requieren programas para visualizarlos

## Compilación de Todos los Programas

```bash
# Desde el directorio utilidades
gcc -o crear_txt crear_txt_biblioteca.c -I. -Wall -Wextra
gcc -o text_to_bin text_a_bin_biblioteca.c -I. -Wall -Wextra
gcc -o ver_bin ver_bin_biblioteca.c -I. -Wall -Wextra
gcc -o listar listar_prestado.c -I. -Wall -Wextra
```

## Uso de common.h en Otros Programas

Para usar common.h en programas de otros directorios:

```bash
# Desde directorio padre
gcc -o programa directorio/programa.c -I./utilidades -Wall -Wextra

# Ejemplos:
gcc -o buscar busqueda/buscar_titulo_lineal.c -I./utilidades -Wall -Wextra
gcc -o ordenar ordenacion/ordenar_precio_insercion.c -I./utilidades -Wall -Wextra
```

## Conceptos Clave

- **Header file (.h)**: Archivo con declaraciones compartidas
- **Include guards**: Previenen inclusión múltiple (`#ifndef`, `#define`, `#endif`)
- **Inline functions**: Funciones pequeñas optimizadas por el compilador
- **Typedef struct**: Define tipos de datos personalizados
- **Formato binario**: Representación en bytes de estructuras
- **Parsing**: Convertir texto a datos estructurados

## Extensiones Posibles

- Añadir más campos a la estructura Libro
- Crear utilidades para otros tipos de datos
- Implementar compresión de archivos binarios
- Añadir checksums para verificar integridad
- Crear herramientas de migración entre versiones
- Implementar índices para búsquedas rápidas

## Debugging

Para verificar el flujo completo:

```bash
# 1. Crear archivo de texto
./crear_txt
cat libros_biblioteca.txt  # Ver contenido

# 2. Convertir a binario
./text_to_bin
ls -lh libros_biblioteca.bin  # Ver tamaño

# 3. Verificar binario
./ver_bin  # Debe mostrar los mismos datos

# 4. Consultar datos
./listar  # Filtrar prestados
```

## Notas Importantes

- El archivo common.h debe estar en el path de include al compilar
- Los programas asumen que los archivos de datos están en el directorio actual
- Es recomendable usar paths absolutos o relativos consistentes
- Validar siempre que los archivos existan antes de operaciones de lectura
