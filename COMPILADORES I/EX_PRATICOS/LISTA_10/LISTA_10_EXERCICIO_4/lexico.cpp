#include "lexico.h"

using namespace std;

queue<Token> lexicalParse(string entrada, vector<Estado> automata){

    queue<Token> tokens;

    int estadoInicial = 1;

    int ultimoFinal = 0,
            estadoAtual = estadoInicial;

    int cursorInicialCadeia = 0,
            cursorAtual = 0,
            cursorFinalReconhecido = 0;

    int tamanhoString = entrada.length();

    while (cursorAtual <= tamanhoString && cursorInicialCadeia <= tamanhoString-1) {

        bool prosseguirCadeia = true;

        while (prosseguirCadeia) {
            cursorAtual++;

            if (cursorAtual > tamanhoString) {
                if (automata[estadoAtual].isFinal()) {
                    int tamanhoDaCadeia = abs(cursorInicialCadeia - cursorFinalReconhecido);
                    if (tamanhoDaCadeia > 0) {
                        tokens.push(Token(entrada.substr(cursorInicialCadeia, tamanhoDaCadeia), automata[ultimoFinal].getType()));

                    } else {
                        throw LexicalException(entrada.at(cursorAtual-1));
                    }

                } else {
                    if(cursorAtual - cursorFinalReconhecido != 1)
                        throw LexicalException(entrada.at(cursorAtual-1));
                }

                return tokens;
            }

            char leitura = entrada.at(cursorAtual - 1);
            estadoAtual = automata[estadoAtual].getTransition(leitura);

            if (estadoAtual == 0) {
                prosseguirCadeia = false;
                int tamanhoDaCadeia = abs(cursorInicialCadeia - cursorFinalReconhecido);
                if (tamanhoDaCadeia > 0) {
                    tokens.push(Token(entrada.substr(cursorInicialCadeia, tamanhoDaCadeia), automata[ultimoFinal].getType()));

                }else {
                    if (cursorAtual - cursorInicialCadeia == 1 && (entrada.at(cursorAtual - 1) == ' ' || entrada.at(cursorAtual - 1) == '\n'));
                    else
                        throw LexicalException(entrada.at(cursorAtual-1));

                    if (cursorFinalReconhecido == cursorInicialCadeia) {
                        cursorFinalReconhecido++;
                    } else {
                        cursorFinalReconhecido = cursorAtual;
                    }
                }

                continue;
            }

            if (automata[estadoAtual].isFinal()) {
                cursorFinalReconhecido = cursorAtual;
                ultimoFinal = estadoAtual;
            }
        }

        estadoAtual = estadoInicial;
        cursorInicialCadeia = cursorFinalReconhecido;
        cursorAtual = cursorFinalReconhecido;
    }


}