FLEX = flex
BISON = bison
CC = gcc
CFLAGS = -std=c99
LIBS = 

EXEC_NAME = l14e1
LEXICO = lexico.l
SINTATICO = sintatico.y

$(EXEC_NAME): $(LEXICO) $(SINTATICO)
	$(FLEX) $(LEXICO)
	$(BISON) -d $(SINTATICO)
	$(CC) lex.yy.c sintatico.tab.c -o $(EXEC_NAME)

clean:
	rm -f lex.yy.c
	rm -f sintatico.tab.c
	rm -f sintatico.tab.h
	rm -f $(EXEC_NAME)