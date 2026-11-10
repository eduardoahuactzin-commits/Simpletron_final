
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX 200

char expresion[MAX];
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
    int i = 0;
    int parentesis = 0;
    int esperaNumero = 1;
    int encontroElemento = 0;

    while (texto[i] != '\0' && texto[i] != '\n')
    {
        if (texto[i] == ' ')
        {
            i++;
        }
        else if (isdigit(texto[i]))
        {
            if (!esperaNumero)
                return 0;

            while (isdigit(texto[i]))
                i++;

            esperaNumero = 0;
            encontroElemento = 1;
        }
        else if (texto[i] == '(')
        {
            if (!esperaNumero)
                return 0;

            parentesis++;
            i++;
            esperaNumero = 1;
        }
        else if (texto[i] == ')')
        {
            if (esperaNumero || parentesis <= 0)
                return 0;

            parentesis--;
            i++;
            esperaNumero = 0;
        }
        else if (esOperador(texto[i]))
        {
            if (esperaNumero)
                return 0;

            i++;
            esperaNumero = 1;
        }
        else
        {
            return 0;
        }
    }

    if (!encontroElemento || esperaNumero || parentesis != 0)
        return 0;

    return 1;
}

int convertirPostfija(char texto[])
{
    int i = 0;
    int tope = -1;
    int j;
    char numero[30];

    cantidadPostfija = 0;

    while (texto[i] != '\0' && texto[i] != '\n')
    {
        if (texto[i] == ' ')
        {
            i++;
        }
        else if (isdigit(texto[i]))
        {
            j = 0;

            while (isdigit(texto[i]))
            {
                if (j >= 29)
                    return 0;

                numero[j] = texto[i];
                j++;
                i++;
            }

            numero[j] = '\0';

            strcpy(postfija[cantidadPostfija], numero);
            cantidadPostfija++;
        }
        else if (texto[i] == '(')
        {
            operadores[++tope] = texto[i];
            i++;
        }
        else if (texto[i] == ')')
        {
            while (tope >= 0 && operadores[tope] != '(')
            {
                postfija[cantidadPostfija][0] = operadores[tope--];
                postfija[cantidadPostfija][1] = '\0';
                cantidadPostfija++;
            }

            if (tope < 0)
                return 0;

            tope--;
            i++;
        }
        else if (esOperador(texto[i]))
        {
            while (tope >= 0 && operadores[tope] != '(' &&
                   (prioridad(operadores[tope]) > prioridad(texto[i]) ||
                   (prioridad(operadores[tope]) == prioridad(texto[i]) &&
                    texto[i] != '^')))
            {
                postfija[cantidadPostfija][0] = operadores[tope--];
                postfija[cantidadPostfija][1] = '\0';
                cantidadPostfija++;
            }

            operadores[++tope] = texto[i];
            i++;
        }

        if (cantidadPostfija >= MAX)
            return 0;
    }

    while (tope >= 0)
    {
        if (operadores[tope] == '(')
            return 0;

        postfija[cantidadPostfija][0] = operadores[tope--];
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
    int a, b;
    int valor;
    int j;
    long long potencia;

    for (i = 0; i < cantidadPostfija; i++)
    {
        if (isdigit(postfija[i][0]))
        {
            valor = atoi(postfija[i]);

            tope++;
            pila[tope] = valor;
        }
        else
        {
            if (tope < 1)
                return 0;

            b = pila[tope--];
            a = pila[tope--];

            if (postfija[i][0] == '+')
                valor = a + b;
            else if (postfija[i][0] == '-')
                valor = a - b;
            else if (postfija[i][0] == '*')
                valor = a * b;
            else if (postfija[i][0] == '/')
            {
                if (b == 0)
                    return 0;

                valor = a / b;
            }
            else if (postfija[i][0] == '^')
            {
                if (b < 0)
                    return 0;

                potencia = 1;

                for (j = 0; j < b; j++)
                {
                    potencia = potencia * a;

                    if (potencia > 999999999 || potencia < -999999999)
                        return 0;
                }

                valor = (int)potencia;
            }
            else
                return 0;

            tope++;
            pila[tope] = valor;
        }
    }

    if (tope != 0)
        return 0;

    *resultado = pila[0];

    return 1;
}

int main()
{
    int resultado;
    int i;

    printf("=== EVALUADOR DE EXPRESIONES ===\n");

    printf("Introduce una expresion: ");
    fgets(expresion, MAX, stdin);

    if (!validarExpresion(expresion))
    {
        printf("Error: expresion invalida.\n");
        return 1;
    }

    if (!convertirPostfija(expresion))
    {
        printf("Error al convertir la expresion.\n");
        return 1;
    }

    printf("\nExpresion postfija: ");

    for (i = 0; i < cantidadPostfija; i++)
        printf("%s ", postfija[i]);

    if (!evaluarPostfija(&resultado))
    {
        printf("\nError al evaluar la expresion.\n");
        return 1;
    }

    printf("\nResultado: %d\n", resultado);

    return 0;
}

