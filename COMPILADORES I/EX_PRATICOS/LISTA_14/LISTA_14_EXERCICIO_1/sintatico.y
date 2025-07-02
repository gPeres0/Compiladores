%{
    #include <stdio.h>
    #include <stdbool.h>
    #include <string.h>
    #include <stdlib.h>
    
    extern int yylex();
    extern char *yytext;
    void yyerror(char const *s);

    bool first = true;

    extern unsigned columnCounter;

    bool error = false;
    extern bool lexicalError;
    char strError[50];

    extern char *wrongChars;
    extern int wrongCharsCount;

    extern bool finishing;
%}

%token ADD
%token SUB
%token MUL
%token DIV
%token POWER
%token MODULE
%token L_PARENT
%token R_PARENT
%token SEN
%token COS
%token TAN
%token ABS
%token NUM
%token VAR
%token EOL

%start calclist

%%

calclist: exp EOL {
    if(!error && !lexicalError) {
        if(first) {
            printf("EXPRESSAO CORRETA");
            first = false;
        }
        else {
            printf("\nEXPRESSAO CORRETA");
        }
    }
    
    return 1;
}
;

exp: factor         {}
   | exp ADD factor {}
   | exp SUB factor {}
;

factor: signal term               {}
      | factor MUL signal term    {}
      | factor DIV signal term    {}
      | factor POWER term         {}
      | factor MODULE signal term {}
;

term: NUM                       {}
    | VAR                       {}
    | L_PARENT exp R_PARENT     {}
    | SEN L_PARENT exp R_PARENT {}
    | COS L_PARENT exp R_PARENT {}
    | TAN L_PARENT exp R_PARENT {}
    | ABS L_PARENT exp R_PARENT {}
;

signal: ADD {}
      | SUB {}
      |     {}
;

%%

void yyerror(char const *s) {
    if(!error && !lexicalError) {
        unsigned columnNumber = columnCounter - strlen(yytext);

        if(yytext[0] != '\n' && yytext[0] != '\0') {
            sprintf(strError, "Erro de sintaxe na coluna [%u]: %s", columnNumber, yytext);
        }
        else {
            sprintf(strError, "A expressao terminou de forma inesperada.");
        }

        error = true;
    }
}

int main() {
    fseek(stdin, 0, SEEK_END);
    int fLen = ftell(stdin);
    rewind(stdin);

    wrongChars = (char *) calloc(fLen, sizeof(char));

    while(!finishing) {
        yyparse();

        if(yytext[0] != '\n') {
            while(yylex() != EOL);
        }

        if(lexicalError) {
            if(first) {
                printf("Caractere(s) invalido(s) -> [");
                first = false;
            }
            else {
                printf("\nCaractere(s) invalido(s) -> [");
            }
            for(int i = 0; i < wrongCharsCount - 1; i++) {
                printf("%c,", wrongChars[i]);
            }
            printf("%c]", wrongChars[wrongCharsCount - 1]);

            memset(wrongChars, 0, sizeof(wrongChars));
            wrongCharsCount = 0;
            lexicalError = false;
        }
        else if(error) {
            if(first) {
                printf("%s", strError);
                first = false;
            }
            else {
                printf("\n%s", strError);
            }
        }

        error = false;
    };

    return 0;
}