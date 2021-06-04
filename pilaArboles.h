#include <stdio.h>
#include <stdlib.h>
#include "arbol.h"

#ifndef __PILAS_H_
#define __PILAS_H_


//Estructura que representa una pila de arboles
typedef struct _pilaArboles{
    Arbol* tope;    //El arbol que se encuentra en el tope de la lista
    struct _pilaArboles* siguiente; //El proximo elemento de la pila
} PilaArboles;


Arbol* desapilar_arbol(PilaArboles** pila);


PilaArboles* apilar_arbol(PilaArboles* pila, Arbol* arbol);


void destruir_pila_arboles(PilaArboles* pila);


#endif // ** __PILAS_H_ **