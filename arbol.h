#include <stdio.h>
#include <stdlib.h>

#ifndef __ARBOLES_H__
#define __ARBOLES_H__


typedef struct _arbol{
    char* data; //Operador u operando que contine la raiz
    struct _arbol* izquierda;   //Subarbol izquierdo
    struct _arbol* derecha;     //Subarbol derecho
} Arbol;

Arbol* crear_arbol();

Arbol* nuevo_nodo_arbol(char* dato, Arbol* izquierda, Arbol* derecha);

void destruir_arbol(Arbol* arbol);

void imprimir_arbol(Arbol* arbol);


#endif // ** __ARBOLES_H__ **