
#include <stdio.h>
#include "Expresiones.h"

int main()
{
    int resultado;
    int i;
    char expresion[MAX];

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
    {
        printf("%s ", postfija[i]);
    }

    if (!evaluarPostfija(&resultado))
    {
        printf("\nError al evaluar la expresion.\n");
        return 1;
    }

    printf("\nResultado: %d\n", resultado);

    return 0;
}


