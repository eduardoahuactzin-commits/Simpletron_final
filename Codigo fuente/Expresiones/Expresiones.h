
#ifndef EXPRESIONES_H
#define EXPRESIONES_H

#define MAX_EXPRESIONES 200

int prioridad(char operador);
int esOperador(char caracter);
int validarExpresion(char texto[]);
int convertirPostfija(char texto[]);
int evaluarPostfija(int *resultado);

extern char postfija[MAX_EXPRESIONES][30];
extern char operadores[MAX_EXPRESIONES];
extern int cantidadPostfija;

#endif

