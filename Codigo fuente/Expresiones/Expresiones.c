
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Expresiones.h"

char postfija[MAX_EXPRESIONES][30];
char operadores[MAX_EXPRESIONES];
int cantidadPostfija = 0;

int prioridad(char operador)
{
    if (operador == '^')
        return 3;

    if (operador == '*' || operador == '/' || operador == '%')
        return 2;

    if (operador == '+' || operador == '-')
        return 1;

    return 0;
}

int esOperador(char caracter)
{
    return caracter == '+' || caracter == '-' ||
           caracter == '*' || caracter == '/' ||
           caracter == '%' || caracter == '^';
}

int esOperando(char caracter)
{
    return isdigit((unsigned char)caracter) ||
           (caracter >= 'a' && caracter <= 'z');
}

int validarExpresion(char texto[])
{
    int i = 0;
    int parentesis = 0;
    int esperaOperando = 1;

    while (texto[i] != '\0')
    {
        if (isspace((unsigned char)texto[i]))
        {
            i++;
            continue;
        }

        if (isdigit((unsigned char)texto[i]))
        {
            if (!esperaOperando)
                return 0;

            while (isdigit((unsigned char)texto[i]))
                i++;

            esperaOperando = 0;
        }
        else if (texto[i] >= 'a' && texto[i] <= 'z')
        {
            if (!esperaOperando)
                return 0;

            i++;
            esperaOperando = 0;
        }
        else if (texto[i] == '(')
        {
            if (!esperaOperando)
                return 0;

            parentesis++;
            i++;
        }
        else if (texto[i] == ')')
        {
            if (esperaOperando || parentesis <= 0)
                return 0;

            parentesis--;
            i++;
            esperaOperando = 0;
        }
        else if (esOperador(texto[i]))
        {
            if (esperaOperando)
                return 0;

            esperaOperando = 1;
            i++;
        }
        else
        {
            return 0;
        }
    }

    return parentesis == 0 && !esperaOperando;
}

int convertirPostfija(char texto[])
{
    int i;
    int j;
    int k = 0;

    cantidadPostfija = 0;

    if (!validarExpresion(texto))
        return 0;

    for (i = 0; texto[i] != '\0'; i++)
    {
        if (isspace((unsigned char)texto[i]))
            continue;

        if (isdigit((unsigned char)texto[i]))
        {
            j = 0;

            while (isdigit((unsigned char)texto[i]))
            {
                if (j >= 28)
                    return 0;

                postfija[cantidadPostfija][j] = texto[i];
                j++;
                i++;
            }

            postfija[cantidadPostfija][j] = '\0';
            cantidadPostfija++;
            i--;

        }
        else if (texto[i] >= 'a' && texto[i] <= 'z')
        {
            postfija[cantidadPostfija][0] = texto[i];
            postfija[cantidadPostfija][1] = '\0';
            cantidadPostfija++;
        }
        else if (texto[i] == '(')
        {
            if (k >= MAX_EXPRESIONES)
                return 0;

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

            if (k == 0)
                return 0;

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

            if (k >= MAX_EXPRESIONES)
                return 0;

            operadores[k++] = texto[i];
        }

        if (cantidadPostfija >= MAX_EXPRESIONES)
            return 0;
    }

    while (k > 0)
    {
        if (operadores[k - 1] == '(')
            return 0;

        postfija[cantidadPostfija][0] = operadores[--k];
        postfija[cantidadPostfija][1] = '\0';
        cantidadPostfija++;

        if (cantidadPostfija >= MAX_EXPRESIONES)
            return 0;
    }

    return 1;
}

int evaluarPostfija(int *resultado)
{
    int pila[MAX_EXPRESIONES];
    int tope = -1;
    int i;
    int a;
    int b;
    int j;
    int potencia;
    int temporal;

    for (i = 0; i < cantidadPostfija; i++)
    {
        if (isdigit((unsigned char)postfija[i][0]))
        {
            if (tope >= MAX_EXPRESIONES - 1)
                return 0;

            pila[++tope] = atoi(postfija[i]);
        }
        else if (esOperador(postfija[i][0]))
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
            else if (postfija[i][0] == '%')
            {
                if (b == 0)
                    return 0;

                temporal = a % b;
            }
            else if (postfija[i][0] == '^')
            {
                if (b < 0)
                    return 0;

                potencia = 1;

                for (j = 0; j < b; j++)
                    potencia *= a;

                temporal = potencia;
            }
            else
                return 0;

            pila[++tope] = temporal;
        }
        else
        {
            /* El evaluador numerico no puede calcular variables. */
            return 0;
        }
    }

    if (tope != 0)
        return 0;

    *resultado = pila[tope];

    return 1;
}

