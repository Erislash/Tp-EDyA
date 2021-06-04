OBJS	= operador.o helpers.o arbol.o expresiones.o pilaArboles.o main.o
SOURCE	= operador.c helpers.c arbol.c expresiones.c pilaArboles.c main.c
HEADER	= operador.h helpers.h arbol.h expresiones.h pilaArboles.h
OUT	= arbol_aritmetico
CC	 = gcc
FLAGS	 = -g -c -Wall -Wextra -Werror
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

operador.o: operador.c
	$(CC) $(FLAGS) operador.c -std=c99

helpers.o: helpers.c
	$(CC) $(FLAGS) helpers.c -std=c99

arbol.o: arbol.c
	$(CC) $(FLAGS) arbol.c -std=c99

expresiones.o: expresiones.c
	$(CC) $(FLAGS) expresiones.c -std=c99

pilaArboles.o: pilaArboles.c
	$(CC) $(FLAGS) pilaArboles.c -std=c99

main.o: main.c
	$(CC) $(FLAGS) main.c -std=c99


clean:
	rm -f $(OBJS)

run: $(OUT)
	./$(OUT)