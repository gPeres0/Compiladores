%{
#include <sintatico.tab.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern int yylex();
extern int line;
extern int column;
extern int start_line;
extern char* yytext;
extern bool hadWarning;

int openBrackets = 0;
bool fileEnded = false;

void yyerror(void* s) {
    int errorColumn = column - (int)strlen(yytext);

    if (openBrackets > 0 && fileEnded) {
        printf("error:syntax:%d:%d: expected declaration or statement at end of input", line, errorColumn);
    } else {
        printf("error:syntax:%d:%d: %s", line, errorColumn, yytext);
    }

    fseek(stdin, start_line, SEEK_SET);

    char errorLine[2048];  // Alocação estática segura
    fgets(errorLine, sizeof(errorLine), stdin);
    if (errorLine[strlen(errorLine) - 1] == '\n') {
        errorLine[strlen(errorLine) - 1] = '\0';
    }

    printf("\n%s\n", errorLine);

    for (int i = 0; i < errorColumn - 1; i++) {
        printf(" ");
    }
    printf("^");

    exit(1);
}
%}

/* === Tokens === */

%token VOID INT CHAR RETURN BREAK SWITCH CASE DEFAULT DO WHILE FOR IF ELSE TYPEDEF STRUCT
%token PLUS MINUS MULTIPLY DIV REMAINDER INC DEC
%token BITWISE_AND BITWISE_OR BITWISE_NOT BITWISE_XOR
%token NOT LOGICAL_AND LOGICAL_OR
%token EQUAL NOT_EQUAL LESS_THAN GREATER_THAN LESS_EQUAL GREATER_EQUAL
%token R_SHIFT L_SHIFT
%token ASSIGN ADD_ASSIGN MINUS_ASSIGN
%token SEMICOLON COMMA COLON
%token L_PAREN R_PAREN
%token L_CURLY_BRACKET R_CURLY_BRACKET
%token L_SQUARE_BRACKET R_SQUARE_BRACKET
%token TERNARY_CONDITIONAL
%token NUMBER_SIGN POINTER
%token PRINTF SCANF DEFINE EXIT

%token IDENTIFIER NUM_OCTAL NUM_HEXA NUM_INTEGER STRING CHARACTER

%start programa

%%

/* === Regras Sintáticas === */

programa:
          declaracoes programa            { fileEnded = true; return 1; }
        | funcao programa                 { fileEnded = true; return 1; }
        |
;

declaracoes:
            NUMBER_SIGN DEFINE IDENTIFIER expressao
          | declaracao_variaveis
          | declaracao_prototipos
;

funcao:
          tipo declara_ponteiro IDENTIFIER parametros L_CURLY_BRACKET { openBrackets++; } declara_variaveis comandos R_CURLY_BRACKET { openBrackets--; }
;

declara_ponteiro:
                  MULTIPLY declara_ponteiro
                |
;

declara_variaveis:
                    declaracao_variaveis declara_variaveis
                  |
;

declaracao_variaveis:
                        tipo declaracao_variaveis_prime SEMICOLON
;

declaracao_variaveis_prime:
                              declara_ponteiro IDENTIFIER declara_expressao_colchetes declara_expressao_atribuicao declara_virgula_variaveis
;

declara_expressao_colchetes:
                               L_SQUARE_BRACKET expressao R_SQUARE_BRACKET declara_expressao_colchetes
                             |
;

declara_expressao_atribuicao:
                                ASSIGN expressao_atribuicao
                              |
;

declara_virgula_variaveis:
                             COMMA declaracao_variaveis_prime
                           |
;

declaracao_prototipos:
                         tipo declara_ponteiro IDENTIFIER parametros SEMICOLON
;

parametros:
             L_PAREN declara_parametros R_PAREN
;

declara_parametros:
                     tipo declara_ponteiro IDENTIFIER declara_expressao_colchetes declara_virgula_parametros
                   |
;

declara_virgula_parametros:
                              COMMA declara_parametros
                            |
;

tipo:
        INT
      | CHAR
      | VOID
;

bloco:
         L_CURLY_BRACKET { openBrackets++; } comandos R_CURLY_BRACKET { openBrackets--; }
;

comandos:
           lista_comandos lista_comandos_prime
;

lista_comandos_prime:
                       lista_comandos lista_comandos_prime
                     |
;

lista_comandos:
                 DO bloco WHILE L_PAREN expressao R_PAREN SEMICOLON
               | IF L_PAREN expressao R_PAREN bloco else
               | WHILE L_PAREN expressao R_PAREN bloco
               | FOR L_PAREN expressao_for SEMICOLON expressao_for SEMICOLON expressao_for R_PAREN bloco
               | PRINTF L_PAREN STRING expressao_printf R_PAREN SEMICOLON
               | SCANF L_PAREN STRING COMMA BITWISE_AND IDENTIFIER R_PAREN SEMICOLON
               | EXIT L_PAREN expressao R_PAREN SEMICOLON
               | RETURN expressao_return SEMICOLON
               | expressao SEMICOLON
               | SEMICOLON
               | bloco
;

else:
        ELSE bloco
      |
;

expressao_for:
                expressao
              |
;

expressao_printf:
                   COMMA expressao
                 |
;

expressao_return:
                   expressao
                 |
;

expressao:
            expressao_atribuicao
          | expressao COMMA expressao_atribuicao
;

expressao_atribuicao:
                       expressao_condicional
                     | expressao_unaria atribuicao_simbolo expressao_atribuicao
;

atribuicao_simbolo:
                     ASSIGN
                   | ADD_ASSIGN
                   | MINUS_ASSIGN
;

expressao_condicional:
                        expressao_or_logico
                      | expressao_or_logico TERNARY_CONDITIONAL expressao COLON expressao_condicional
;

expressao_or_logico:
                      expressao_and_logico
                    | expressao_or_logico LOGICAL_OR expressao_and_logico
;

expressao_and_logico:
                       expressao_or
                     | expressao_and_logico LOGICAL_AND expressao_or
;

expressao_or:
               expressao_xor
             | expressao_or BITWISE_OR expressao_xor
;

expressao_xor:
                expressao_and
              | expressao_xor BITWISE_XOR expressao_and
;

expressao_and:
                expressao_igualdade
              | expressao_and BITWISE_AND expressao_igualdade
;

expressao_igualdade:
                      expressao_relacional
                    | expressao_igualdade igualdade_simbolo expressao_relacional
;

igualdade_simbolo:
                    NOT_EQUAL
                  | EQUAL
;

expressao_relacional:
                       expressao_shift
                     | expressao_relacional relacional_simbolo expressao_shift
;

relacional_simbolo:
                     LESS_THAN
                   | LESS_EQUAL
                   | GREATER_THAN
                   | GREATER_EQUAL
;

expressao_shift:
                  expressao_aditiva
                | expressao_shift shift_simbolo expressao_aditiva
;

shift_simbolo:
                L_SHIFT
              | R_SHIFT
;

expressao_aditiva:
                    expressao_multiplicativa
                  | expressao_aditiva aditiva_simbolo expressao_multiplicativa
;

aditiva_simbolo:
                  MINUS
                | PLUS
;

expressao_multiplicativa:
                            expressao_cast
                          | expressao_multiplicativa multiplicativa_simbolo expressao_cast
;

multiplicativa_simbolo:
                          MULTIPLY
                        | DIV
                        | REMAINDER
;

expressao_cast:
                 expressao_unaria
               | L_PAREN tipo declara_ponteiro R_PAREN expressao_cast
;

expressao_unaria:
                   expressao_pos_fixa
                 | unaria_simbolo expressao_unaria
                 | unaria_simbolo_prime expressao_cast
;

unaria_simbolo:
                 INC
               | DEC
;

unaria_simbolo_prime:
                       BITWISE_AND
                     | MULTIPLY
                     | PLUS
                     | MINUS
                     | BITWISE_NOT
                     | NOT
;

expressao_pos_fixa:
                     expressao_primaria
                   | expressao_pos_fixa L_SQUARE_BRACKET expressao R_SQUARE_BRACKET
                   | expressao_pos_fixa INC
                   | expressao_pos_fixa DEC
                   | expressao_pos_fixa L_PAREN pos_fixa_atribuicao R_PAREN
;

pos_fixa_atribuicao:
                      expressao
                    |
;

expressao_primaria:
                     IDENTIFIER
                   | numero
                   | CHARACTER
                   | STRING
                   | L_PAREN expressao R_PAREN
;

numero:
          NUM_INTEGER
        | NUM_HEXA
        | NUM_OCTAL
;

%%

int main(int argc, char** argv) {
    while (yyparse());

    printVerification();
    printf("SUCCESSFUL COMPILATION.");

    return 0;
}
