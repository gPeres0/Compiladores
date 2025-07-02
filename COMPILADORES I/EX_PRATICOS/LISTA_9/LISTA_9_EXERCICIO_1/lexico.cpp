#include "lexico.h"

#include <iostream>
#include <cstring>
#include <cstdlib>
#include "automato.h"
#include "sintatico.h"

using namespace std;

static char ultimo_token_lido[64];
static char ultimo_caractere_lido;
static char inicio_cadeia;

static int quebra_linha;

char* get_ultimo_token_lido() {
    return ultimo_token_lido;
}

char get_ultimo_caractere_lido() {
    return ultimo_caractere_lido;
}

void reset_quebra_linha() {
    quebra_linha = 0;
}

int leu_quebra_linha() {
    return quebra_linha;
}

int get_token() {
    char caractere_lido;
    int estado_atual = 1;
    int cursor_ultimo_final = 0;
    int ultimo_estado_final = 0;
    int cursor_atual = -1;

    memset(ultimo_token_lido, 0, sizeof(ultimo_token_lido));

    formatar_inicio();  

    while (true) {
        caractere_lido = cin.get();
        cursor_atual++;

        ultimo_caractere_lido = caractere_lido;

        if (caractere_lido == '\n') {
            quebra_linha = 1;
        }

        if (cursor_atual == 0) {
            inicio_cadeia = caractere_lido;
        }

        ultimo_token_lido[cursor_atual] = caractere_lido;

        estado_atual = get_proximo_estado(estado_atual, caractere_lido);

        if (estado_atual == 0) {
            if (inicio_cadeia == EOF) {
                return END_OF_FILE;
            }
            if (ultimo_estado_final == 0) {
                cout << "ERRO LEXICO.";
                exit(0);
            } else {
                if (caractere_lido != EOF) {
                    cin.seekg(cursor_ultimo_final - cursor_atual, ios::cur);
                } else {
                    cin.seekg(1, ios::cur);
                }
                ultimo_token_lido[cursor_atual] = '\0';
                return retornar_token(ultimo_estado_final);
            }
        }
        
        if (estado_final(estado_atual)) {
            ultimo_estado_final = estado_atual;
            cursor_ultimo_final = cursor_atual;
        }
    }
}
