#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "operador.h"

TablaOps* crear_tabla(){
    /*
        crear_tabla: void -> TablaOps*
        Crea una lista que guardará todos los operadores disponibles

        Params: No toma argumentos

        Devuelve una lista que representa una tabla de operadores
    */
    TablaOps* tabla = malloc(sizeof(TablaOps));
    tabla->head = NULL;
    tabla->tail = NULL;
    return tabla;
}

void cargar_operador(TablaOps** tabla, char* simbolo, int aridad, FuncionEvaluacion eval){
    /*
        cargar_operador: TablaOps** char* int FuncionEvaluacion -> void
        Carga un nuevo operador con las caracteristicas especificadas por los argumentos
        de la funcion, agregando un nuevo nodo a la lista de operadores pasada como primer argumento

        Params:
            tabla: la lista que contiene los operadores
            simbolo: el simbolo que representa al nuevo operador
            aridad: la aridad del nuevo operador
            eval: la funcion de evaluacion correspondiente al nuevo operador

        No devuelve nada
    */

    Operador* nuevoOperador = malloc(sizeof(Operador));
    nuevoOperador->simbolo = malloc(strlen(simbolo) + 1);
    strcpy(nuevoOperador->simbolo, simbolo);
    nuevoOperador->aridad = aridad;
    nuevoOperador->evaluar = eval;

    NodoOperador* nodo = malloc(sizeof(NodoOperador));
    nodo->operador = nuevoOperador;
    nodo->next = NULL;
    nodo->prev = NULL;

    if((*tabla)->head == NULL){
        (*tabla)->head = nodo;
        (*tabla)->tail = nodo;
    }else{
        nodo->prev = (*tabla)->tail;
        (*tabla)->tail->next = nodo;
        (*tabla)->tail = nodo;
    }
}


//Operaciones aritmeticas. Se realiza la operación
//correspondiente, se devuelve el resultado de ser posible y se libera la 
//memoria ocupada por los argumentos
int suma(int* args){
    int res = args[0] + args[1];
    free(args);
    return res;
}
int resta(int* args){
    int res = args[0] - args[1];
    free(args);
    return res;
}
int opuesto(int* args){
    int res = args[1];
    free(args);
    return -res;
}
int producto(int* args){
    int res = args[0] * args[1];
    free(args);
    return res;
}
int division(int* args){
    if(args[1] == 0){
        printf("Error: Intento de dividir por 0\n");
        return 0;
    }
    printf("DIVIDIENDO\n1) - %d\n2) - %d\n", args[0], args[1]);
    int res = args[0] / args[1];
    free(args);
    return res;
}
int modulo(int* args){
    if(args[1] == 0){
        printf("Error: Intento de modulo por 0\n");
        return 0;
    }
    int res = args[0] % args[1];
    free(args);
    return res;
}
int potencia(int* args){
    
    int pot = 1;
    for (int i = 0; i < args[1]; i++)
    {
        pot *= args[0];
    }
    free(args);
    return pot;
    
}


void destruir_tabla(TablaOps *tabla){
    /*
        destruir_tabla: TablaOps* -> void
        Libera toda la memoria solicitada por una tabla

        Params:
            tabla: la lista que contiene los operadores, que será liberada

        No devuelve nada
    */
    NodoOperador* node = tabla->head;
    while (node != NULL){
        NodoOperador* next = node->next;
        free(node->operador->simbolo);
        free(node->operador);
        free(node);
        node = next;
    }
    free(tabla);
}


Operador* operador_valido(char* simbolo, TablaOps* tablaOperadores){
    /*
        operador_valido: char* TablaOps* -> Operador* | NULL
        Comprueba si un simbolo es un operador que se encuentra en la lista de operadores o no

        Params:
            simbolo: el simbolo que se comprabará que corresponda a un operador guardado en
            la tabla de operadores

            tablaOperadores: una lista que representa la tabla de operadores

        Devuelve el operador si el símbolo se corresponde con un operador guardado en
        la tabla de operadores. Si ese no fuera el caso, devuelve NULL
    */

    NodoOperador* node = tablaOperadores->head;
    while (node != NULL){
        if(strcmp(node->operador->simbolo, simbolo) == 0){
            return node->operador;
        }
        node = node->next;
    }
    return NULL;
}