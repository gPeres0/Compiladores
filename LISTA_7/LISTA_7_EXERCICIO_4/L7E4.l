%option noyywrap


%{
int totalLines = 0;
int blankLines = 0;
int linesWComments = 0;
int nBNCLines = 0;
int semiColons = 0;
int preprocDirect = 0;
%}


%%


"#define"|"#include" { preprocDirect++; }
"//" { linesWComments++; }
"/*" { linesWComments++; }
"*/" { linesWComments++; }
";\n" { semiColons++; totalLines++; }
";" { semiColons++; }
."\n" { totalLines++; }
"\n" { blankLines++; totalLines++; }
. { }


%%


int main(int argc, char** argv) {
    printf(" total    blank lines w/   nb, nc    semi- preproc. file\n");
    printf(" lines    lines comments    lines   colons  direct.\n");
    printf("------+--------+--------+--------+--------+--------+----\n");

    if (argc < 2) {
        yylex();
        printf(" total    blank lines w/   nb, nc    semi- preproc. file\n");
        printf(" lines    lines comments    lines   colons  direct.\n");
        printf("------+--------+--------+--------+--------+--------+----\n");
        printf("%6d %8d %8d %8d %8d %8d %s \n", totalLines, blankLines, linesWComments, nBNCLines, semiColons, preprocDirect, argv[1]);
    }
    else {
        int sumTotalLines = 0;
        int sumBlankLines = 0;
        int sumLinesWComments = 0;
        int sumNBNCLines = 0;
        int sumSemiColons = 0;
        int sumPreprocDirect = 0;

        for (int i = 1; i < argc; i++) {
            FILE* f = fopen(argv[i], "r");

            if (!f) {
                perror(argv[i]);
                return 1;
            }

            yyrestart(f);
            yylex();
            fclose(f);

            nBNCLines = totalLines - blankLines - linesWComments;

            printf("%6d %8d %8d %8d %8d %8d %s\n", totalLines, blankLines, linesWComments, nBNCLines, semiColons, preprocDirect, argv[i]);

            sumTotalLines += totalLines; totalLines = 0;
            sumBlankLines += blankLines; blankLines = 0;
            sumLinesWComments += linesWComments; linesWComments = 0;
            sumNBNCLines += nBNCLines; nBNCLines = 0;
            sumSemiColons += semiColons; semiColons = 0;
            sumPreprocDirect += preprocDirect; preprocDirect = 0;
        }

        printf("%6d %8d %8d %8d %8d %8d total", sumTotalLines, sumBlankLines, sumLinesWComments, sumNBNCLines, sumSemiColons, sumPreprocDirect);
    }

    return 0;
}