#include "sintatico.hpp"
#include "automato.hpp"
#include "lexico.hpp"

using namespace std;

int main () {
    input = "";
    char c;
    while ((c = getchar()) != EOF) {
        input += c;
    }

    Lexico lexico;
    Sintatico sintatico;

    int programa_correto = 1;
    
    s.push(".");
    s.push("BlocoComandos");
    s.push("ProcedimentoFuncao");
    s.push("BlocoVariaveis");
    s.push(";");
    s.push("id");
    s.push("algoritmo");

    while(input.size() > 0) {
        if(pedir_novo_token) {
            valor_token_lido = lexico.analisadorLexico(input);

            if(valor_token_lido == 0) {
                programa_correto = 0;
                break;
            } else if (valor_token_lido == 54 || valor_token_lido == 55 || valor_token_lido == 57) {
                continue;
            }
        }

        sintatico.analisadorSintatico(valor_token_lido);
    }

    if(programa_correto) { cout << "PROGRAMA CORRETO." << endl; }

    return 0;
}