FLEX=flex
BISON=bison
CC=gcc
CFLAGS=-Wall

PROGRAMA = sintatico
LEXICO = lexico.l
SINTATICO = sintatico.y

$(PROGRAMA): $(LEXICO) $(SINTATICO)
	$(FLEX) $(LEXICO)
	$(BISON) -d $(SINTATICO)
	$(CC) $(CFLAGS) *.c -I. -o $(PROGRAMA)

clean:
	rm -f sintatico.tab.c
	rm -f sintatico.tab.h
	rm -f lex.yy.c
	rm -f $(PROGRAMA)