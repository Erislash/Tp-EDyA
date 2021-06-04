#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "operador.h"
#include "helpers.h"
#include "arbol.h"
#include "expresiones.h"
#include "pilaArboles.h"

// OBJS	= operador.o helpers.o arbol.o expresiones.o pilaArboles.o main.o
// SOURCE	= operador.c helpers.c arbol.c expresiones.c pilaArboles.c main.c
// HEADER	= operador.h helpers.h arbol.h expresiones.h pilaArboles.h


enum COMANDO {
    INVALIDO,
    PROCESANDO,
    PROCESADO,
    SALIR,
    CARGAR,
    IMPIMIR,
    EVALUAR
};


int evaluar_arbol(Arbol* arbol, TablaOps* tablaOperadores){
    /*
        evaluar_arbol: Arbol* TablaOps*
        Evalua un arbol de expresiones aritmeticas y devuelve su resultado

        Params:
            arbol: un arbol aritmetico que será evaluado

            tablaOperadores: tabla con todos los operadores

        Devuelve arbol que representa la operación introducida por el
        usuario. Si ocurre algún problema, se cancela la operación, se
        libera el espacio solicitado hasta el momento del error y se devuelve NULL
    */

    if (arbol == NULL){
        return 0;
    }
    Operador* operador = operador_valido(arbol->data, tablaOperadores);
    if (operador != NULL)
    {
        int* exp = malloc(sizeof(int) * 2);
        exp[0] = evaluar_arbol(arbol->izquierda, tablaOperadores);
        exp[1] = evaluar_arbol(arbol->derecha, tablaOperadores);

        return operador->evaluar(exp);
    }else{
        return atoi(arbol->data);
    }
}


Arbol* crear_arbol_aritmetico(char* expresion, TablaOps* tablaOperadores){
    /*
        crear_arbol_aritmetico: char* TablaOps*
        Crea un nuevo arbol binario, que representa un arbol aritmético.

        Params:
            expresion: la expresion algebraica con la que se
            construirá el arbol

            tablaOperadores: tabla con todos los operadores

        Devuelve arbol que representa la operación introducida por el
        usuario. Si ocurre algún problema, se cancela la operación, se
        libera el espacio solicitado hasta el momento del error y se devuelve NULL
    */


   //Crea una pila para ir guardando los subarboles
    PilaArboles* pila = NULL;

    //Lleva un registro de los arboles disponibles, es útil para
    //comprobar la validez de un operador teniendo en cuenta su aridad
    int arbolesEnPila = 0;

    //Guarda los operadores y operandos de la expresion
    char * datoExpresion = strtok(expresion, " ");

    //Mientra haya datos, se comprueba si es un dato numerico o un simbolo
    while( datoExpresion != NULL ) {

        //Si es un dato numerico se crea un nuevo arbol que contien ese dato
        //mientras que sus ramas están vacias. Luego, se apila en la pila de arboles
        if(es_numerico(datoExpresion) != 0){
            Arbol* nodoNumerico = malloc(sizeof(Arbol));

            nodoNumerico->data = malloc((strlen(datoExpresion) * sizeof(char))  + 1);
            strcpy(nodoNumerico->data, datoExpresion);

            nodoNumerico->derecha = NULL;
            nodoNumerico->izquierda = NULL;

            pila = apilar_arbol(pila, nodoNumerico);
            arbolesEnPila++;
        }else{
            //Si el dato no es numérico, se comprueba que ese simbolo se encuentra
            //dentro de la tabla de operadores. Si es un operador que está en la tabla,
            //comprueba si la cantidad de arboles en pila se corresponde con la aridad
            //del operador y se crea un nuevo arbol, cuyo dato es el operador y las
            //ramas se corresponden con los operandos. Luego, se apila en la
            //pila de arboles

            Operador* op = operador_valido(datoExpresion, tablaOperadores);
            if(op != NULL){
                if (arbolesEnPila < op->aridad){
                    advertencia("La cantidad de operandos es erronea para el operador aplicado");
                    destruir_pila_arboles(pila);
                    return NULL;
                }

                //Crea un nuevo arbol cuyo dato es el operador y las ramas son
                //los demás arboles que se encontraban en la pila
                Arbol* nodoOperador = malloc(sizeof(Arbol));
                nodoOperador->data = malloc(strlen(datoExpresion) + 1);
                nodoOperador->derecha = NULL;
                nodoOperador->izquierda = NULL;
                strcpy(nodoOperador->data, datoExpresion);

                if (op->aridad == 1) {
                    nodoOperador->derecha = desapilar_arbol(&pila);
                    arbolesEnPila--;
                }
                else if (op->aridad == 2) {
                    Arbol* derecha = desapilar_arbol(&pila);
                    arbolesEnPila--;
                    Arbol* izquierda = desapilar_arbol(&pila);
                    arbolesEnPila--;

                    //Se comprueba si la expresion intenta dividir por 0 o
                    //intenta encontrar el modulo entre algún número y 0. Si es el caso
                    //se destruye la pila de arboles, el ultimo arbol creado y se devuelve NULL
                    if ((strcmp(op->simbolo, "/") == 0 || strcmp(op->simbolo, "%") == 0) && evaluar_arbol(derecha, tablaOperadores) == 0) {
                        advertencia("La expresion intenta dividir por 0 o calcular el modulo de un numero y 0");
                        destruir_arbol(nodoOperador);
                        destruir_pila_arboles(pila);
                        return NULL;
                    }
                    nodoOperador->derecha = derecha;
                    nodoOperador->izquierda = izquierda;
                }
                
                pila = apilar_arbol(pila, nodoOperador);
                arbolesEnPila++;
            }
            else {
                advertencia("Operador no encontrado");
                destruir_pila_arboles(pila);
                return NULL;
            }
        }
        datoExpresion = strtok(NULL, " ");
    }

    Arbol* arbol = desapilar_arbol(&pila);
    destruir_pila_arboles(pila);
    return arbol;
}


NodoExpresiones* crear_expresion(char* alias, char* expresion, TablaOps* tablaOperadores) {
    /*
        crear_expresion: char* char* TablaOps*
        Crea una nueva expresion, le asigna un alias y un arbol.
        Devuelve dicha expresion si todo sale bien, de lo contrario, devuelve NULL

        Params:
            alias: el alias que se le asignará al arbol
            
            expresion: la expresion algebraica con la que se
            construirá el arbol

            tablaOperadores: tabla con todos los operadores
        Devuelve un nodo cuyo dato es el arbol que representa la operación
        introducida por el usuario. Si ocurre algún error durante el proceso
        se devuelve NULL y se libera el espacio pedido dentro de
        la función
    */

    NodoExpresiones* nodo = malloc(sizeof(NodoExpresiones));

    nodo->next = NULL;
    nodo->prev = NULL;
    Expresion* nuevaExpresion = malloc(sizeof(Expresion));

    nuevaExpresion->alias = malloc(strlen(alias) + 1);

    strcpy(nuevaExpresion->alias, alias);
    Arbol* arbol = crear_arbol_aritmetico(expresion, tablaOperadores);
    if (arbol == NULL)
    {
        free(nodo);
        free(nuevaExpresion->alias);
        destruir_arbol(arbol);
        free(nuevaExpresion);
        return NULL;
    }
    nuevaExpresion->arbol = arbol;
    nodo->expresion = nuevaExpresion;
    return nodo;
}


void cargar_expresion(Expresiones** expresiones, NodoExpresiones** nuevoNodo){
    /*
        cargar_expresion: Expresiones** NodoExpresiones**
        Agrega un nuevo nodo al final de la lista de expresiones.

        Params:
            expresiones: la lista de expresiones
            
            nuevoNodo: el nuevo nodo a agregar al final de la lista

            tablaOperadores: tabla con todos los operadores
        No devuelve nada
    */

    if ((*nuevoNodo) != NULL) {
        if ((*expresiones)->head == NULL) {
            (*expresiones)->head = (*nuevoNodo);
            (*expresiones)->tail = (*nuevoNodo);
        } else {
            (*expresiones)->tail->next = (*nuevoNodo);
            (*nuevoNodo)->prev = (*expresiones)->tail;
            (*expresiones)->tail = (*nuevoNodo);
        }
    }
}


NodoExpresiones* nueva_expresion(char* entrada, Expresiones* expresiones, TablaOps* tablaOperadores) {
    /*
        nueva_expresion: char* Expresiones* TablaOps*
        Evalua el alias de una nueva expresion y la agrega
         a la lista de expresiones si ese alias es válido y no ha
         sido usado. Si ocurre algún problema durante la operación, se devuelve NULL

        Params:
            entrada: la entrada del usuario por consola. Cuyo formato ya fue
            parcialmente comprobado
            
            expresiones: puntero de expresiones que contiene todas las expresiones
            introducidas.

            tablaOperadores: tabla con todos los operadores
        Devuelve un nodo cuyo sato es la expresion. Si llega a haber algun
        problema en el proceso de guardado, devuelve NULL
    */
    
    char* copiaEntrada = malloc(strlen(entrada) + 1);
    strcpy(copiaEntrada, entrada);
    
    char* token = strtok(copiaEntrada, " ");
    char* alias = malloc(strlen(token) + 1);
    strcpy(alias, token);
    
    //Si el alias es una palabra reservada, se libera la memoria
    //pedida hasta el momento en la funcion y se devuelve NULL
    if (strcmp(alias, "imprimir") == 0 || strcmp(alias, "salir") == 0 || strcmp(alias, "cargar") == 0 || strcmp(alias, "evaluar") == 0) {
        advertencia("El alias no puede ser una palabra reservada");
        free(alias);
        free(copiaEntrada);
        return NULL;
    }
    
    //Se comprueba si el alias introducido por el usuario ya existe.
    //Si existe, no se sobreescribe y se cancela la carga de la expresión
    Expresion* exp = buscar_expresion(alias, expresiones);
    if (exp != NULL) {
        advertencia("Alias existente");
        printf("El alias \"%s\" ya existe para la expresion: ", alias);
        imprimir_arbol(exp->arbol);
        free(alias);
        free(copiaEntrada);
        return NULL;
    }

    //En las siguientes lineas se extrae la operación matemática
    //introducida por el usuario

    //Se empieza a extraer una expresion desde el primer número
    //encontrado desde el comando "cargar"
    char *dest = strstr(entrada, "cargar");
    int pos;
    pos = dest - entrada;

    while ((int)entrada[pos] < 48 || (int)entrada[pos] > 57){ ++pos; }

    int longExpresion = 0;
    while (entrada[pos + longExpresion] != '\0'){ ++longExpresion; }
    
    char* expresion = malloc((sizeof(char) * longExpresion) + 1);
    
    for (int i = 0; i < longExpresion; ++i) {
        expresion[i] = entrada[pos];
        ++pos;
    }
    expresion[longExpresion] = '\0';

    //Se crea un nuevo nodo con la expresión extraida.
    //Si no se pudo crear, se cancela la operación y se libera toda la
    //memoria pedida hasta el momento en la función
    NodoExpresiones* nuevaExpresion = crear_expresion(alias, expresion, tablaOperadores);
    if (nuevaExpresion == NULL) {
        free(copiaEntrada);
        free(expresion);
        free(alias);
        free(nuevaExpresion);
        return NULL;
    }


    //Se libera la memoria requerida para operaciónes intermedias
    free(expresion);
    free(alias);
    free(copiaEntrada);
    return nuevaExpresion;
}


void imprimir_expresion(char* entrada, Expresiones* expresiones){
    /*
        imprimir_expresion: char* Expresiones*
        Imprime una expresion

        Params:
            entrada: la entrada del usuario por consola. Cuyo formato ya fue
            parcialmente comprobado
            
            expresiones: puntero de expresiones que contiene todas las expresiones
            introducidas.
        No devuelve nada
    */


    char* copiaEntrada = malloc(strlen(entrada) + 1);
    strcpy(copiaEntrada, entrada);
    
    char* alias = strtok(copiaEntrada, " ");
    alias = strtok(NULL, " ");

    if (alias != NULL) {
        Expresion* expresion = buscar_expresion(alias, expresiones);
        if (expresion == NULL) {
            advertencia("No se encontro esa alias para imprimir");
        }else{
            printf("Alias: %s\n", alias);
            printf("Expresion: ");
            imprimir_arbol(expresion->arbol);
            printf("\n\n");
        }
    }
    free(copiaEntrada);
}


void evaluar_expresion(char* entrada, Expresiones* expresiones, TablaOps* tablaOperadores){
    /*
        evaluar_expresion: char* Expresiones* TablaOps* -> int
        Evalua la expresion que corresponde a un alias dada, si se
        encuentra ese alias, se imprime su resultado

        Params:
            entrada: la entrada del usuario por consola. Cuyo formato ya fue
            parcialmente comprobado
            
            expresiones: puntero de expresiones que contiene todas las expresiones
            introducidas.

            tablaOperadores: tabla con todos los operadores cargados
        No devuelve nada
    */

    char* copiaEntrada = malloc(strlen(entrada) + 1);
    strcpy(copiaEntrada, entrada);
    
    char* alias = strtok(copiaEntrada, " ");
    alias = strtok(NULL, " ");

    if (alias != NULL) {
        Expresion* expresion = buscar_expresion(alias, expresiones);
        if (expresion == NULL) {
            advertencia("No se encontro una expresion con el alias especificada para evaluar");
        }else{
            printf("Alias: %s\n", alias);
            printf("Resultado: %d", evaluar_arbol(expresion->arbol, tablaOperadores) );
            printf("\n\n");
        }
    }
    free(copiaEntrada);
}


int procesar_entrada(char* entrada, Expresiones** expresiones, TablaOps* tablaOperadores) {
    /*
        procesar_entrada: char* Expresiones** TablaOps* -> int
        Procesa la entrada. Comprueba parciamente su formato y las proximas validaciones
        se hacen dentro de las demas funciones

        Params:
            entrada: la entrada del usuario por consola.
            
            expresiones: puntero al puntero de expresiones que contiene todas las expresiones
            introducidas.

            tablaOperadores: tabla con todos los operadores cargados
        Devuelve un entero. En particular alguno de los estados del enumerador "COMANDO"
    */


    //Se comprueba el formato de la entrada. Primero, se cuentan los espacios que hay en la expresion
    int espacios = 0;
    int lonEntrada = strlen(entrada); 
    for (int i = 0; i < lonEntrada; i++){
        if(entrada[i] == ' ') espacios++;
    }

    //Si no hay espacios y la entrada es "salir", se devuelve el estado SALIR. que luego
    //terminara el bucle princial. Sino se devuelve el estado INVALIDO   
    if (espacios == 0){
        if (strcmp(entrada, "salir") == 0)
        {
            advertencia("Adios");
            return SALIR;
        }
        advertencia("Comando invalido");
        return INVALIDO;
    }

    //Si hay un solo espacio, se comprueba si la orden es "imprimir" o "evaluar", si no lo es
    //se devuelve el estado INVALIDO
    if(espacios == 1) {
        //Copia de la entrada. Para usar con la funcion "strtok", que modifica
        //la cadena pasada por argumento
        char* copiaEntrada = malloc(strlen(entrada) + 1); 
        strcpy(copiaEntrada, entrada);
        char* orden = strtok(copiaEntrada, " ");

        if (orden != NULL){
            if (strcmp(orden, "imprimir") == 0) {
                imprimir_expresion(entrada, (*expresiones));
            } else if (strcmp(orden, "evaluar") == 0) {
                evaluar_expresion(entrada, (*expresiones), tablaOperadores);
            } else {
                advertencia("COMANDO INVALIDO...");
                return INVALIDO;
            }
        } else {
            advertencia("FORMATO INVALIDO...");
            return INVALIDO;
        } 
        free(copiaEntrada);  
    }


    //Si hay 3 espacios, compruebo el formato del comando de carga
    if (espacios > 3) {
        char* copiaEntrada = malloc(strlen(entrada) + 1);
        strcpy(copiaEntrada, entrada);
        char* token = strtok(copiaEntrada, " ");
        int cont = 0;
        //Se comprueba el formato de carga (ALIAS = cargar EXPR)
        while(token != NULL) {
            if(cont == 1 && strcmp(token, "=") != 0){
                advertencia("Formato de carga de expresion invalido");
                free(copiaEntrada);
                return INVALIDO;
            }
            else if (cont == 2 && strcmp(token, "cargar") != 0){
                advertencia("Formato de carga de expresion invalido");
                free(copiaEntrada);
                return INVALIDO;
            }
            token = strtok(NULL, " ");
            ++cont;
        }

        free(copiaEntrada);

        NodoExpresiones* nodo = nueva_expresion(entrada, (*expresiones),tablaOperadores);

        if(nodo == NULL){
            advertencia("Imposible cargar expresion");
            return INVALIDO;
        }
        cargar_expresion(expresiones, &nodo);
    }

    return PROCESADO;
}


void interpretar(TablaOps* tablaOperadores){
    //interpretar: TablaOps* -> void
    //Es la parte interactiva del programa
    //Recibe como parametro un puntero a la tabla de operadores
    //No devuelve nada


    Expresiones* expresiones = crear_expresiones();    //Lista que guarda todas las expresiones cargadas
    char entrada[200] = "";     //Variable que guarda la entrada del usario
    enum COMANDO Comando = PROCESANDO;      //Guarda el estado del programa

    //Si el estado no es salir, comenzar el bucle del programa
    while (Comando != SALIR) {  
        Comando = PROCESANDO;
        printf("\n\n\n\n\n\n\nIngrese un comando:\n\t<ALIAS> = cargar <EXPRESION> - Para cargar una nueva expresion en notacion POSTFIJA (si se vuelve a usar un mismo alias, la expresion anterior se sobrescribira)\n\timprimir <ALIAS> para mostrar una expresion cargada anteriormente\n\tevaluar <ALIAS> - Para evaluar una expresion cargada\n\tsalir - Para finalizar el programa\n");
        imprimirAlias(expresiones);     //Imprime por consola las alias de todas las expresiones introducidas
        printf("--> ");

        /*En las siguientes lineas se comprueba que la entrada
        del usuario se haya podido almacenar, que no comience con un numero
        y que no este vacia 
        */

        if(fgets(entrada, 200, stdin) == NULL){
            advertencia("Imposible cargar comando");
            Comando = INVALIDO;
        }else{
            entrada[strlen(entrada) - 1] = '\0';
        }

        if (strlen(entrada) == 0 || entrada[0] == ' ') {
            advertencia("La entrada esta vacia o inicia con un caracter vacío");
            Comando = INVALIDO;
        }


        if ((int)entrada[0] >= 48 && (int)entrada[0] <= 57) {
            advertencia("El primer caracter del comando no puede ser un numero");
            Comando = INVALIDO;
        }
        //Si la entrada pasa las primeras pruebas, se procesa
        if (Comando != INVALIDO) {
            Comando = procesar_entrada(entrada, &expresiones, tablaOperadores);
        
        }

        /*
        Si el comando es SALIR, se libera toda la memoria usada por las
        expresiones. (Incluyendo los arboles que guarda la lista, alias de las expresiones, nodos, etc)
        */
        if(Comando == SALIR){
            destruir_expresiones(expresiones);
        }
    }
}


void testear_imprimir_arbol_1(TablaOps* tabla){
    Arbol* cero = nuevo_nodo_arbol("0", NULL, NULL);
    Arbol* cinco = nuevo_nodo_arbol("5", NULL, NULL);
    Arbol* negativo = nuevo_nodo_arbol("--", NULL, cinco);
    Arbol* veinte = nuevo_nodo_arbol("20", NULL, NULL);
    Arbol* mas = nuevo_nodo_arbol("+", negativo, veinte);
    Arbol* division = nuevo_nodo_arbol("/", mas, cero);
    imprimir_arbol(division);
    printf("\n");
    printf("eval: %d\n", evaluar_arbol(division, tabla));
    destruir_arbol(division);
    printf("\n");

    //Debe imprimir la expresion ((( --5) + 20) / 0). La operación de division
    //detectará que se intenta dividir por 0 y lo informará imprimiendo un mensaje en consola
}

void testear_pila_arbol(){
    //Uso esta funcion para comprobar visualmente el apilamiento y
    //desapilamiento de un arbol

    Arbol* raiz1_2 = nuevo_nodo_arbol("8", NULL, NULL);
    Arbol* raiz1 = nuevo_nodo_arbol("--", NULL, raiz1_2);

    Arbol* raiz2 = nuevo_nodo_arbol("10", NULL, NULL);

    Arbol* raiz3 = nuevo_nodo_arbol("*", NULL, NULL);

    Arbol* raiz4_3 = nuevo_nodo_arbol("12", NULL, NULL);
    Arbol* raiz4_2 = nuevo_nodo_arbol("99", NULL, NULL);
    Arbol* raiz4 = nuevo_nodo_arbol("+", raiz4_2, raiz4_3);


    PilaArboles* pila = NULL;
    pila = apilar_arbol(pila, raiz1);
    pila = apilar_arbol(pila, raiz2);
    pila = apilar_arbol(pila, raiz3);
    pila = apilar_arbol(pila, raiz4);

    printf("Desapilado: 1\n");
    Arbol* el1 = desapilar_arbol(&pila);
    imprimir_arbol(el1);
    printf("\n");
    
    printf("Desapilado: 2\n");
    Arbol* el2 = desapilar_arbol(&pila);
    imprimir_arbol(el2);
    printf("\n");

    printf("Desapilado: 3\n");
    Arbol* el3 = desapilar_arbol(&pila);
    imprimir_arbol(el3);
    printf("\n");

    printf("Desapilado: 4\n");
    Arbol* el4 = desapilar_arbol(&pila);
    imprimir_arbol(el4);
    printf("\n");

    destruir_pila_arboles(pila);
    destruir_arbol(el1);
    destruir_arbol(el2);
    destruir_arbol(el3);
    destruir_arbol(el4);
}


int main(void){
    //Tabla de operadores
    TablaOps* tabla = crear_tabla();
    //Carga de operadores...
    cargar_operador(&tabla, "+", 2, suma);      //Agrego operador de suma
    cargar_operador(&tabla, "-", 2, resta);     //Agrego operador de resta
    cargar_operador(&tabla, "--", 1, opuesto);  //Agrego operador de opuesto
    cargar_operador(&tabla, "*", 2, producto);  //Agrego operador de producto
    cargar_operador(&tabla, "/", 2, division);  //Agrego operador de division
    cargar_operador(&tabla, "%", 2, modulo);    //Agrego operador de modulo
    cargar_operador(&tabla, "^", 2, potencia);  //Agrego operador de potencia

    //Luego de cargar los operadores se interpretan las entradas del usuario
    interpretar(tabla);


    // testear_es_numerico();
    // testear_imprimir_arbol(tabla);
    // testear_pila_arbol();

    //Libero la memoria usada por la tabla de operadores
    destruir_tabla(tabla);
    advertencia("Programa terminado");
    return 0;
}