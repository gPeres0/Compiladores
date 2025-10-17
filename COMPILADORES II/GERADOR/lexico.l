%option noyywrap

%{
#include "ast.hh"
#include "mips.hh"
#include "helpers.hh"
#include "traversal.hh"
#include "sintatico.tab.hh"
extern YYSTYPE yylval;

bool breakLine = false;
void printLineBreak() {
    if (!breakLine) { breakLine = true; }
    if (breakLine) { 
        printf("\n"); 
        
        breakLine = false;
    }
}
%}

%%
      
<INITIAL>{

" "                     {  }
"AST"                   { return AST; }
"void"                  { return VOID; }
"int"                   { return INT; }
"char"                  { return CHAR; }
"RETURN"                { return RETURN; }
"GLOBAL"                { return GLOBAL; }
"VARIABLE"              { return VARIABLE; }
"TYPE"                  { return TYPE; }
"FUNCTION"              { return FUNCTION; }
"END_FUNCTION"          { return END_FUNCTION; }
"PARAMETER"             { return PARAMETER; }
"RETURN_TYPE"           { return RETURN_TYPE; }
"CONSTANT"              { return CONSTANT; }
"IF"                    { return IF; }
"VALUE"                 { return VALUE; }
"WHILE"                 { return WHILE; }
"FOR"                   { return FOR; }
"DO_WHILE"              { return DO_WHILE; }
"PRINTF"                { return PRINTF; }
"SCANF"                 { return SCANF; }
"EXIT"                  { return EXIT; }
"+"                     { return PLUS; }
"-"                     { return MINUS; }
"*"                     { return MULTIPLY; }
"/"                     { return DIV; }
"%"                     { return REMAINDER; }
"++"                    { return INC; }
"--"                    { return DEC; }
"&"                     { return BITWISE_AND; }
"|"                     { return BITWISE_OR; }
"~"                     { return BITWISE_NOT; }
"^"                     { return BITWISE_XOR; }
"!"                     { return NOT; }
"&&"                    { return LOGICAL_AND; }
"||"                    { return LOGICAL_OR; }
"=="                    { return EQUAL; }
"!="                    { return NOT_EQUAL; }
"<"                     { return LESS_THAN; }
">"                     { return GREATER_THAN; }
"<="                    { return LESS_EQUAL; }
">="                    { return GREATER_EQUAL; }
">>"                    { return R_SHIFT; }
"<<"                    { return L_SHIFT; }
"="                     { return ASSIGN; }
"+="                    { return ADD_ASSIGN; }
"-="                    { return MINUS_ASSIGN; }
";"                     { return SEMICOLON; }
","                     { return COMMA; }
":"                     { return COLON; }
"("                     { return L_PAREN; }
")"                     { return R_PAREN; }
"["                     { return L_SQUARE_BRACKET; }
"]"                     { return R_SQUARE_BRACKET; }
"?"                     { return TERNARY_CONDITIONAL; }
"-->"                   { return START_ARROW; }
"//".*                  { }
[_a-zA-Z][_a-zA-Z0-9]*  { 
                            yylval.string_token = new std::string(yytext);
                            return IDENTIFIER; 
                        }
[0-9][0-9]*             { 
                            yylval.integer_token = atoi(yytext);
                            return NUM_INTEGER;
                        }
\"([^"\\\n]|\\.|\\\n)*\" { 
                            yylval.string_token = new std::string(yytext);
                            return STRING; 
                            
                        }
\'[^'\n]*\'             { 
    
                            yylval.character_token = yytext[1];
                            return CHARACTER;
                        }	
\n                      { return NEWLINE; }
\r
.                       { printf("Unknown: %d"), *yytext; printLineBreak(); exit(0); }
}

%%

void yyerror(char *s) {
    printf("Error: %s\n", s);
}