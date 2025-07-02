%option noyywrap


%{
int printed = 0;

void verifiedPrint(char* toPrint) {
    printed ? printf("\n") : (printed = 1);
    printf("%s", toPrint);
}
%}


%%


"\n" { }
"+" { verifiedPrint("+"); }
"-" { verifiedPrint("-"); }
[a-z]([0-9]|[a-z])* { verifiedPrint(yytext); }
[0-9]+"e"(([0-9]+)|(("+"|"-")[0-9]+)) { verifiedPrint(yytext); }
[0-9]+ { verifiedPrint(yytext); }
. { verifiedPrint("ERRO"); }


%%


int main(int argc, char** argv) {
    yylex();

    return 0;
}