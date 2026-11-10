
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
    int j = 0;
    int instruccion = 0;
    int detener = 0;
    int error = 0;
    int resultadoLectura = 0;
    int resultado = 0;
    int cantidadInstrucciones = 0;

    /* Archivo */
    FILE *archivo;
    char nombreArchivo[200];

    printf("=== SIMULADOR SIMPLETRON ===\n");

    printf("\nIntroduce la ruta del archivo SML:\n");
    scanf(" %199[^\n]", nombreArchivo);

    /* Eliminar comillas */
    if (nombreArchivo[0] == '"')
    {
        for (j = 0; nombreArchivo[j] != '\0'; j++)
        {
            nombreArchivo[j] = nombreArchivo[j + 1];
        }

        for (j = 0; nombreArchivo[j] != '\0'; j++)
        {
            if (nombreArchivo[j] == '"')
            {
                nombreArchivo[j] = '\0';
                break;
            }
        }
    }

    /* Abrir archivo */
    archivo = fopen(nombreArchivo, "r");

    if (archivo == NULL)
    {
        printf("\nError: no se pudo abrir el archivo.\n");
        return 1;
    }

    /* Cargar instrucciones */
    printf("\n=== CARGA DEL PROGRAMA ===\n");

    while (1)
    {
        resultadoLectura = fscanf(archivo, "%d", &instruccion);

        if (resultadoLectura == EOF)
        {
            break;
        }

        if (resultadoLectura != 1)
        {
            printf("\nError: el archivo contiene un dato no numerico.\n");
            error = 1;
            break;
        }

        if (i >= 100)
        {
            printf("\nError: el programa supera las 100 instrucciones.\n");
            error = 1;
            break;
        }

        if (instruccion < 0 || instruccion > 9999)
        {
            printf("\nError: instruccion fuera de rango.\n");
            error = 1;
            break;
        }

        memoria[i] = instruccion;

        printf("Memoria[%d]: %d\n", i, instruccion);

        i++;
    }

    fclose(archivo);

    cantidadInstrucciones = i;

    if (error == 0 && cantidadInstrucciones == 0)
    {
        printf("\nError: el archivo esta vacio.\n");
        error = 1;
    }

    if (error == 1)
    {
        printf("\nNo se puede ejecutar el programa.\n");
        return 1;
    }

    /* Ejecucion */
    printf("\n=== EJECUCION DEL SIMPLETRON ===\n");

    while (detener == 0)
    {
        /* FETCH */
        if (contadorInstrucciones < 0 ||
            contadorInstrucciones >= cantidadInstrucciones)
        {
            printf("\nError: direccion de memoria fuera del programa.\n");
            detener = 1;
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

                if (scanf("%d", &memoria[operando]) != 1)
                {
                    printf("Error: entrada no valida.\n");
                    detener = 1;
                }
                else if (memoria[operando] < -9999 ||
                         memoria[operando] > 9999)
                {
                    printf("Error: valor fuera de rango.\n");
                    detener = 1;
                }
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
                resultado = acumulador + memoria[operando];

                if (resultado < -9999 || resultado > 9999)
                {
                    printf("Error: desbordamiento en la suma.\n");
                    detener = 1;
                }
                else
                {
                    acumulador = resultado;
                }
                break;

            case 31:
                resultado = acumulador - memoria[operando];

                if (resultado < -9999 || resultado > 9999)
                {
                    printf("Error: desbordamiento en la resta.\n");
                    detener = 1;
                }
                else
                {
                    acumulador = resultado;
                }
                break;

            case 32:
                if (memoria[operando] == 0)
                {
                    printf("Error: division entre cero.\n");
                    detener = 1;
                }
                else
                {
                    resultado = acumulador / memoria[operando];

                    if (resultado < -9999 || resultado > 9999)
                    {
                        printf("Error: desbordamiento en la division.\n");
                        detener = 1;
                    }
                    else
                    {
                        acumulador = resultado;
                    }
                }
                break;

            case 33:
                resultado = acumulador * memoria[operando];

                if (resultado < -9999 || resultado > 9999)
                {
                    printf("Error: desbordamiento en la multiplicacion.\n");
                    detener = 1;
                }
                else
                {
                    acumulador = resultado;
                }
                break;

            case 40:
                if (operando >= cantidadInstrucciones)
                {
                    printf("Error: direccion de salto invalida.\n");
                    detener = 1;
                }
                else
                {
                    contadorInstrucciones = operando;
                }
                break;

            case 41:
                if (acumulador < 0)
                {
                    if (operando >= cantidadInstrucciones)
                    {
                        printf("Error: direccion de salto invalida.\n");
                        detener = 1;
                    }
                    else
                    {
                        contadorInstrucciones = operando;
                    }
                }
                break;

            case 42:
                if (acumulador == 0)
                {
                    if (operando >= cantidadInstrucciones)
                    {
                        printf("Error: direccion de salto invalida.\n");
                        detener = 1;
                    }
                    else
                    {
                        contadorInstrucciones = operando;
                    }
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

    /* Memoria final */
    printf("\n=== MEMORIA FINAL ===\n");

    for (i = 0; i < 100; i++)
    {
        printf("Memoria[%02d]: %+05d\n", i, memoria[i]);
    }

    printf("\nAcumulador final: %d\n", acumulador);

    return 0;
}

