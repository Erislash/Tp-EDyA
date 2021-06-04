#include <stdio.h>
#include <stdlib.h>

#ifndef __OPERADORES_H__
#define __OPERADORES_H__

//Puntero a funcion
typedef int (*FuncionEvaluacion)(int *args);

//Estructura que representa un operador con todas sus especificaciones
typedef struct _operador{
    char* simbolo;
    int aridad;
    FuncionEvaluacion evaluar;
} Operador;

//Estructura que representa un nodo de una lista de operadores
typedef struct _nodoOperador{
    Operador* operador;
    struct _nodoOperador* next;
    struct _nodoOperador* prev;
} NodoOperador;

//Estructura que representa una lista de operadores
typedef struct _tablaOps{
    NodoOperador* head;
    NodoOperador* tail;
} TablaOps;

TablaOps* crear_tabla();

void cargar_operador(TablaOps **tabla, char *simbolo, int aridad, FuncionEvaluacion eval);

int suma(int* args);

int resta(int* args);

int opuesto(int* args);

int producto(int* args);

int division(int* args);

int modulo(int* args);

int potencia(int* args);

void destruir_tabla(TablaOps *tabla);

Operador* operador_valido(char* simbolo, TablaOps* tablaOperadores);


#endif // ** __OPERADORES_H__ **