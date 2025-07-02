FLEX=flex
BISON=bison
CC=gcc
LIBS=-I. -lm

PROGRAMA = l15e1
LEXICO = lexico.l
SINTATICO = sintatico.y

$(PROGRAMA): $(LEXICO)
	$(FLEX) $(LEXICO)
	$(BISON) -d $(SINTATICO)
	$(CC) *.c $(LIBS) -o $(PROGRAMA)

clean:
	rm -f lex.yy.c
	rm -f l15e1
	rm -f sintatico.tab.c
	rm -f sintatico.tab.h