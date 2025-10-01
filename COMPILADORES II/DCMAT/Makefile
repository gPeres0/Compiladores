FLEX = flex
BISON = bison
CC = gcc
CFLAGS =

# Nome do programa
PROGRAMA = dcmat

# Arquivos de entrada
LEXICO = lexico.l
SINTATICO = sintatico.y
OBJECTS = ast.o plot.o symbol.o matrix.o 

# Regra principal: compilar o programa
$(PROGRAMA): $(OBJECTS) $(LEXICO) $(SINTATICO)
	$(FLEX) $(LEXICO)
	$(BISON) -d $(SINTATICO)
	$(CC) -g $(CFLAGS) *.c -I. -o $(PROGRAMA) -lm
	rm -f sintatico.tab.c
	rm -f sintatico.tab.h
	rm -f lex.yy.c
	rm -f *.o

# Compilar ast.o
ast.o: ast.c definitions.h
	$(CC) $(CFLAGS) -c ast.c

# Compilar plot.o
plot.o: plot.c definitions.h
	$(CC) $(CFLAGS) -c plot.c

# Compilar symbol.o
symbol.o: symbol.c definitions.h
	$(CC) $(CFLAGS) -c symbol.c

# Compilar matrix.o
matrix.o: matrix.c definitions.h
	$(CC) $(CFLAGS) -c matrix.c