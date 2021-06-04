#include <stdio.h>
#include <stdlib.h>
#include "arbol.h"
#include "pilaArboles.h"


Arbol* desapilar_arbol(PilaArboles** pila){
    /*
        desapilar_arbol: PilaArboles** -> Arbol* | NULL
        Desapila un arbol de una pila de arboles y lo devuelve

        Params:
            pila: la pila de arboles de expresiones aritmeticas

        Devuelve un arbol de expresiones aritmeticas. En caso de que ocurra algun error, devuelve NULL
    */

    if((*pila) == NULL) return NULL;
    Arbol* arbol = (*pila)->tope;
    PilaArboles* siguiente = (*pila)->siguiente;
    free((*pila));
    (*pila) = siguiente;
    return arbol;
}


PilaArboles* apilar_arbol(PilaArboles* pila, Arbol* arbol){
    /*
        apilar_arbol: PilaArboles* Arbol* -> PilaArboles* | NULL
        Desapila un arbol de una pila de arboles y lo devuelve

        Params:
            pila: la pila de arboles de expresiones aritmeticas

            arbol: el arbol que se apilará

        Devuelve la pila con el nuevo arbol agregado
    */
   if (arbol == NULL) {
       return pila;
   }

    PilaArboles* nuevaPila = malloc(sizeof(PilaArboles));
    
    nuevaPila->siguiente = NULL;
    nuevaPila->tope = arbol;

    if (pila == NULL) {
        return nuevaPila;
    } else {
        nuevaPila->siguiente = pila;
        return nuevaPila;
    }
}


void destruir_pila_arboles(PilaArboles* pila){
    /*
        destruir_pila_arboles: PilaArboles* ->  void
        Libera toda la memoria pedida por una pila y sus arboles internos

        Params:
            pila: la pila de arboles de expresiones aritmeticas que será liberada

        No devuelve nada
    */
    if (pila != NULL) {
        destruir_arbol(pila->tope);
        destruir_pila_arboles(pila->siguiente);
        free(pila);
    }
}
