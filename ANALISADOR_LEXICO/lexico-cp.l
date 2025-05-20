%option noyywrap

/* NOTAS:                                                                           */
/*   - teste10: String não reconheceu o primeiro fechamento de aspas, como deveria  */
/*   - Implementar a lógica de comentários                                          */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int line = 1, column = 1;

char* treatString(const char* str) {
    size_t len = strlen(str);
    /* Aloca len-1 bytes: len-2 caracteres + '\0' */
    char* buf = (char*)malloc(len - 1);
    if (!buf) return NULL;
    memcpy(buf, str + 1, len - 2);
    buf[len - 2] = '\0';
    return buf;
}

%}


%%


\n                  { line++; column = 1; }
[\f\t\r ]+          { line++; column += yyleng; }

"void"      { printf("VOID\n"); line++; column+=yyleng; }
"int"       { printf("INT\n"); line++; column+=yyleng; }
"char"      { printf("CHAR\n"); line++; column+=yyleng; }
"return"    { printf("RETURN\n"); line++; column+=yyleng; }
"break"     { printf("BREAK\n"); line++; column+=yyleng; }
"switch"    { printf("SWITCH\n"); line++; column+=yyleng; }
"case"      { printf("CASE\n"); line++; column+=yyleng; }
"default"   { printf("DEFAULT\n"); line++; column+=yyleng; }
"do"        { printf("DO\n"); line++; column+=yyleng; }
"while"     { printf("WHILE\n"); line++; column+=yyleng; }
"for"       { printf("FOR\n"); line++; column+=yyleng; }
"if"        { printf("IF\n"); line++; column+=yyleng; }
"else"      { printf("ELSE\n"); line++; column+=yyleng; }
"typedef"   { printf("TYPEDEF\n"); line++; column+=yyleng; }
"struct"    { printf("STRUCT\n"); line++; column+=yyleng; }
"+"         { printf("PLUS\n"); line++; column+=yyleng; }
"-"         { printf("MINUS\n"); line++; column+=yyleng; }
"*"         { printf("MULTIPLY\n"); line++; column+=yyleng; }
"/"         { printf("DIV\n"); line++; column+=yyleng; }
"%"         { printf("REMAINDER\n"); line++; column+=yyleng; }
"++"        { printf("INC\n"); line++; column+=yyleng; }
"--"        { printf("DEC\n"); line++; column+=yyleng; }
"&"         { printf("BITWISE_AND\n"); line++; column+=yyleng; }
"|"         { printf("BITWISE_OR\n"); line++; column+=yyleng; }
"~"         { printf("BITWISE_NOT\n"); line++; column+=yyleng; }
"^"         { printf("BITWISE_XOR\n"); line++; column+=yyleng; }
"!"         { printf("NOT\n"); line++; column+=yyleng; }
"&&"        { printf("LOGICAL_AND\n"); line++; column+=yyleng; }
"||"        { printf("LOGICAL_OR\n"); line++; column+=yyleng; }
"=="        { printf("EQUAL\n"); line++; column+=yyleng; }
"!="        { printf("NOT_EQUAL\n"); line++; column+=yyleng; }
"<"         { printf("LESS_THAN\n"); line++; column+=yyleng; }
">"         { printf("GREATER_THAN\n"); line++; column+=yyleng; }
"<="        { printf("LESS_EQUAL\n"); line++; column+=yyleng; }
">="        { printf("GREATER_EQUAL\n"); line++; column+=yyleng; }
">>"        { printf("R_SHIFT\n"); line++; column+=yyleng; }
"<<"        { printf("L_SHIFT\n"); line++; column+=yyleng; }
"="         { printf("ASSIGN\n"); line++; column+=yyleng; }
"+="        { printf("ADD_ASSIGN\n"); line++; column+=yyleng; }
"-="        { printf("MINUS_ASSIGN\n"); line++; column+=yyleng; }
";"         { printf("SEMICOLON\n"); line++; column+=yyleng; }
","         { printf("COMMA\n"); line++; column+=yyleng; }
":"         { printf("COLON\n"); line++; column+=yyleng; }
"("         { printf("L_PARAM\n"); line++; column+=yyleng; }
")"         { printf("R_PARAM\n"); line++; column+=yyleng; }
"{"         { printf("L_CURLY_BRACKET\n"); line++; column+=yyleng; }
"}"         { printf("R_CURLY_BRACKET\n"); line++; column+=yyleng; }
"["         { printf("L_SQUARE_BRACKET\n"); line++; column+=yyleng; }
"]"         { printf("R_SQUARE_BRACKET\n"); line++; column+=yyleng; }
"?"         { printf("TERNARY_CONDITIONAL\n"); line++; column+=yyleng; }
"#"         { printf("NUMBER_SIGN\n"); line++; column+=yyleng; }
"->"        { printf("POINTER\n"); line++; column+=yyleng; }
"printf"    { printf("PRINTF\n"); line++; column+=yyleng; }
"scanf"     { printf("SCANF\n"); line++; column+=yyleng; }
"define"    { printf("DEFINE\n"); line++; column+=yyleng; }
"exit"      { printf("EXIT\n"); line++; column+=yyleng; }

\\[abfnrtv\\'\"?0]  { printf("CHARACTER(%s)\n", yytext); line++; column += yyleng; }

\".+\"  {
    char* s = treatString(yytext);
    printf("STRING(%s)\n", s);
    line++; column+=yyleng;

    free(s);
}

\'.+\'  {
    char* s = treatString(yytext);
    printf("CHARACTER(%s)\n", s);
    line++; column+=yyleng;
    
    free(s);
}

("_"|[a-zA-Z]+)[a-zA-Z0-9]* {
    if(yyleng > 255) {
        fprintf(stderr, "error:lexical:%d:%d: identifier too long\n", line, column);
        line++;
        column+=yyleng;
    } else {
        printf("IDENTIFIER(%s)\n", yytext);
        line++;
        column+=yyleng;
    }
}

0[xX][0-9a-fA-F]+   { printf("NUM_HEXA(%s)\n", yytext); line++; column+=yyleng; }
0[1-7][0-7]*        { printf("NUM_OCTAL(%s)\n", yytext); line++; column+=yyleng; }
[0-9]+              { printf("NUM_INTEGER(%s)\n", yytext); line++; column+=yyleng; }

. { fprintf(stderr, "error:lexical:%d:%d: %s\n", line, column, yytext); line++; column+=yyleng; }


%%


int main(int argc, char** argv) {
    yylex();
    return 0;
}
