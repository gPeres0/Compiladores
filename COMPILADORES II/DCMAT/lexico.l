%option noyywrap


%{
#include "definitions.h"
#include <sintatico.tab.h>
#include <stdbool.h>

bool breakLine = false;

void printLineBreak() {
    if (!breakLine) { breakLine = true; }
    if (breakLine) { 
        printf("\n"); 
        
        breakLine = false;
    }
}

void invalidSymbol() {
    printLineBreak();
    printf("Invalid Symbol: %s\n", yytext);
    printLineBreak();
}
%}


%%


"about"             { return ABOUT; }
"abs"               { return ABS; }
"axis"              { return AXIS; }
"connect_dots"      { return CONNECT_DOTS; }
"cos"               { return COS; }
"determinant"       { return DETERMINANT; }
"e"|"E"             { return EULER; }
"erase"             { return ERASE; }
"float"             { return FLOAT; }
"h_view"            { return H_VIEW; }
"integral_steps"    { return INTEGRAL_STEPS; }
"integrate"         { return INTEGRATE; }
"linear_system"     { return LINEAR_SYSTEM; }
"matrix"            { return MATRIX; }
"off"               { return OFF; }
"on"                { return ON; }
("p"|"P")("i"|"I")  { return PI; }
"plot"              { return PLOT; }
"precision"         { return PRECISION; }
"quit"              { return QUIT; }
"reset"             { return RESET; }
"rpn"               { return RPN; }
"sen"               { return SEN; }
"set"               { return SET; }
"settings"          { return SETTINGS; }
"show"              { return SHOW; }
"solve"             { return SOLVE; }
"sum"               { return SUM; }
"symbols"           { return SYMBOLS; }
"tan"               { return TAN; }
"v_view"            { return V_VIEW; }
"x"                 { yylval.name = strdup(yytext); return X; }

"+"     { return PLUS; }
"-"     { return MINUS; }
"*"     { return TIMES; }
"/"     { return DIVIDE; }
"^"     { return POWER; }
"%"     { return REMAINDER; }
"("     { return L_PAREN; }
")"     { return R_PAREN; }
":"     { return COLON; }
"="     { return EQUAL; }
":="    { return ASSIGN; }
"["     { return L_BRACKET; }
"]"     { return R_BRACKET; }
";"     { return SEMICOLON; }
","     { return COMMA; }

[0-9]+                  { yylval.intValue = atoi(yytext); return NUM_INT; }
[0-9]+"."[0-9]+         { yylval.floatValue = atof(yytext); return NUM_FLOAT; }
[a-zA-Z][_0-9a-zA-Z]*   { yylval.name = strdup(yytext); return ID;}

"\n"    { return EOL; }
" "     {  }

.   { invalidSymbol(); } 


%%
