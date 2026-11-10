
#include <stdio.h>

int main()
{
    /* Memoria */
    int memoria[100] = {0};

    /* Registros */
    int acumulador = 0;
    int contadorInstrucciones = 0;
    int registroInstruccion = 0;
    int codigoOperacion = 0;
    int operando = 0;

    /* Variables auxiliares */
    int i = 0;
    int instruccion = 0;
    int detener = 0;
    int error = 0;

    /* Archivo */
    FILE *archivo;
    char nombreArchivo[200];

    printf("=== SIMULADOR SIMPLETRON ===\n");

    /* Solicitar ruta del archivo */
    printf("\nIntroduce la ruta del archivo SML:\n");
    scanf(" %199[^\n]", nombreArchivo);

    /* Eliminar comillas si existen */
    if (nombreArchivo[0] == '"')
    {
        for (i = 0; nombreArchivo[i] != '\0'; i++)
        {
            nombreArchivo[i] = nombreArchivo[i + 1];
        }

        for (i = 0; nombreArchivo[i] != '\0'; i++)
        {
            if (nombreArchivo[i] == '"')
            {
                nombreArchivo[i] = '\0';
                break;
            }
        }
    }

    /* Abrir archivo */
    archivo = fopen(nombreArchivo, "r");

    if (archivo == NULL)
    {
        printf("Error: no se pudo abrir el archivo.\n");
        return 1;
    }

    /* Cargar instrucciones */
    printf("\n=== CARGA DEL PROGRAMA ===\n");

    while (i < 100 && fscanf(archivo, "%d", &instruccion) == 1)
    {
        if (instruccion < -9999 || instruccion > 9999)
        {
            printf("Error: instruccion fuera de rango.\n");
            error = 1;
            break;
        }

        memoria[i] = instruccion;

        printf("Memoria[%d]: %d\n", i, instruccion);

        i++;
    }

    fclose(archivo);

    if (error == 1)
    {
        return 1;
    }

    /* Ciclo de ejecucion */
    printf("\n=== EJECUCION DEL SIMPLETRON ===\n");

    while (detener == 0)
    {
        /* FETCH */
        if (contadorInstrucciones < 0 || contadorInstrucciones >= 100)
        {
            printf("Error: direccion de memoria invalida.\n");
            break;
        }

        registroInstruccion = memoria[contadorInstrucciones];

        /* DECODE */
        codigoOperacion = registroInstruccion / 100;
        operando = registroInstruccion % 100;

        printf("\nInstruccion: %d\n", registroInstruccion);

        contadorInstrucciones++;

        /* EXECUTE */
        switch (codigoOperacion)
        {
            case 10:
                printf("Introduce un numero: ");
                scanf("%d", &memoria[operando]);
                break;

            case 11:
                printf("Resultado: %d\n", memoria[operando]);
                break;

            case 20:
                acumulador = memoria[operando];
                break;

            case 21:
                memoria[operando] = acumulador;
                break;

            case 30:
                acumulador = acumulador + memoria[operando];
                break;

            case 31:
                acumulador = acumulador - memoria[operando];
                break;

            case 32:
                if (memoria[operando] == 0)
                {
                    printf("Error: division entre cero.\n");
                    detener = 1;
                }
                else
                {
                    acumulador = acumulador / memoria[operando];
                }
                break;

            case 33:
                acumulador = acumulador * memoria[operando];
                break;

            case 40:
                contadorInstrucciones = operando;
                break;

            case 41:
                if (acumulador < 0)
                {
                    contadorInstrucciones = operando;
                }
                break;

            case 42:
                if (acumulador == 0)
                {
                    contadorInstrucciones = operando;
                }
                break;

            case 43:
                printf("Programa terminado.\n");
                detener = 1;
                break;

            default:
                printf("Error: codigo de operacion desconocido.\n");
                detener = 1;
                break;
        }
    }

    /* Mostrar memoria */
    printf("\n=== MEMORIA FINAL ===\n");

    for (i = 0; i < 100; i++)
    {
        printf("Memoria[%02d]: %+05d\n", i, memoria[i]);
    }

    printf("\nAcumulador final: %d\n", acumulador);

    return 0;
}

