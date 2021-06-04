#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "arbol.h"
#include "helpers.h"
#include "expresiones.h"


Expresion* buscar_expresion(char* alias, Expresiones* expresiones){
    /*
        buscar_expresion: char* Expresiones* -> Expresion*
        Devuelve una expresion que coincida con el alias pasado como parametro.
        Si no se encuentra una expresion asociada a ese alias, se devuelve NULL

        Params:
            alias: el alias buscado

            expresiones: todas las expresiones cargadas hasta el momento

        Devuelve una expresion si el alias introducido corresponde a una. Sino devuelve NULL
    */
    NodoExpresiones* head = expresiones->head;
    while (head != NULL){
        if (strcmp(head->expresion->alias, alias) == 0){
            return head->expresion;
        }
        head = head->next;
    }
    return NULL;
}


Expresiones* crear_expresiones(){
    /*
        crear_expresiones: void -> Expresiones*
        crea una lista de expresiones

        Params:
            No toma parametros

        Devuelve una lista de expresiones inicializada
    */
    Expresiones* expresiones = malloc(sizeof(Expresiones));
    expresiones->head = NULL;
    expresiones->tail = NULL;
    return expresiones;
}


void destruir_expresiones(Expresiones* expresiones){
    /*
        destruir_expresiones: Expresiones* -> void
        Liibera toda la memoria usada por una lista de expresiones

        Params:
            alias: el alias buscado

            expresiones: todas las expresiones cargadas hasta el momento

        No devuelve nada
    */

    NodoExpresiones* head = expresiones->head;
    while (head != NULL) {
        destruir_arbol(head->expresion->arbol);
        free(head->expresion->alias);
        free(head->expresion);
        NodoExpresiones* next = head->next;
        free(head);
        head = next;
    }
    free(expresiones);
}


void imprimirAlias(Expresiones* expresiones){
    /*
        imprimirAlias: Expresiones* -> void
        imprime un listado de todas las alias que se cargaron hasta el momento

        Params:
            expresiones: todas las expresiones cargadas hasta el momento

        No devuelve nada
    */

    NodoExpresiones* head = expresiones->head;
    if (head != NULL){
        printf("==========-------==========\n");
        printf("Expresiones: \n");
        int i = 1;
        while (head != NULL) {
            printf("\t %d - %s\n", i, head->expresion->alias);
            head = head->next;
            ++i;
        }
        printf("...no hay mas expresiones.\n");
        
        printf("==========-------==========\n");
    }else{
        printf("==========-------==========\n");
        printf("Aun no ha cargado ninguna expresion\n");
        printf("==========-------==========\n");
    }
}