#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>
#include "helpers.h"


int es_numerico(char* cadena){
    /*
        es_numerico: char* -> int
        Comprueba si una cadea de caracteres representa un valor numerico o no

        Params:
            cadena: cadena de caracteres a comprobar

        Devuelve 1 si es una cadena numerica. En caso de encontrar un caracter no numerico
        se devuelve 0.
    */

    int i = 0;
    while (cadena[i] != '\0') {
        //isdigit() devuelve 0 en caso de que el argumento no sea un caracter numerico
        if(isdigit(cadena[i]) == 0){
            return 0;
        }
        ++i;
    }
    return 1;
}

void testear_es_numerico(){
    assert(es_numerico("0") == 1);
    assert(es_numerico("1") == 1);
    assert(es_numerico("111") == 1);
    assert(es_numerico("-") == 0);
    assert(es_numerico("+") == 0);
    assert(es_numerico("*") == 0);
    assert(es_numerico("/") == 0);
    assert(es_numerico("---") == 0);
    assert(es_numerico("-22") == 0);
    assert(es_numerico("22--") == 0);
}


void advertencia(char* mensaje){
    /*
        advertencia: char* -> void
        Imprime un mensaje por consola

        Params:
            mensaje: el mensaje a imprimir

        No devuelve nada
    */
    printf("\n=========----\\/----=========\n");
    printf("%s", mensaje);
    printf("\n=========----\\/----=========\n");
}

