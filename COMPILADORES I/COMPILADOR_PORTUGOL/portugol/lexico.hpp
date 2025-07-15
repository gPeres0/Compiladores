#include "automato.hpp"

using namespace std;


class Lexico {
   public:

    int analisadorLexico(string line) {
        int estado_atual = 1;
        int estado_last_final = 0;
        int posicao_estado_last_final = 0;
        string token_atual = "";
        string token_last_final = "";

        for (int i = 0; i < line.length(); i++) {
            int proximo_estado = transicoes[estado_atual][line[i]];
            token_atual += line[i];

            if (proximo_estado != 0 && proximo_estado != 171) {
                estado_atual = proximo_estado;

                if (line[i] == '\n') {
                    linha++;
                    coluna = 1;
                } else {
                    coluna++;
                }
                
                if (estados_finais[estado_atual] == 1) {
                    estado_last_final = estado_atual;
                    posicao_estado_last_final = i;
                    token_last_final = token_atual;
                }

                if (i == line.length() - 1) {
                    if (estados_finais[estado_atual] == 1) {
                        if (token_last_final == " ") {
                            continue;
                        }
                        input.erase(0, posicao_estado_last_final + 1);
                        token_lido = token_last_final;
                        return tokens[estado_last_final];
                    }
                }
            } else {
                if (estado_last_final != 0) {
                    if (token_last_final != " ") {
                        input.erase(0, posicao_estado_last_final + 1);
                        token_lido = token_last_final;
                        return tokens[estado_last_final];
                    }
                } else {
                    if (line[i] == ' ') {
                        estado_atual = 1;
                        estado_last_final = 0;
                        posicao_estado_last_final = 0;
                        token_last_final = "";
                        token_atual = "";
                        coluna++;
                        continue;
                    } else if (line[i] == '\n') {
                        linha++;
                        coluna = 1;
                        input.erase(0, i + 1);
                        token_lido = token_last_final;
                        return OUTRO;
                    }
                    cout << "ERRO LEXICO. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_atual << "\'" << endl;
                    return ERRO;
                }
            }
        }
        return ERRO;
    }
};
