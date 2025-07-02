%option noyywrap


%{
int printed = 0;

void verifiedPrint(char* toPrint) {
    printed ? printf("\n") : (printed = 1);
    printf("%s", toPrint);
}
%}


%%


"aba"+ { verifiedPrint(yytext); printf(" (action 1)"); }
"a""b"*"a" { verifiedPrint(yytext); printf(" (action 2)"); }
"a"|"b" { verifiedPrint(yytext); printf(" (action 3)"); }


%%


int main(int argc, char** argv) {
    yylex();

    return 0;
}