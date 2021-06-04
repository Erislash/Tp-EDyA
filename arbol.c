#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "arbol.h"
#include "helpers.h"



Arbol* crear_arbol(){
    return NULL;
}


Arbol* nuevo_nodo_arbol(char* dato, Arbol* izquierda, Arbol* derecha){
    /*
        nuevo_nodo_arbol: char* Arbol* Arbol* -> Arbol*
        Crea un nuevo arbol con los parametros dados

        Params:
            dato: el dato que contendrá el nuevo arbol

            izquierda: subarbol izquierdo

            derecha: subarbol derecho

        Devuelve arbol
    */

    Arbol* nuevoArbol = crear_arbol();
    nuevoArbol = malloc(sizeof(Arbol));
    nuevoArbol->data = malloc(strlen(dato) + 1);
    strcpy(nuevoArbol->data, dato);
    nuevoArbol->derecha = derecha;
    nuevoArbol->izquierda = izquierda;
    return nuevoArbol;
}


void destruir_arbol(Arbol* arbol){
    /*
        destruir_arbol: Arbol* -> void
        Libera toda la memoria asignada a un arbol y sus nodos

        Params:
            arbol: el arbol a liberar

        No devuelve nada
    */

    if (arbol != NULL) {
        free(arbol->data);
        destruir_arbol(arbol->derecha);
        destruir_arbol(arbol->izquierda);
        free(arbol);
    }
}


void imprimir_arbol(Arbol* arbol){
    /*
        imprimir_arbol: Arbol* -> void
        Imprime por consola un arbol de expresiones aritmeticas en notacion
        infija

        Params:
            arbol: el arbol a imprimir

        No devuelve nada
    */

    if (arbol == NULL){
        printf("");
    }else{
        if(es_numerico(arbol->data) != 1){
            printf("(");
            imprimir_arbol(arbol->izquierda);
            if (strcmp(arbol->data, "--") == 0) {
                printf(" %s", arbol->data);
            }else{
                printf(" %s ", arbol->data);
            }
            imprimir_arbol(arbol->derecha);
            printf(")");
        }else{
            printf("%s", arbol->data);
        }
    }
}