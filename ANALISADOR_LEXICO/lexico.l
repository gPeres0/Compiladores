%option noyywrap


%{
#include <bits/stdc++.h>

string getLineColCoord() {
    // Função para encontrar coordenadas de linha e coluna para erro
    int line, column;
    return "line:column";
}
%}


%%


/*  Implementar máximo de 255 caracteres para Identificadores   */
"_"|[a-zA-Z]+[a-zA-Z0-9]* { std::cout << "IDENTIFIER(" << yytext << ")"; }  // Identificadores: IDENTIFIER

/*  Alterar posição por conta da prioridade */
[0-9]+ { std::cout << "NUM_INTEGER(" << yytext << ")"; }                    // Inteiros: NUM_INTEGER

/*   Verificar prioridade   */
0[1-7][0-7] { std::cout << "NUM_OCTAL(" << yytext << ")"; }                 // Octais: NUM_OCTAL

/*   Verificar prioridade   */
0[xX][0-9a-fA-F]+ { std::cout << "NUM_HEXA(" << yytext << ")"; }            // Hexadecimal: NUM_HEXA

/*  Alterar posição por conta da prioridade
    Verificar se a ER está correta          */
""".+""" { std::cout << "STRING(" << yytext << ")"; }                       // String: STRING

/*   Verificar prioridade   */
"'".+"'" { std::cout << "CHARACTER(" << yytext << ")"; }                    // Caracter: CHARACTER

"void" { std::cout << "VOID"; }                                     // VOID
"int" { std::cout << "INT"; }                                       // INT
"char" { std::cout << "CHAR"; }                                     // CHAR
"return" { std::cout << "RETURN"; }                                 // RETURN
"break" { std::cout << "BREAK"; }                                   // BREAK
"switch" { std::cout << "SWITCH"; }                                 // SWITCH
"case" { std::cout << "CASE"; }                                     // CASE
"default" { std::cout << "DEFAULT"; }                               // DEFAULT
"do" { std::cout << "DO"; }                                         // DO
"while" { std::cout << "WHILE"; }                                   // WHILE
"for" { std::cout << "FOR"; }                                       // FOR
"if" { std::cout << "IF"; }                                         // IF
"else" { std::cout << "ELSE"; }                                     // ELSE
"typedef" { std::cout << "TYPEDEF"; }                               // TYPEDEF
"struct" { std::cout << "STRUCT"; }                                 // STRUCT
"+" { std::cout << "PLUS"; }                                        // PLUS
"-" { std::cout << "MINUS"; }                                       // MINUS
"*" { std::cout << "MULTIPLY"; }                                    // MULTIPLY
"/" { std::cout << "DIV"; }                                         // DIV
"%" { std::cout << "REMAINDER"; }                                   // REMAINDER
"++" { std::cout << "INC"; }                                        // INC
"--" { std::cout << "DEC"; }                                        // DEC
"&" { std::cout << "BITWISE_AND"; }                                 // BITWISE_AND
"|" { std::cout << "BITWISE_OR"; }                                  // BITWISE_OR
"~" { std::cout << "BITWISE_NOT"; }                                 // BITWISE_NOT
"^" { std::cout << "BITWISE_XOR"; }                                 // BITWISE_XOR
"!" { std::cout << "NOT"; }                                         // NOT
"&&" { std::cout << "LOGICAL_AND"; }                                // LOGICAL_AND
"||" { std::cout << "LOGICAL_OR"; }                                 // LOGICAL_OR
"==" { std::cout << "EQUAL"; }                                      // EQUAL
"!=" { std::cout << "NOT_EQUAL"; }                                  // NOT_EQUAL
"<" { std::cout << "LESS_THAN"; }                                   // LESS_THAN
">" { std::cout << "GREATER_THAN"; }                                // GREATER_THAN
"<=" { std::cout << "LESS_EQUAL"; }                                 // LESS_EQUAL
">=" { std::cout << "GREATER_EQUAL"; }                              // GREATER_EQUAL
">>" { std::cout << "R_SHIFT"; }                                    // R_SHIFT
"<<"" { std::cout << "L_SHIFT"; }                                    // L_SHIFT
"=" { std::cout << "ASSIGN"; }                                      // ASSIGN
"+=" { std::cout << "ADD_ASSIGN"; }                                 // ADD_ASSIGN
"-=" { std::cout << "MINUS_ASSIGN"; }                               // MINUS_ASSIGN
";" { std::cout << "SEMICOLON"; }                                   // SEMICOLON
"," { std::cout << "COMMA"; }                                       // COMMA
":" { std::cout << "COLON"; }                                       // COLON
"(" { std::cout << "L_PARAM"; }                                     // L_PARAM
")" { std::cout << "R_PARAM"; }                                     // R_PARAM
"{" { std::cout << "L_CURLY_BRACKET"; }                             // L_CURLY_BRACKET
"}" { std::cout << "R_CURLY_BRACKET"; }                             // R_CURLY_BRACKET
"[" { std::cout << "L_SQUARE_BRACKET"; }                            // L_SQUARE_BRACKET
"]" { std::cout << "R_SQUARE_BRACKET"; }                            // R_SQUARE_BRACKET
"?" { std::cout << "TERNARY_CONDITIONAL"; }                         // TERNARY_CONDITIONAL
"#" { std::cout << "NUMBER_SIGN"; }                                 // NUMBER_SIGN
"->" { std::cout << "POINTER"; }                                    // POINTER
"printf" { std::cout << "PRINTF"; }                                 // PRINTF
"scanf" { std::cout << "SCANF"; }                                   // SCANF
"define" { std::cout << "DEFINE"; }                                 // DEFINE
"exit" { std::cout << "EXIT"; }                                     // EXIT

. { verifyNewLine; std::cout << "error:lexical:" << getLineColCoord << ":" << yytext; }   // error:lexical

/*
[a-hj-z]([0-9]|[a-z])* { printf("%s ID", yytext); }        // ID
"."[0-9]+ { printf("%s REAL", yytext); }                   // REAL
[0-9]+"."[0-9]* { printf("%s REAL", yytext); }             // REAL
[0-9]+ { printf("%s NUM", yytext); }                       // NUM
"--"[a-z]+"\n" { notPrintNewLine(yytext); }              // comment
. { printf("%s error", yytext); }                          // error
*/


%%


int main(int argc, char** argv) {
    yylex();

    return 0;
}