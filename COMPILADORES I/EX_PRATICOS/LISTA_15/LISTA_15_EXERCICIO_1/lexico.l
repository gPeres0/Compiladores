%option noyywrap

%{
#include "sintatico.tab.h"
%}

%%

[\f\t\r ]*	    { }
<<EOF>>			{ return ENDFILE; }
"\n"            { return NEWLINE; }
"int"			{ return INT; }
"float"			{ return FLOAT; }
"char"			{ return CHAR; }
[a-z]+			{ return ID; }
","				{ return COMMA; }
"$"             { return END; }

.				{ printf("Simbolo Invalido -> %s\n\n", yytext); }

%%