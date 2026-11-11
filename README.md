# Proyecto Simpletron

## 1. Descripción del proyecto

Este proyecto consiste en desarrollar un simulador Simpletron y un compilador para un lenguaje de programación sencillo llamado Simple.

El simulador permite ejecutar instrucciones de lenguaje máquina (SML), mientras que el compilador transforma programas escritos en Simple a instrucciones que puede ejecutar el simulador.

## 2. Objetivo

Comprender el funcionamiento de un procesador básico, el manejo de memoria, los registros, las instrucciones y el proceso de compilación de un lenguaje de programación.

## 3. Información general

- **Lenguaje de programación:** C
- **Sistema de control de versiones:** Git y GitHub
- **Entorno de desarrollo:** GCC / MinGW
- **Lenguaje fuente:** Simple
- **Lenguaje máquina:** SML

## 4. Integrantes

- Eduardo Ahuactzin Galicia
- Pedro Cruz Hernández

## 5. Estructura del proyecto

```text
Simpletron-final/
│
├── Codigo fuente/
│   ├── Simpletron/
│   │   └── Simpletron.c
│   │
│   ├── Compilador/
│   │   └── Compilador.c
│   │
│   └── Expresiones/
│       ├── Expresiones.c
│       ├── Expresiones.h
│       └── PruebaExpresiones.c
│
├── Programas de prueba/
│   ├── Simple/
│   │   ├── prueba.simple
│   │   └── prueba_goto.simple
│   │
│   └── SML/
│       ├── prueba.sml
│       └── prueba_compilada.sml
│
└── README.md
```

## 6. Características del simulador Simpletron

- Memoria de 100 posiciones.
- Registro acumulador.
- Contador de instrucciones.
- Lectura y escritura de datos.
- Operaciones aritméticas.
- Instrucciones de salto.
- Ejecución de programas desde archivos SML.
- Validación de instrucciones y direcciones de memoria.
- Detección de división entre cero.
- Detección de errores durante la ejecución.

## 7. Instrucciones SML implementadas

| Código | Instrucción | Descripción |
|---|---|---|
| 10 | READ | Leer un dato |
| 11 | WRITE | Mostrar un dato |
| 20 | LOAD | Cargar un valor |
| 21 | STORE | Guardar un valor |
| 30 | ADD | Sumar |
| 31 | SUBTRACT | Restar |
| 32 | DIVIDE | Dividir |
| 33 | MULTIPLY | Multiplicar |
| 34 | MOD | Obtener módulo |
| 35 | POWER | Elevar a una potencia |
| 40 | BRANCH | Salto incondicional |
| 41 | BRANCHNEG | Salto si es negativo |
| 42 | BRANCHZERO | Salto si es cero |
| 43 | HALT | Finalizar programa |

## 8. Características del lenguaje Simple

El lenguaje Simple permite utilizar:

- `rem`: comentarios.
- `input`: entrada de datos.
- `let`: asignación de variables.
- `print`: mostrar resultados.
- `goto`: saltos entre líneas.
- `if`: condiciones.
- `end`: finalizar el programa.

También permite utilizar expresiones aritméticas con:

- Suma (+)
- Resta (-)
- Multiplicación (*)
- División (/)
- Módulo (%)
- Potencia (^)
- Paréntesis para agrupar operaciones.

## 9. Evaluador de expresiones

Se desarrolló un módulo independiente para procesar expresiones matemáticas.

Sus funciones principales son:

- Validar expresiones.
- Comprobar paréntesis.
- Convertir expresiones infijas a postfijas.
- Evaluar expresiones postfijas.
- Respetar la prioridad de los operadores.
- Manejar números de varios dígitos y variables.

Este módulo también se encuentra integrado en el compilador.

## 10. Ejemplo de programa Simple

```text
10 input a
20 let b = (a + 3) * 2
30 print b
40 end
```

Si el usuario introduce el número 5, el resultado esperado es:

```text
16
```

## 11. Compilación y ejecución

Los siguientes comandos se ejecutan desde la carpeta principal del proyecto en Git Bash.

### Compilar el simulador

```bash
gcc "Codigo fuente/Simpletron/Simpletron.c" -o "Codigo fuente/Simpletron/Simpletron.exe"
```

### Compilar el compilador

```bash
gcc "Codigo fuente/Compilador/Compilador.c" "Codigo fuente/Expresiones/Expresiones.c" -o "Codigo fuente/Compilador/Compilador.exe"
```

### Ejecutar el compilador

```bash
"./Codigo fuente/Compilador/Compilador.exe"
```

Cuando solicite el archivo Simple, introducir:

```text
Programas de prueba/Simple/prueba.simple
```

El compilador genera el archivo:

```text
Programas de prueba/SML/prueba_compilada.sml
```

### Ejecutar el simulador

```bash
"./Codigo fuente/Simpletron/Simpletron.exe"
```

Introducir el archivo SML generado:

```text
Programas de prueba/SML/prueba_compilada.sml
```

El simulador solicitará los datos necesarios y ejecutará el programa.

## 12. Pruebas realizadas

Se realizaron pruebas de:

- Operaciones aritméticas.
- Instrucciones de lectura y escritura.
- Instrucciones de salto.
- Condiciones.
- División entre cero.
- Direcciones de memoria inválidas.
- Módulo y potencia.
- Evaluación de expresiones.
- Compilación de programas Simple a SML.
- Ejecución de programas compilados.

## 13. Control de versiones

El proyecto utiliza Git y GitHub para administrar los cambios del código fuente.

Repositorio:

https://github.com/eduardoahuactzin-commits/Simpletron_final

## 14. Estado del proyecto

Proyecto desarrollado en lenguaje C. Incluye un simulador Simpletron, un evaluador de expresiones y un compilador del lenguaje Simple a SML.

## 15. Rama de mejoras

La rama `mejoras` se utiliza para trabajar en extensiones y mejoras
del simulador Simpletron sin modificar directamente la versión principal.

Entre las extensiones implementadas se encuentran:

- Operación de módulo.
- Operación de potencia.
- Validaciones de errores.
- Detección de división entre cero.
- Validación de direcciones de memoria.