
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../Expresiones/Expresiones.h"

#define MAX 100
#define MAX_LINEA 200

int memoria[MAX];
int operandos[MAX];
int tipoOperandos[MAX];
int cantidadCodigo = 0;

int variables[26];
int cantidadVariables = 0;

int valoresConstantes[MAX];
int cantidadConstantes = 0;

int cantidadTemporales = 0;

int numerosLineas[MAX];
int posicionesLineas[MAX];
int cantidadLineas = 0;

int saltosPendientes[MAX];
int lineasPendientes[MAX];
int cantidadPendientes = 0;

/* Tipos de operandos:
   0 = variable
   1 = constante
   2 = temporal
   3 = direccion absoluta de salto
*/

int buscarVariable(char letra)
{
    int posicion = letra - 'a';

    if (posicion < 0 || posicion >= 26)
        return -1;

    if (variables[posicion] == -1)
    {
        if (cantidadVariables >= MAX)
            return -1;

        variables[posicion] = cantidadVariables;
        cantidadVariables++;
    }

    return variables[posicion];
}

int buscarConstante(int valor)
{
    int i;

    for (i = 0; i < cantidadConstantes; i++)
    {
        if (valoresConstantes[i] == valor)
            return i;
    }

    if (cantidadConstantes >= MAX)
        return -1;

    valoresConstantes[cantidadConstantes] = valor;
    cantidadConstantes++;

    return cantidadConstantes - 1;
}

int esNumero(char texto[])
{
    int i = 0;

    if (texto[0] == '-')
        i++;

    if (texto[i] == '\0')
        return 0;

    for (; texto[i] != '\0'; i++)
    {
        if (!isdigit((unsigned char)texto[i]))
            return 0;
    }

    return 1;
}

int agregarCodigo(int operacion, int operando)
{
    if (cantidadCodigo >= MAX)
        return 0;

    memoria[cantidadCodigo] = operacion;
    operandos[cantidadCodigo] = operando;
    tipoOperandos[cantidadCodigo] = 3;

    cantidadCodigo++;

    return 1;
}

int agregarMemoria(int operacion, int tipo, int indice)
{
    if (cantidadCodigo >= MAX)
        return 0;

    memoria[cantidadCodigo] = operacion;
    operandos[cantidadCodigo] = indice;
    tipoOperandos[cantidadCodigo] = tipo;

    cantidadCodigo++;

    return 1;
}

int buscarLinea(int numero)
{
    int i;

    for (i = 0; i < cantidadLineas; i++)
    {
        if (numerosLineas[i] == numero)
            return posicionesLineas[i];
    }

    return -1;
}

int registrarLinea(int numero)
{
    if (cantidadLineas >= MAX)
        return 0;

    if (cantidadLineas > 0 &&
        numero <= numerosLineas[cantidadLineas - 1])
        return 0;

    numerosLineas[cantidadLineas] = numero;
    posicionesLineas[cantidadLineas] = cantidadCodigo;
    cantidadLineas++;

    return 1;
}

int agregarSaltoPendiente(int posicion, int linea)
{
    if (cantidadPendientes >= MAX)
        return 0;

    saltosPendientes[cantidadPendientes] = posicion;
    lineasPendientes[cantidadPendientes] = linea;
    cantidadPendientes++;

    return 1;
}

int agregarOperando(int operacion, char texto[])
{
    int variable;
    int constante;
    int valor;

    if (esNumero(texto))
    {
        valor = atoi(texto);

        if (valor < -9999 || valor > 9999)
            return 0;

        constante = buscarConstante(valor);

        if (constante < 0)
            return 0;

        return agregarMemoria(operacion, 1, constante);
    }

    if (strlen(texto) != 1)
        return 0;

    variable = buscarVariable(texto[0]);

    if (variable < 0)
        return 0;

    return agregarMemoria(operacion, 0, variable);
}

int esComparador(char texto[])
{
    if (strcmp(texto, "<") == 0) return 1;
    if (strcmp(texto, "<=") == 0) return 1;
    if (strcmp(texto, ">") == 0) return 1;
    if (strcmp(texto, ">=") == 0) return 1;
    if (strcmp(texto, "==") == 0) return 1;
    if (strcmp(texto, "!=") == 0) return 1;

    return 0;
}

int agregarIf(char izquierda[], char comparador[],
              char derecha[], int lineaDestino)
{
    int posicionSalto;
    int posicionOmitir;

    if (strcmp(comparador, ">") == 0)
    {
        if (!agregarOperando(20, derecha))
            return 0;

        if (!agregarOperando(31, izquierda))
            return 0;

        posicionSalto = cantidadCodigo;

        if (!agregarCodigo(41, 0))
            return 0;

        return agregarSaltoPendiente(posicionSalto, lineaDestino);
    }

    if (!agregarOperando(20, izquierda))
        return 0;

    if (!agregarOperando(31, derecha))
        return 0;

    if (strcmp(comparador, "<") == 0)
    {
        posicionSalto = cantidadCodigo;

        if (!agregarCodigo(41, 0))
            return 0;

        return agregarSaltoPendiente(posicionSalto, lineaDestino);
    }

    if (strcmp(comparador, "==") == 0)
    {
        posicionSalto = cantidadCodigo;

        if (!agregarCodigo(42, 0))
            return 0;

        return agregarSaltoPendiente(posicionSalto, lineaDestino);
    }

    if (strcmp(comparador, "<=") == 0)
    {
        posicionSalto = cantidadCodigo;

        if (!agregarCodigo(41, 0))
            return 0;

        if (!agregarSaltoPendiente(posicionSalto, lineaDestino))
            return 0;

        posicionSalto = cantidadCodigo;

        if (!agregarCodigo(42, 0))
            return 0;

        return agregarSaltoPendiente(posicionSalto, lineaDestino);
    }

    if (strcmp(comparador, ">=") == 0)
    {
        posicionOmitir = cantidadCodigo;

        if (!agregarCodigo(41, 0))
            return 0;

        posicionSalto = cantidadCodigo;

        if (!agregarCodigo(40, 0))
            return 0;

        if (!agregarSaltoPendiente(posicionSalto, lineaDestino))
            return 0;

        operandos[posicionOmitir] = cantidadCodigo;

        return 1;
    }

    if (strcmp(comparador, "!=") == 0)
    {
        posicionOmitir = cantidadCodigo;

        if (!agregarCodigo(42, 0))
            return 0;

        posicionSalto = cantidadCodigo;

        if (!agregarCodigo(40, 0))
            return 0;

        if (!agregarSaltoPendiente(posicionSalto, lineaDestino))
            return 0;

        operandos[posicionOmitir] = cantidadCodigo;

        return 1;
    }

    return 0;
}

/* Convierte una expresion postfija en instrucciones SML.
   Cada operacion intermedia utiliza una celda temporal. */

int compilarExpresion(char texto[], int variableDestino)
{
    int pilaTipo[MAX_EXPRESIONES];
    int pilaIndice[MAX_EXPRESIONES];
    int tope = -1;

    int i;
    int valor;
    int constante;
    int variable;
    int tipoIzquierda;
    int tipoDerecha;
    int indiceIzquierda;
    int indiceDerecha;
    int temporal;
    int operacion;

    if (!convertirPostfija(texto))
        return 0;

    for (i = 0; i < cantidadPostfija; i++)
    {
        if (isdigit((unsigned char)postfija[i][0]))
        {
            valor = atoi(postfija[i]);

            if (valor > 9999)
                return 0;

            constante = buscarConstante(valor);

            if (constante < 0 || tope >= MAX_EXPRESIONES - 1)
                return 0;

            tope++;
            pilaTipo[tope] = 1;
            pilaIndice[tope] = constante;
        }
        else if (postfija[i][0] >= 'a' &&
                 postfija[i][0] <= 'z')
        {
            variable = buscarVariable(postfija[i][0]);

            if (variable < 0 || tope >= MAX_EXPRESIONES - 1)
                return 0;

            tope++;
            pilaTipo[tope] = 0;
            pilaIndice[tope] = variable;
        }
        else
        {
            if (tope < 1)
                return 0;

            tipoDerecha = pilaTipo[tope];
            indiceDerecha = pilaIndice[tope];
            tope--;

            tipoIzquierda = pilaTipo[tope];
            indiceIzquierda = pilaIndice[tope];
            tope--;

            if (postfija[i][0] == '+') operacion = 30;
            else if (postfija[i][0] == '-') operacion = 31;
            else if (postfija[i][0] == '/') operacion = 32;
            else if (postfija[i][0] == '*') operacion = 33;
            else if (postfija[i][0] == '%') operacion = 34;
            else if (postfija[i][0] == '^') operacion = 35;
            else return 0;

            if (cantidadTemporales >= MAX)
                return 0;

            temporal = cantidadTemporales;
            cantidadTemporales++;

            if (!agregarMemoria(20, tipoIzquierda, indiceIzquierda))
                return 0;

            if (!agregarMemoria(operacion, tipoDerecha, indiceDerecha))
                return 0;

            if (!agregarMemoria(21, 2, temporal))
                return 0;

            tope++;

            pilaTipo[tope] = 2;
            pilaIndice[tope] = temporal;
        }
    }

    if (tope != 0)
        return 0;

    if (!agregarMemoria(20, pilaTipo[tope], pilaIndice[tope]))
        return 0;

    if (!agregarMemoria(21, 0, variableDestino))
        return 0;

    return 1;
}

int main()
{
    FILE *entrada;
    FILE *salida;

    char nombreArchivo[200];
    char lineaTexto[MAX_LINEA];
    char instruccion[30];
    char dato1[30];
    char dato2[30];
    char operador[10];
    char expresion[MAX_LINEA];
    char letraDestino;

    char *igual;
    char *inicioExpresion;

    int numeroLinea;
    int numeroDestino;
    int variable;
    int operacion;
    int posicion;
    int direccion;
    int i;
    int terminado = 0;
    int temporal;
    int cantidadDatos;

    for (i = 0; i < 26; i++)
        variables[i] = -1;

    printf("=== COMPILADOR SIMPLE A SML ===\n\n");
    printf("Archivo Simple: ");

    if (scanf(" %199[^\n]", nombreArchivo) != 1)
    {
        printf("Error al leer el nombre del archivo.\n");
        return 1;
    }

    entrada = fopen(nombreArchivo, "r");

    if (entrada == NULL)
    {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    while (fgets(lineaTexto, sizeof(lineaTexto), entrada) != NULL)
    {
        dato1[0] = '\0';
        dato2[0] = '\0';
        operador[0] = '\0';

        if (sscanf(lineaTexto, "%d %29s",
                   &numeroLinea, instruccion) != 2)
            continue;

        if (terminado)
        {
            printf("Error: hay instrucciones despues de end.\n");
            fclose(entrada);
            return 1;
        }

        if (!registrarLinea(numeroLinea))
        {
            printf("Error: numero de linea repetido o desordenado.\n");
            fclose(entrada);
            return 1;
        }

        if (strcmp(instruccion, "rem") == 0)
            continue;

        if (strcmp(instruccion, "input") == 0)
        {
            if (sscanf(lineaTexto, "%d %29s %29s",
                       &numeroLinea, instruccion, dato1) != 3 ||
                strlen(dato1) != 1 ||
                dato1[0] < 'a' || dato1[0] > 'z')
            {
                printf("Error en input.\n");
                fclose(entrada);
                return 1;
            }

            variable = buscarVariable(dato1[0]);

            if (variable < 0 ||
                !agregarMemoria(10, 0, variable))
            {
                printf("Variable invalida o memoria insuficiente.\n");
                fclose(entrada);
                return 1;
            }
        }
        else if (strcmp(instruccion, "print") == 0)
        {
            if (sscanf(lineaTexto, "%d %29s %29s",
                       &numeroLinea, instruccion, dato1) != 3)
            {
                printf("Error en print.\n");
                fclose(entrada);
                return 1;
            }

            if (!agregarOperando(11, dato1))
            {
                printf("Error en el valor de print.\n");
                fclose(entrada);
                return 1;
            }
        }
        else if (strcmp(instruccion, "let") == 0)
        {
            igual = strchr(lineaTexto, '=');

            if (igual == NULL ||
                sscanf(lineaTexto, "%d %29s %c",
                       &numeroLinea, instruccion,
                       &letraDestino) != 3 ||
                strlen(instruccion) == 0 ||
                strcmp(instruccion, "let") != 0 ||
                letraDestino < 'a' || letraDestino > 'z')
            {
                printf("Error en let.\n");
                fclose(entrada);
                return 1;
            }

            variable = buscarVariable(letraDestino);

            if (variable < 0)
            {
                printf("Variable de asignacion invalida.\n");
                fclose(entrada);
                return 1;
            }

            inicioExpresion = igual + 1;

            while (isspace((unsigned char)*inicioExpresion))
                inicioExpresion++;

            strncpy(expresion, inicioExpresion, MAX_LINEA - 1);
            expresion[MAX_LINEA - 1] = '\0';

            expresion[strcspn(expresion, "\r\n")] = '\0';

            if (!compilarExpresion(expresion, variable))
            {
                printf("Error en la expresion de let.\n");
                fclose(entrada);
                return 1;
            }
        }
        else if (strcmp(instruccion, "goto") == 0)
        {
            if (sscanf(lineaTexto, "%d %29s %d",
                       &numeroLinea, instruccion,
                       &numeroDestino) != 3)
            {
                printf("Error en goto.\n");
                fclose(entrada);
                return 1;
            }

            posicion = cantidadCodigo;

            if (!agregarCodigo(40, 0) ||
                !agregarSaltoPendiente(posicion, numeroDestino))
            {
                printf("Memoria insuficiente.\n");
                fclose(entrada);
                return 1;
            }
        }
        else if (strcmp(instruccion, "if") == 0)
        {
            if (sscanf(lineaTexto,
                       "%d if %29s %9s %29s goto %d",
                       &numeroLinea, dato1, operador,
                       dato2, &numeroDestino) != 5)
            {
                printf("Error en if.\n");
                fclose(entrada);
                return 1;
            }

            if (!esComparador(operador))
            {
                printf("Comparador invalido.\n");
                fclose(entrada);
                return 1;
            }

            if (!agregarIf(dato1, operador, dato2, numeroDestino))
            {
                printf("Error al generar la condicion if.\n");
                fclose(entrada);
                return 1;
            }
        }
        else if (strcmp(instruccion, "end") == 0)
        {
            if (!agregarCodigo(43, 0))
            {
                printf("Memoria insuficiente.\n");
                fclose(entrada);
                return 1;
            }

            terminado = 1;
        }
        else
        {
            printf("Instruccion no reconocida: %s\n", instruccion);
            fclose(entrada);
            return 1;
        }
    }

    fclose(entrada);

    if (!terminado)
    {
        printf("Error: falta end.\n");
        return 1;
    }

    for (i = 0; i < cantidadPendientes; i++)
    {
        posicion = buscarLinea(lineasPendientes[i]);

        if (posicion < 0)
        {
            printf("Error: no existe la linea %d.\n",
                   lineasPendientes[i]);
            return 1;
        }

        operandos[saltosPendientes[i]] = posicion;
    }

    cantidadDatos = cantidadVariables +
                    cantidadConstantes +
                    cantidadTemporales;

    if (cantidadCodigo + cantidadDatos > MAX)
    {
        printf("Error: memoria insuficiente para el programa SML.\n");
        return 1;
    }

    salida = fopen("Programas de prueba/SML/prueba_compilada.sml", "w");

    if (salida == NULL)
    {
        printf("Error al crear el archivo SML.\n");
        return 1;
    }

    printf("\n=== CODIGO SML GENERADO ===\n");

    for (i = 0; i < cantidadCodigo; i++)
    {
        if (memoria[i] == 40 ||
            memoria[i] == 41 ||
            memoria[i] == 42)
        {
            direccion = operandos[i];
        }
        else if (memoria[i] == 43)
        {
            direccion = 0;
        }
        else if (tipoOperandos[i] == 0)
        {
            direccion = cantidadCodigo + operandos[i];
        }
        else if (tipoOperandos[i] == 1)
        {
            direccion = cantidadCodigo +
                        cantidadVariables +
                        operandos[i];
        }
        else if (tipoOperandos[i] == 2)
        {
            direccion = cantidadCodigo +
                        cantidadVariables +
                        cantidadConstantes +
                        operandos[i];
        }
        else
        {
            direccion = operandos[i];
        }

        if (direccion < 0 || direccion > 99)
        {
            printf("Error: direccion fuera de rango.\n");
            fclose(salida);
            return 1;
        }

        temporal = memoria[i] * 100 + direccion;

        fprintf(salida, "%04d\n", temporal);
        printf("%04d\n", temporal);
    }

    /* Espacio para variables */
    for (i = 0; i < cantidadVariables; i++)
        fprintf(salida, "0000\n");

    /* Espacio para constantes */
    for (i = 0; i < cantidadConstantes; i++)
        fprintf(salida, "%+05d\n", valoresConstantes[i]);

    /* Espacio para resultados temporales */
    for (i = 0; i < cantidadTemporales; i++)
        fprintf(salida, "0000\n");

    fclose(salida);

    printf("\nCompilacion terminada.\n");
    printf("Archivo: prueba_compilada.sml\n");

    return 0;
}

