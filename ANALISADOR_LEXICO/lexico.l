%option noyywrap


%{
    
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int line = 1, column = 1;
int comment_line = 0, comment_column = 0;
bool first_print = true;

char* treatString(const char* str) {
    size_t len = strlen(str);
    char* buf = (char*)malloc(len - 1);
    if (!buf) return NULL;
    memcpy(buf, str + 1, len - 2);
    buf[len - 2] = '\0';
    return buf;
}

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


\n          { line++; column = 1; }

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

"void"      { printVerification(); printf("VOID"); column+=yyleng; }
"int"       { printVerification(); printf("INT"); column+=yyleng; }
"char"      { printVerification(); printf("CHAR"); column+=yyleng; }
"return"    { printVerification(); printf("RETURN"); column+=yyleng; }
"break"     { printVerification(); printf("BREAK"); column+=yyleng; }
"switch"    { printVerification(); printf("SWITCH"); column+=yyleng; }
"case"      { printVerification(); printf("CASE"); column+=yyleng; }
"default"   { printVerification(); printf("DEFAULT"); column+=yyleng; }
"do"        { printVerification(); printf("DO"); column+=yyleng; }
"while"     { printVerification(); printf("WHILE"); column+=yyleng; }
"for"       { printVerification(); printf("FOR"); column+=yyleng; }
"if"        { printVerification(); printf("IF"); column+=yyleng; }
"else"      { printVerification(); printf("ELSE"); column+=yyleng; }
"typedef"   { printVerification(); printf("TYPEDEF"); column+=yyleng; }
"struct"    { printVerification(); printf("STRUCT"); column+=yyleng; }
"+"         { printVerification(); printf("PLUS"); column+=yyleng; }
"-"         { printVerification(); printf("MINUS"); column+=yyleng; }
"*"         { printVerification(); printf("MULTIPLY"); column+=yyleng; }
"/"         { printVerification(); printf("DIV"); column+=yyleng; }
"%"         { printVerification(); printf("REMAINDER"); column+=yyleng; }
"++"        { printVerification(); printf("INC"); column+=yyleng; }
"--"        { printVerification(); printf("DEC"); column+=yyleng; }
"&"         { printVerification(); printf("BITWISE_AND"); column+=yyleng; }
"|"         { printVerification(); printf("BITWISE_OR"); column+=yyleng; }
"~"         { printVerification(); printf("BITWISE_NOT"); column+=yyleng; }
"^"         { printVerification(); printf("BITWISE_XOR"); column+=yyleng; }
"!"         { printVerification(); printf("NOT"); column+=yyleng; }
"&&"        { printVerification(); printf("LOGICAL_AND"); column+=yyleng; }
"||"        { printVerification(); printf("LOGICAL_OR"); column+=yyleng; }
"=="        { printVerification(); printf("EQUAL"); column+=yyleng; }
"!="        { printVerification(); printf("NOT_EQUAL"); column+=yyleng; }
"<"         { printVerification(); printf("LESS_THAN"); column+=yyleng; }
">"         { printVerification(); printf("GREATER_THAN"); column+=yyleng; }
"<="        { printVerification(); printf("LESS_EQUAL"); column+=yyleng; }
">="        { printVerification(); printf("GREATER_EQUAL"); column+=yyleng; }
">>"        { printVerification(); printf("R_SHIFT"); column+=yyleng; }
"<<"        { printVerification(); printf("L_SHIFT"); column+=yyleng; }
"="         { printVerification(); printf("ASSIGN"); column+=yyleng; }
"+="        { printVerification(); printf("ADD_ASSIGN"); column+=yyleng; }
"-="        { printVerification(); printf("MINUS_ASSIGN"); column+=yyleng; }
";"         { printVerification(); printf("SEMICOLON"); column+=yyleng; }
","         { printVerification(); printf("COMMA"); column+=yyleng; }
":"         { printVerification(); printf("COLON"); column+=yyleng; }
"("         { printVerification(); printf("L_PAREN"); column+=yyleng; }
")"         { printVerification(); printf("R_PAREN"); column+=yyleng; }
"{"         { printVerification(); printf("L_CURLY_BRACKET"); column+=yyleng; }
"}"         { printVerification(); printf("R_CURLY_BRACKET"); column+=yyleng; }
"["         { printVerification(); printf("L_SQUARE_BRACKET"); column+=yyleng; }
"]"         { printVerification(); printf("R_SQUARE_BRACKET"); column+=yyleng; }
"?"         { printVerification(); printf("TERNARY_CONDITIONAL"); column+=yyleng; }
"#"         { printVerification(); printf("NUMBER_SIGN"); column+=yyleng; }
"->"        { printVerification(); printf("POINTER"); column+=yyleng; }
"printf"    { printVerification(); printf("PRINTF"); column+=yyleng; }
"scanf"     { printVerification(); printf("SCANF"); column+=yyleng; }
"define"    { printVerification(); printf("DEFINE"); column+=yyleng; }
"exit"      { printVerification(); printf("EXIT"); column+=yyleng; }

\\[abfnrtv\\'\"?0]  { printVerification(); printf("CHARACTER(%s)", yytext); column += yyleng; }

\"(\\.|[^"\\])*\"   {
    char* s = treatString(yytext);
    printVerification();
    printf("STRING(%s)", s);
    column+=yyleng;
    free(s);
}

"'"(\\.|[^\\']){1}"'"  {
    char* s = treatString(yytext);
    printVerification();
    printf("CHARACTER(%s)", s);
    column+=yyleng;
    free(s);
}

[a-zA-Z_]+[a-zA-Z0-9_]* {
    if(yyleng > 255) {
        printVerification();
        printf("error:lexical:%d:%d: identifier too long", line, column);
        column+=yyleng;
    } else {
        printVerification();
        printf("IDENTIFIER(%s)", yytext);
        column+=yyleng;
    }
}

0[xX][0-9a-fA-F]+   { printVerification(); printf("NUM_HEXA(%s)", yytext); column+=yyleng; }
0[1-7][0-7]*        { printVerification(); printf("NUM_OCTAL(%s)", yytext); column+=yyleng; }
[0-9]+              { printVerification(); printf("NUM_INTEGER(%s)", yytext); column+=yyleng; }

. { printVerification(); printf("error:lexical:%d:%d: %s", line, column, yytext); column+=yyleng; }


%%


int main(int argc, char** argv) {
    yylex();
    return 0;
}
