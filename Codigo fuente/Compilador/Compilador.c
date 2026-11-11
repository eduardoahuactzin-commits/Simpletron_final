
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

int memoria[MAX];
int cantidadCodigo = 0;
int cantidadVariables = 0;
int cantidadConstantes = 0;

int variables[26];
int direcciones[MAX];
int valores[MAX];

char nombresConstantes[MAX][30];

int buscarConstante(char texto[])
{
    int i;

    for (i = 0; i < cantidadConstantes; i++)
    {
        if (strcmp(nombresConstantes[i], texto) == 0)
            return i;
    }

    strcpy(nombresConstantes[cantidadConstantes], texto);
    cantidadConstantes++;

    return cantidadConstantes - 1;
}

int obtenerVariable(char letra)
{
    int posicion = letra - 'a';

    if (posicion < 0 || posicion >= 26)
        return -1;

    if (variables[posicion] == -1)
    {
        variables[posicion] = cantidadVariables;
        cantidadVariables++;
    }

    return variables[posicion];
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
        if (!isdigit(texto[i]))
            return 0;
    }

    return 1;
}

int agregarCodigo(int operacion, int operando)
{
    if (cantidadCodigo >= MAX)
        return 0;

    memoria[cantidadCodigo] = operacion;
    direcciones[cantidadCodigo] = operando;
    cantidadCodigo++;

    return 1;
}

int main()
{
    FILE *entrada;
    FILE *salida;

    char nombre[200];
    char linea[200];
    char instruccion[30];
    char dato1[30];
    char dato2[30];

    int numeroLinea;
    int resultado;
    int variable;
    int constante;
    int i;
    int operacion;
    int direccion;
    int terminado = 0;

    for (i = 0; i < 26; i++)
        variables[i] = -1;

    printf("=== COMPILADOR SIMPLE A SML ===\n\n");
    printf("Archivo Simple: ");
    scanf(" %199[^\n]", nombre);

    entrada = fopen(nombre, "r");

    if (entrada == NULL)
    {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    while (fgets(linea, sizeof(linea), entrada) != NULL)
    {
        if (sscanf(linea, "%d %29s", &numeroLinea, instruccion) != 2)
            continue;

        if (strcmp(instruccion, "rem") == 0)
            continue;

        if (strcmp(instruccion, "input") == 0)
        {
            if (sscanf(linea, "%d %29s %29s",
                       &numeroLinea, instruccion, dato1) != 3)
            {
                printf("Error en input.\n");
                fclose(entrada);
                return 1;
            }

            variable = obtenerVariable(dato1[0]);

            if (variable < 0 || strlen(dato1) != 1)
            {
                printf("Variable invalida.\n");
                fclose(entrada);
                return 1;
            }

            agregarCodigo(10, variable);
        }
        else if (strcmp(instruccion, "print") == 0)
        {
            if (sscanf(linea, "%d %29s %29s",
                       &numeroLinea, instruccion, dato1) != 3)
            {
                printf("Error en print.\n");
                fclose(entrada);
                return 1;
            }

            if (esNumero(dato1))
            {
                constante = buscarConstante(dato1);
                agregarCodigo(11, -constante - 1);
            }
            else
            {
                variable = obtenerVariable(dato1[0]);

                if (variable < 0 || strlen(dato1) != 1)
                {
                    printf("Variable invalida.\n");
                    fclose(entrada);
                    return 1;
                }

                agregarCodigo(11, variable);
            }
        }
        else if (strcmp(instruccion, "let") == 0)
        {
            resultado = sscanf(linea, "%d %29s %29s = %29s %29s %29s",
                               &numeroLinea, instruccion,
                               dato1, dato2, dato2, instruccion, dato2);

            if (resultado < 4)
            {
                printf("Error en let.\n");
                fclose(entrada);
                return 1;
            }

            if (sscanf(linea, "%d let %29s = %29s %29s %29s",
                       &numeroLinea, dato1, dato2, instruccion, nombre) < 3)
            {
                printf("Error en asignacion.\n");
                fclose(entrada);
                return 1;
            }

            variable = obtenerVariable(dato1[0]);

            if (variable < 0)
            {
                printf("Variable invalida.\n");
                fclose(entrada);
                return 1;
            }

            if (esNumero(dato2))
            {
                constante = buscarConstante(dato2);
                agregarCodigo(20, -constante - 1);
            }
            else
            {
                i = obtenerVariable(dato2[0]);

                if (i < 0)
                {
                    printf("Operando invalido.\n");
                    fclose(entrada);
                    return 1;
                }

                agregarCodigo(20, i);
            }

            if (sscanf(linea, "%d let %29s = %29s %29s %29s",
                       &numeroLinea, dato1, dato2, instruccion, nombre) == 5)
            {
                if (strcmp(instruccion, "+") == 0) operacion = 30;
                else if (strcmp(instruccion, "-") == 0) operacion = 31;
                else if (strcmp(instruccion, "/") == 0) operacion = 32;
                else if (strcmp(instruccion, "*") == 0) operacion = 33;
                else if (strcmp(instruccion, "%") == 0) operacion = 34;
                else if (strcmp(instruccion, "^") == 0) operacion = 35;
                else
                {
                    printf("Operador invalido.\n");
                    fclose(entrada);
                    return 1;
                }

                if (esNumero(nombre))
                {
                    constante = buscarConstante(nombre);
                    agregarCodigo(operacion, -constante - 1);
                }
                else
                {
                    i = obtenerVariable(nombre[0]);

                    if (i < 0)
                    {
                        printf("Operando invalido.\n");
                        fclose(entrada);
                        return 1;
                    }

                    agregarCodigo(operacion, i);
                }
            }

            agregarCodigo(21, variable);
        }
        else if (strcmp(instruccion, "end") == 0)
        {
            agregarCodigo(43, 0);
            terminado = 1;
            break;
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

    if (cantidadCodigo + cantidadVariables + cantidadConstantes > MAX)
    {
        printf("Error: memoria insuficiente.\n");
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
        if (direcciones[i] < 0)
            direccion = cantidadCodigo + cantidadVariables +
                        (-direcciones[i] - 1);
        else
            direccion = cantidadCodigo + direcciones[i];

        if (memoria[i] == 43)
            direccion = 0;

        resultado = memoria[i] * 100 + direccion;

        fprintf(salida, "%04d\n", resultado);
        printf("%04d\n", resultado);
    }

    for (i = 0; i < cantidadVariables; i++)
        fprintf(salida, "0000\n");

    for (i = 0; i < cantidadConstantes; i++)
        fprintf(salida, "%04d\n", atoi(nombresConstantes[i]));

    fclose(salida);

    printf("\nCompilacion terminada.\n");
    printf("Archivo: prueba_compilada.sml\n");

    return 0;
}

