#include <stdio.h>
#include <stdlib.h>


#ifndef __EXPRESIONES_H__
#define __EXPRESIONES_H__


//Estructura que representa una expresion guardada por el usuario
typedef struct _expresion{
    char* alias;    //el alias al que está asociado el arbol de expresiones aritmeticas
    Arbol* arbol;   //el arbol de expresiones aritmeticas
} Expresion;


//Estructura que representa un nodo de una lista de expresiones
typedef struct _nodoExpresiones{
    Expresion* expresion;   //
    struct _nodoExpresiones* next;  
    struct _nodoExpresiones* prev;  
} NodoExpresiones;

//Estructura que representa una lista de expresiones
typedef struct _expresiones{
    NodoExpresiones* head;
    NodoExpresiones* tail;
} Expresiones;


Expresion* buscar_expresion(char* alias, Expresiones* expresiones);

Expresiones* crear_expresiones();

void imprimirAlias(Expresiones* expresiones);

void destruir_expresiones(Expresiones* expresiones);



#endif // ** __EXPRESIONES_H__ **