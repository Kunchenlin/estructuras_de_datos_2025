# Sistemas de Gestión

## Descripción

Esta carpeta contiene programas completos de sistemas de gestión que integran múltiples estructuras de datos y operaciones. Son aplicaciones prácticas que demuestran cómo usar las estructuras de datos en proyectos reales.

## Programas en esta carpeta

### gestor_drones.c
Sistema completo de gestión de drones:
- Control de flota de drones
- Registro de misiones y estado
- Operaciones CRUD (Create, Read, Update, Delete)
- Seguimiento de ubicación y batería

### gestor_trenes.c
Sistema de gestión de trenes:
- Control de rutas y horarios
- Gestión de vagones y capacidad
- Estado de trenes en tiempo real
- Reservas y asignaciones

### gestor_vuelo.c (Versión 1)
Sistema de gestión de vuelos:
- Registro de vuelos y pasajeros
- Control de horarios de salida/llegada
- Gestión de capacidad y reservas
- Consultas y modificaciones

### gestor_vuelo_v2.c (Versión 2)
Versión mejorada del gestor de vuelos:
- Funcionalidades extendidas
- Mejor organización del código
- Más opciones de consulta
- Optimizaciones implementadas

### metro.c (Versión 1)
Sistema de gestión de red de metro:
- Control de líneas y estaciones
- Horarios y frecuencias
- Cálculo de rutas
- Gestión de trenes

### metro_v2.c (Versión 2)
Versión mejorada del sistema de metro:
- Algoritmos de búsqueda de rutas optimizados
- Mejor interfaz de usuario
- Más funcionalidades administrativas
- Gestión de incidencias

## Características Comunes

Todos estos sistemas incluyen:
- Menús interactivos para el usuario
- Validación de entrada de datos
- Operaciones CRUD completas
- Almacenamiento en memoria (y/o archivos)
- Búsqueda y filtrado de información
- Generación de informes

## Estructuras de Datos Utilizadas

Estos programas integran:
- **Arrays/Vectores**: Para almacenar colecciones de elementos
- **Estructuras (struct)**: Para representar entidades complejas
- **Listas enlazadas**: Para gestión dinámica de elementos
- **Colas**: Para gestión de turnos y procesamiento secuencial
- **Pilas**: Para historial de operaciones
- **Algoritmos de búsqueda**: Para localizar información
- **Algoritmos de ordenación**: Para presentar datos organizados

## Compilación

```bash
# Gestor de drones
gcc -o gestor_drones gestor_drones.c -Wall -Wextra

# Gestor de trenes
gcc -o gestor_trenes gestor_trenes.c -Wall -Wextra

# Gestor de vuelos v1
gcc -o gestor_vuelo gestor_vuelo.c -Wall -Wextra

# Gestor de vuelos v2
gcc -o gestor_vuelo_v2 gestor_vuelo_v2.c -Wall -Wextra

# Sistema de metro v1
gcc -o metro metro.c -Wall -Wextra

# Sistema de metro v2
gcc -o metro_v2 metro_v2.c -Wall -Wextra
```

## Ejecución

```bash
./gestor_drones
./gestor_trenes
./gestor_vuelo
./gestor_vuelo_v2
./metro
./metro_v2
```

## Funcionalidades Típicas

### Operaciones CRUD
- **Create**: Agregar nuevos registros (vuelos, trenes, drones)
- **Read**: Consultar y listar información
- **Update**: Modificar datos existentes
- **Delete**: Eliminar registros

### Consultas
- Búsqueda por ID o nombre
- Filtrado por criterios (fecha, estado, ubicación)
- Ordenación de resultados
- Estadísticas e informes

### Validaciones
- Formato de datos correctos
- Valores dentro de rangos válidos
- Prevención de duplicados
- Verificación de existencia antes de operaciones

## Casos de Uso Educativos

Estos programas son ideales para:
1. **Aprender integración de conceptos**: Combinan múltiples estructuras de datos
2. **Desarrollo de proyectos**: Ejemplos de aplicaciones completas
3. **Práctica de diseño**: Estructura modular y funciones bien definidas
4. **Debugging**: Código complejo para practicar depuración
5. **Refactoring**: Comparar versiones (v1 vs v2) para ver mejoras

## Diferencias entre Versiones

Las versiones 2.0 generalmente incluyen:
- Código más limpio y organizado
- Mejor manejo de errores
- Funcionalidades adicionales
- Optimizaciones de rendimiento
- Mejor documentación
- Interfaces más intuitivas

## Conceptos Avanzados

- **Modularización**: Separación de funcionalidades en funciones
- **Abstracción**: Ocultamiento de detalles de implementación
- **Validación robusta**: Manejo de entradas incorrectas
- **Persistencia**: Guardado y carga de datos (algunos programas)
- **Menús interactivos**: Navegación por opciones
- **Estado del sistema**: Manejo de estados complejos

## Extensiones Posibles

Estos programas pueden extenderse con:
- Persistencia en archivos o bases de datos
- Interfaces gráficas (GUI)
- Conexiones de red para sistemas distribuidos
- Generación de reportes en PDF/HTML
- Autenticación y permisos de usuario
- Logging de operaciones
- Backup y recuperación de datos

## Consejos de Estudio

1. **Ejecuta primero**: Interactúa con el programa para entender qué hace
2. **Lee el código**: Identifica las estructuras de datos usadas
3. **Diagrama de flujo**: Dibuja cómo fluye la información
4. **Modifica gradualmente**: Añade pequeñas funcionalidades
5. **Compara versiones**: Estudia las diferencias entre v1 y v2
6. **Debugging**: Usa GDB o printf para seguir la ejecución
