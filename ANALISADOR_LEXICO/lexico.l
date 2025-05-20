%option noyywrap


%{
#include <bits/stdc++.h>

int line = 1, column = 1;

std::string treatString(std::string str) {
    std::string treated_str = str.substr(1, str.length() - 2);
    return treated_str;
}

%}


%%

\n                  { line++; column = 1; }
[\f\t\r ]+          { line++; column += yyleng; }

"void"      { std::cout << "VOID" << std::endl; line++; column+=yyleng; }                   // VOID
"int"       { std::cout << "INT" << std::endl; line++; column+=yyleng; }                    // INT
"char"      { std::cout << "CHAR" << std::endl; line++; column+=yyleng; }                   // CHAR
"return"    { std::cout << "RETURN" << std::endl; line++; column+=yyleng; }                 // RETURN
"break"     { std::cout << "BREAK" << std::endl; line++; column+=yyleng; }                  // BREAK
"switch"    { std::cout << "SWITCH" << std::endl; line++; column+=yyleng; }                 // SWITCH
"case"      { std::cout << "CASE" << std::endl; line++; column+=yyleng; }                   // CASE
"default"   { std::cout << "DEFAULT" << std::endl; line++; column+=yyleng; }                // DEFAULT
"do"        { std::cout << "DO" << std::endl; line++; column+=yyleng; }                     // DO
"while"     { std::cout << "WHILE" << std::endl; line++; column+=yyleng; }                  // WHILE
"for"       { std::cout << "FOR" << std::endl; line++; column+=yyleng; }                    // FOR
"if"        { std::cout << "IF" << std::endl; line++; column+=yyleng; }                     // IF
"else"      { std::cout << "ELSE" << std::endl; line++; column+=yyleng; }                   // ELSE
"typedef"   { std::cout << "TYPEDEF" << std::endl; line++; column+=yyleng; }                // TYPEDEF
"struct"    { std::cout << "STRUCT" << std::endl; line++; column+=yyleng; }                 // STRUCT
"+"         { std::cout << "PLUS" << std::endl; line++; column+=yyleng; }                   // PLUS
"-"         { std::cout << "MINUS" << std::endl; line++; column+=yyleng; }                  // MINUS
"*"         { std::cout << "MULTIPLY" << std::endl; line++; column+=yyleng; }               // MULTIPLY
"/"         { std::cout << "DIV" << std::endl; line++; column+=yyleng; }                    // DIV
"%"         { std::cout << "REMAINDER" << std::endl; line++; column+=yyleng; }              // REMAINDER
"++"        { std::cout << "INC" << std::endl; line++; column+=yyleng; }                    // INC
"--"        { std::cout << "DEC" << std::endl; line++; column+=yyleng; }                    // DEC
"&"         { std::cout << "BITWISE_AND" << std::endl; line++; column+=yyleng; }            // BITWISE_AND
"|"         { std::cout << "BITWISE_OR" << std::endl; line++; column+=yyleng; }             // BITWISE_OR
"~"         { std::cout << "BITWISE_NOT" << std::endl; line++; column+=yyleng; }            // BITWISE_NOT
"^"         { std::cout << "BITWISE_XOR" << std::endl; line++; column+=yyleng; }            // BITWISE_XOR
"!"         { std::cout << "NOT" << std::endl; line++; column+=yyleng; }                    // NOT
"&&"        { std::cout << "LOGICAL_AND" << std::endl; line++; column+=yyleng; }            // LOGICAL_AND
"||"        { std::cout << "LOGICAL_OR" << std::endl; line++; column+=yyleng; }             // LOGICAL_OR
"=="        { std::cout << "EQUAL" << std::endl; line++; column+=yyleng; }                  // EQUAL
"!="        { std::cout << "NOT_EQUAL" << std::endl; line++; column+=yyleng; }              // NOT_EQUAL
"<"         { std::cout << "LESS_THAN" << std::endl; line++; column+=yyleng; }              // LESS_THAN
">"         { std::cout << "GREATER_THAN" << std::endl; line++; column+=yyleng; }           // GREATER_THAN
"<="        { std::cout << "LESS_EQUAL" << std::endl; line++; column+=yyleng; }             // LESS_EQUAL
">="        { std::cout << "GREATER_EQUAL" << std::endl; line++; column+=yyleng; }          // GREATER_EQUAL
">>"        { std::cout << "R_SHIFT" << std::endl; line++; column+=yyleng; }                // R_SHIFT
"<<"        { std::cout << "L_SHIFT" << std::endl; line++; column+=yyleng; }                // L_SHIFT
"="         { std::cout << "ASSIGN" << std::endl; line++; column+=yyleng; }                 // ASSIGN
"+="        { std::cout << "ADD_ASSIGN" << std::endl; line++; column+=yyleng; }             // ADD_ASSIGN
"-="        { std::cout << "MINUS_ASSIGN" << std::endl; line++; column+=yyleng; }           // MINUS_ASSIGN
";"         { std::cout << "SEMICOLON" << std::endl; line++; column+=yyleng; }              // SEMICOLON
","         { std::cout << "COMMA" << std::endl; line++; column+=yyleng; }                  // COMMA
":"         { std::cout << "COLON" << std::endl; line++; column+=yyleng; }                  // COLON
"("         { std::cout << "L_PARAM" << std::endl; line++; column+=yyleng; }                // L_PARAM
")"         { std::cout << "R_PARAM" << std::endl; line++; column+=yyleng; }                // R_PARAM
"{"         { std::cout << "L_CURLY_BRACKET" << std::endl; line++; column+=yyleng; }        // L_CURLY_BRACKET
"}"         { std::cout << "R_CURLY_BRACKET" << std::endl; line++; column+=yyleng; }        // R_CURLY_BRACKET
"["         { std::cout << "L_SQUARE_BRACKET" << std::endl; line++; column+=yyleng; }       // L_SQUARE_BRACKET
"]"         { std::cout << "R_SQUARE_BRACKET" << std::endl; line++; column+=yyleng; }       // R_SQUARE_BRACKET
"?"         { std::cout << "TERNARY_CONDITIONAL" << std::endl; line++; column+=yyleng; }    // TERNARY_CONDITIONAL
"#"         { std::cout << "NUMBER_SIGN" << std::endl; line++; column+=yyleng; }            // NUMBER_SIGN
"->"        { std::cout << "POINTER" << std::endl; line++; column+=yyleng; }                // POINTER
"printf"    { std::cout << "PRINTF" << std::endl; line++; column+=yyleng; }                 // PRINTF
"scanf"     { std::cout << "SCANF" << std::endl; line++; column+=yyleng; }                  // SCANF
"define"    { std::cout << "DEFINE" << std::endl; line++; column+=yyleng; }                 // DEFINE
"exit"      { std::cout << "EXIT" << std::endl; line++; column+=yyleng; }                   // EXIT

\\[abfnrtv\\'\"?0]  { std::cout << "CHARACTER(" << yytext << ")" << std::endl; line++; column += yyleng; }     // Caracter: CHARACTER

\".+\"  { std::cout << "STRING(" << treatString(yytext) << ")" << std::endl; line++; column+=yyleng; }          // String: STRING

\'.+\'  { std::cout << "CHARACTER(" << treatString(yytext) << ")" << std::endl; line++; column+=yyleng; }       // Caracter: CHARACTER

("_"|[a-zA-Z]+)[a-zA-Z0-9]* {                                                                                   // Identificadores: IDENTIFIER 
    if(yyleng > 255) {
        std::out << "error:lexical:" << line << ":" << column << ": " << "identifier too long" << std::endl; 
        line++; 
        column+=yyleng;
    } else {
        std::cout << "IDENTIFIER(" << yytext << ")" << std::endl; 
        line++; 
        column+=yyleng;
    }
}  

0[xX][0-9a-fA-F]+   { std::cout << "NUM_HEXA(" << yytext << ")" << std::endl; line++; column+=yyleng; }     // Hexadecimal: NUM_HEXA
0[1-7][0-7]         { std::cout << "NUM_OCTAL(" << yytext << ")" << std::endl; line++; column+=yyleng; }    // Octais: NUM_OCTAL
[0-9]+              { std::cout << "NUM_INTEGER(" << yytext << ")" << std::endl; line++; column+=yyleng; }  // Inteiros: NUM_INTEGER

. { std::cout << "error:lexical:" << line << ":" << column << ": " << yytext << std::endl; line++; column+=yyleng; }   // error:lexical


%%


int main(int argc, char** argv) {
    yylex();

    return 0;
}