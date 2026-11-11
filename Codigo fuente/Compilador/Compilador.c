
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    FILE *archivo;
    char nombreArchivo[200];
    char linea[200];

    printf("=== COMPILADOR SIMPLE A SML ===\n\n");

    printf("Nombre o ruta del archivo Simple: ");
    scanf(" %199[^\n]", nombreArchivo);

    archivo = fopen(nombreArchivo, "r");

    if (archivo == NULL)
    {
        printf("Error: no se pudo abrir el archivo.\n");
        return 1;
    }

    printf("\n=== CONTENIDO DEL PROGRAMA ===\n\n");

    while (fgets(linea, sizeof(linea), archivo) != NULL)
    {
        printf("%s", linea);
    }

    fclose(archivo);

    printf("\n\nArchivo leido correctamente.\n");

    return 0;
}

