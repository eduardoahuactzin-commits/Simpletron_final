#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Expresiones.h"

char postfija[MAX][30];
char operadores[MAX];
int cantidadPostfija = 0;

int prioridad(char operador)
{
    if (operador == '^')
        return 3;

    if (operador == '*' || operador == '/')
        return 2;

    if (operador == '+' || operador == '-')
        return 1;

    return 0;
}

int esOperador(char caracter)
{
    return caracter == '+' || caracter == '-' ||
           caracter == '*' || caracter == '/' ||
           caracter == '^';
}

int validarExpresion(char texto[])
{
    int i;
    int parentesis = 0;
    int esperaNumero = 1;

    for (i = 0; texto[i] != '\0'; i++)
    {
        if (isspace(texto[i]))
            continue;

        if (isdigit(texto[i]))
        {
            if (!esperaNumero)
                return 0;

            while (isdigit(texto[i + 1]))
                i++;

            esperaNumero = 0;
        }
        else if (texto[i] == '(')
        {
            if (!esperaNumero)
                return 0;

            parentesis++;
        }
        else if (texto[i] == ')')
        {
            if (esperaNumero || parentesis <= 0)
                return 0;

            parentesis--;
            esperaNumero = 0;
        }
        else if (esOperador(texto[i]))
        {
            if (esperaNumero)
                return 0;

            esperaNumero = 1;
        }
        else
        {
            return 0;
        }
    }

    return parentesis == 0 && !esperaNumero;
}

int convertirPostfija(char texto[])
{
    int i;
    int j = 0;
    int k;
    int longitud = strlen(texto);

    cantidadPostfija = 0;
    k = 0;

    for (i = 0; i < longitud; i++)
    {
        if (isspace(texto[i]))
            continue;

        if (isdigit(texto[i]))
        {
            j = 0;

            while (isdigit(texto[i]))
            {
                postfija[cantidadPostfija][j] = texto[i];
                j++;
                i++;
            }

            postfija[cantidadPostfija][j] = '\0';
            cantidadPostfija++;
            i--;
        }
        else if (texto[i] == '(')
        {
            operadores[k++] = texto[i];
        }
        else if (texto[i] == ')')
        {
            while (k > 0 && operadores[k - 1] != '(')
            {
                postfija[cantidadPostfija][0] = operadores[--k];
                postfija[cantidadPostfija][1] = '\0';
                cantidadPostfija++;
            }

            if (k > 0)
                k--;
        }
        else if (esOperador(texto[i]))
        {
            while (k > 0 &&
                   operadores[k - 1] != '(' &&
                   (prioridad(operadores[k - 1]) > prioridad(texto[i]) ||
                   (prioridad(operadores[k - 1]) == prioridad(texto[i]) &&
                   texto[i] != '^')))
            {
                postfija[cantidadPostfija][0] = operadores[--k];
                postfija[cantidadPostfija][1] = '\0';
                cantidadPostfija++;
            }

            operadores[k++] = texto[i];
        }
    }

    while (k > 0)
    {
        postfija[cantidadPostfija][0] = operadores[--k];
        postfija[cantidadPostfija][1] = '\0';
        cantidadPostfija++;
    }

    return 1;
}

int evaluarPostfija(int *resultado)
{
    int pila[MAX];
    int tope = -1;
    int i;
    int a;
    int b;
    int j;
    int potencia;
    int temporal;

    for (i = 0; i < cantidadPostfija; i++)
    {
        if (isdigit(postfija[i][0]))
        {
            pila[++tope] = atoi(postfija[i]);
        }
        else
        {
            if (tope < 1)
                return 0;

            b = pila[tope--];
            a = pila[tope--];

            if (postfija[i][0] == '+')
                temporal = a + b;
            else if (postfija[i][0] == '-')
                temporal = a - b;
            else if (postfija[i][0] == '*')
                temporal = a * b;
            else if (postfija[i][0] == '/')
            {
                if (b == 0)
                    return 0;

                temporal = a / b;
            }
            else if (postfija[i][0] == '^')
            {
                if (b < 0)
                    return 0;

                potencia = 1;

                for (j = 0; j < b; j++)
                {
                    potencia = potencia * a;
                }

                temporal = potencia;
            }
            else
                return 0;

            pila[++tope] = temporal;
        }
    }

    if (tope != 0)
        return 0;

    *resultado = pila[tope];

    return 1;
}
