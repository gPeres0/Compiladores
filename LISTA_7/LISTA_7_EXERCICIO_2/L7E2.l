%option noyywrap


%{
int printed = 0;

void verifyNewLine() {
    printed ? printf("\n") : (printed = 1);
}

void notPrintNewLine(char* toPrint) {
    toPrint[strlen(toPrint) - 1] = '\0';
    printf("%s comment", toPrint);
}
%}


%%


"i""f" { verifyNewLine(); printf("%s IF", yytext); }
[a-hj-z]([0-9]|[a-z])* { verifyNewLine(); printf("%s ID", yytext); }  // ID
"."[0-9]+ { verifyNewLine(); printf("%s REAL", yytext); }  // REAL
[0-9]+"."[0-9]* { verifyNewLine(); printf("%s REAL", yytext); }  // REAL
[0-9]+ { verifyNewLine(); printf("%s NUM", yytext); }  // NUM
"-""-"[a-z]+"\n" { verifyNewLine(); notPrintNewLine(yytext); }  // comment
" "+ { verifyNewLine(); printf("white space"); }  // white space
. { verifyNewLine(); printf("%s error", yytext); }  // error


%%


int main(int argc, char** argv) {
    yylex();

    return 0;
}