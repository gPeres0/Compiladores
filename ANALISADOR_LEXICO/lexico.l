%option noyywrap


%{
int printed = 0;

void verifyNewLine() {
    printed ? printf("\n") : (printed = 1);
}
%}


%%

// Identificadores: IDENTIFIER
// Inteiros: NUM_INTEGER
// Octais: NUM_OCTAL
// Hexadecimal: NUM_HEXA
// String: STRING
// Caracter: CHARACTER
"v""o""i""d" { verifyNewLine(); printf("VOID"); }               // VOID
"i""n""t" { verifyNewLine(); printf("INT"); }                   // INT
"c""h""a""r" { verifyNewLine(); printf("CHAR"); }               // CHAR
"r""e""t""u""r""n" { verifyNewLine(); printf("RETURN"); }       // RETURN
"b""r""e""a""k" { verifyNewLine(); printf("BREAK"); }           // BREAK
"s""w""i""t""c""h" { verifyNewLine(); printf("SWITCH"); }       // SWITCH
"c""a""s""e" { verifyNewLine(); printf("CASE"); }               // CASE
"d""e""f""a""u""l""t" { verifyNewLine(); printf("DEFAULT"); }   // DEFAULT
"d""o" { verifyNewLine(); printf("DO"); }                       // DO
"w""h""i""l""e" { verifyNewLine(); printf("WHILE"); }           // WHILE
"f""o""r" { verifyNewLine(); printf("FOR"); }                   // FOR
"i""f" { verifyNewLine(); printf("IF"); }                       // IF
"e""l""s""e" { verifyNewLine(); printf("ELSE"); }               // ELSE
"t""y""p""e""d""e""f" { verifyNewLine(); printf("TYPEDEF"); }   // TYPEDEF
"s""t""r""u""c""t" { verifyNewLine(); printf("STRUCT"); }       // STRUCT
"+" { verifyNewLine(); printf("PLUS"); }                        // PLUS
"-" { verifyNewLine(); printf("MINUS"); }                       // MINUS
"*" { verifyNewLine(); printf("MULTIPLY"); }                    // MULTIPLY
"/" { verifyNewLine(); printf("DIV"); }                         // DIV
"%" { verifyNewLine(); printf("REMAINDER"); }                   // REMAINDER
"+""+" { verifyNewLine(); printf("INC"); }                      // INC
"-""-" { verifyNewLine(); printf("DEC"); }                      // DEC
"&" { verifyNewLine(); printf("BITWISE_AND"); }                 // BITWISE_AND
"|" { verifyNewLine(); printf("BITWISE_OR"); }                  // BITWISE_OR
"~" { verifyNewLine(); printf("BITWISE_NOT"); }                 // BITWISE_NOT
"^" { verifyNewLine(); printf("BITWISE_XOR"); }                 // BITWISE_XOR
"!" { verifyNewLine(); printf("NOT"); }                         // NOT
"&""&" { verifyNewLine(); printf("LOGICAL_AND"); }              // LOGICAL_AND
"|""|" { verifyNewLine(); printf("LOGICAL_OR"); }               // LOGICAL_OR
"=""=" { verifyNewLine(); printf("EQUAL"); }                    // EQUAL
"!""=" { verifyNewLine(); printf("NOT_EQUAL"); }                // NOT_EQUAL
"<" { verifyNewLine(); printf("LESS_THAN"); }                   // LESS_THAN
">" { verifyNewLine(); printf("GREATER_THAN"); }                // GREATER_THAN
"<""=" { verifyNewLine(); printf("LESS_EQUAL"); }               // LESS_EQUAL
">""=" { verifyNewLine(); printf("GREATER_EQUAL"); }            // GREATER_EQUAL
">"">" { verifyNewLine(); printf("R_SHIFT"); }                  // R_SHIFT
"<""<" { verifyNewLine(); printf("L_SHIFT"); }                  // L_SHIFT
"=" { verifyNewLine(); printf("ASSIGN"); }                      // ASSIGN
"+""=" { verifyNewLine(); printf("ADD_ASSIGN"); }               // ADD_ASSIGN
"-""=" { verifyNewLine(); printf("MINUS_ASSIGN"); }             // MINUS_ASSIGN
";" { verifyNewLine(); printf("SEMICOLON"); }                   // SEMICOLON
"," { verifyNewLine(); printf("COMMA"); }                       // COMMA
":" { verifyNewLine(); printf("COLON"); }                       // COLON
"(" { verifyNewLine(); printf("L_PARAM"); }                     // L_PARAM
")" { verifyNewLine(); printf("R_PARAM"); }                     // R_PARAM
"{" { verifyNewLine(); printf("L_CURLY_BRACKET"); }             // L_CURLY_BRACKET
"}" { verifyNewLine(); printf("R_CURLY_BRACKET"); }             // R_CURLY_BRACKET
"[" { verifyNewLine(); printf("L_SQUARE_BRACKET"); }            // L_SQUARE_BRACKET
"]" { verifyNewLine(); printf("R_SQUARE_BRACKET"); }            // R_SQUARE_BRACKET
"?" { verifyNewLine(); printf("TERNARY_CONDITIONAL"); }         // TERNARY_CONDITIONAL
"#" { verifyNewLine(); printf("NUMBER_SIGN"); }                 // NUMBER_SIGN
"-"">" { verifyNewLine(); printf("POINTER"); }                  // POINTER
"p""r""i""n""t""f" { verifyNewLine(); printf("PRINTF"); }       // PRINTF
"s""c""a""n""f" { verifyNewLine(); printf("SCANF"); }           // SCANF
"d""e""f""i""n""e" { verifyNewLine(); printf("DEFINE"); }       // DEFINE
"e""x""i""t" { verifyNewLine(); printf("EXIT"); }               // EXIT




[a-hj-z]([0-9]|[a-z])* { verifyNewLine(); printf("%s ID", yytext); }        // ID
"."[0-9]+ { verifyNewLine(); printf("%s REAL", yytext); }                   // REAL
[0-9]+"."[0-9]* { verifyNewLine(); printf("%s REAL", yytext); }             // REAL
[0-9]+ { verifyNewLine(); printf("%s NUM", yytext); }                       // NUM
"-""-"[a-z]+"\n" { verifyNewLine(); notPrintNewLine(yytext); }              // comment
. { verifyNewLine(); printf("%s error", yytext); }                          // error


%%


int main(int argc, char** argv) {
    yylex();

    return 0;
}