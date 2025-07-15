%option noyywrap


%{

#include "sintatico.tab.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int line = 1, column = 1;
int start_line = 0;
int comment_line = 0, comment_column = 0;

bool first_print = true;

void printVerification() {
        if (first_print) {
            first_print = false;
        } else {
            printf("\n");
        }
}

%}


%x comment


%%


\n          { line++; start_line += column; column = 1; }

[\f\t\r ]+  { column += yyleng; }

"/*"                { 
    BEGIN(comment); 
    comment_line = line; 
    comment_column = column; 
    column += yyleng; 
}

<comment><<EOF>>    { 
    BEGIN(INITIAL);
    printVerification();
    printf("error:lexical:%d:%d: unterminated comment", comment_line, comment_column); 
}

<comment>\n         {column = 1; line++;}

<comment>"/*/"      { 
    BEGIN(INITIAL); 
    column += yyleng; 
}

<comment>"*/"       { 
    BEGIN(INITIAL);
    column += yyleng; 
}

<comment>"/*"       {
    printVerification();
    printf("warning:%d:%d: '/*' within block comment", line, column);
    column += yyleng; 
}

<comment>.          {column += yyleng;}

"void"      { column+=yyleng; return VOID; }
"int"       { column+=yyleng; return INT; }
"char"      { column+=yyleng; return CHAR; }
"return"    { column+=yyleng; return RETURN; }
"break"     { column+=yyleng; return BREAK; }
"switch"    { column+=yyleng; return SWITCH; }
"case"      { column+=yyleng; return CASE; }
"default"   { column+=yyleng; return DEFAULT; }
"do"        { column+=yyleng; return DO; }
"while"     { column+=yyleng; return WHILE; }
"for"       { column+=yyleng; return FOR; }
"if"        { column+=yyleng; return IF; }
"else"      { column+=yyleng; return ELSE; }
"typedef"   { column+=yyleng; return TYPEDEF; }
"struct"    { column+=yyleng; return STRUCT; }
"+"         { column+=yyleng; return PLUS; }
"-"         { column+=yyleng; return MINUS; }
"*"         { column+=yyleng; return MULTIPLY; }
"/"         { column+=yyleng; return DIV; }
"%"         { column+=yyleng; return REMAINDER; }
"++"        { column+=yyleng; return INC; }
"--"        { column+=yyleng; return DEC; }
"&"         { column+=yyleng; return BITWISE_AND; }
"|"         { column+=yyleng; return BITWISE_OR; }
"~"         { column+=yyleng; return BITWISE_NOT; }
"^"         { column+=yyleng; return BITWISE_XOR; }
"!"         { column+=yyleng; return NOT; }
"&&"        { column+=yyleng; return LOGICAL_AND; }
"||"        { column+=yyleng; return LOGICAL_OR; }
"=="        { column+=yyleng; return EQUAL; }
"!="        { column+=yyleng; return NOT_EQUAL; }
"<"         { column+=yyleng; return LESS_THAN; }
">"         { column+=yyleng; return GREATER_THAN; }
"<="        { column+=yyleng; return LESS_EQUAL; }
">="        { column+=yyleng; return GREATER_EQUAL; }
">>"        { column+=yyleng; return R_SHIFT; }
"<<"        { column+=yyleng; return L_SHIFT; }
"="         { column+=yyleng; return ASSIGN; }
"+="        { column+=yyleng; return ADD_ASSIGN; }
"-="        { column+=yyleng; return MINUS_ASSIGN; }
";"         { column+=yyleng; return SEMICOLON; }
","         { column+=yyleng; return COMMA; }
":"         { column+=yyleng; return COLON; }
"("         { column+=yyleng; return L_PAREN; }
")"         { column+=yyleng; return R_PAREN; }
"{"         { column+=yyleng; return L_CURLY_BRACKET; }
"}"         { column+=yyleng; return R_CURLY_BRACKET; }
"["         { column+=yyleng; return L_SQUARE_BRACKET; }
"]"         { column+=yyleng; return R_SQUARE_BRACKET; }
"?"         { column+=yyleng; return TERNARY_CONDITIONAL; }
"#"         { column+=yyleng; return NUMBER_SIGN; }
"->"        { column+=yyleng; return POINTER; }
"printf"    { column+=yyleng; return PRINTF; }
"scanf"     { column+=yyleng; return SCANF; }
"define"    { column+=yyleng; return DEFINE; }
"exit"      { column+=yyleng; return EXIT; }

\\[abfnrtv\\'\"?0]  { column+=yyleng; return CHARACTER; }
\"(\\.|[^"\\])*\"   { column+=yyleng; return STRING; }
"'"(\\.|[^\\']){1}"'"  { column+=yyleng; return CHARACTER; }

[a-zA-Z_]+[a-zA-Z0-9_]* {
    if(yyleng > 255) {
        printf("error:lexical:%d:%d: identifier too long", line, column);
        column+=yyleng;
    } else {
        column+=yyleng;
        return IDENTIFIER;
    }
}

0[xX][0-9a-fA-F]+   { column+=yyleng; return NUM_HEXA; }
0[1-7][0-7]*        { column+=yyleng; return NUM_OCTAL; }
[0-9]+              { column+=yyleng; return NUM_INTEGER; }

. { column+=yyleng; printf("error:lexical:%d:%d: %s", line, column, yytext); }


%%
