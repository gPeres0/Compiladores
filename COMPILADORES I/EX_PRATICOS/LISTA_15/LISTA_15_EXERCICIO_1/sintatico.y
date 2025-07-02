%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabelaSimbolos.h"

TabelaHash tabelaSimbolos;
char tipoAtual[16];
int linhaAtual;

void yyerror(void* s);
void shiftLine();
void resetAndParse();
void searchAndInsert();

extern int yylex();
extern char* yytext;

%}

%token ENDFILE
%token NEWLINE
%token INT
%token FLOAT
%token CHAR
%token ID
%token COMMA
%token END

%start sintatico

%%

sintatico
	: l15 NEWLINE { exibirMensagemSucesso(tabelaSimbolos, linhaAtual); linhaAtual++; resetAndParse(); }
	| l15 ENDFILE { exibirMensagemSucesso(tabelaSimbolos, linhaAtual); limparTabela(tabelaSimbolos); exit(0); }
;

l15
	: d d_linha END { }
;


d_linha
	: d d_linha { }
	| { }
;

d
	: t i { }
;

t
	: INT { strcpy(tipoAtual, "int"); }
	| FLOAT { strcpy(tipoAtual, "float"); }
	| CHAR { strcpy(tipoAtual, "char"); }
;

i
	: ID { searchAndInsert(yytext); }
	| i COMMA ID { searchAndInsert(yytext); }
;

%%

void searchAndInsert(char* nome) {
	inserirTabelaSimbolos(tabelaSimbolos, tipoAtual, nome, linhaAtual);
}

void resetAndParse() {
	limparTabela(tabelaSimbolos);
	tabelaSimbolos = criarTabelaSimbolos();
	yyparse();
}

void shiftLine() {
	while(strcmp("\n", yytext) != 0) {
		if(yytext[0] == 0) {
			exit(0);
		}
		yylex();
	}
	yyparse();
}

void yyerror (void* s) {
	printf("Erro sintatico -> %s\n", yytext);
	shiftLine();
}

int main(int argc, char** argv) {
	linhaAtual = 1;
	tabelaSimbolos = criarTabelaSimbolos();
	yyparse();
	limparTabela(tabelaSimbolos);
	return 0;
}