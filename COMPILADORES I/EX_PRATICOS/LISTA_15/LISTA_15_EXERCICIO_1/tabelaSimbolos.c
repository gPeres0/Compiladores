#include "tabelaSimbolos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO 211

typedef struct structNoLista {
    char tipo[8];
    char nome[16];
    struct structNoLista *proximo;
} *PonteiroNoLista;

typedef struct structHashTable {
    int flagErro;
    PonteiroNoLista *listas;
} *PonteiroTabelaHash;

long funcaoHash(char *nome) {
    int i;
    long index;
    index = 0;
    for (i = 0; i < strlen(nome); i++) {
        index += nome[i];
    }
    return index % TAMANHO;
}

TabelaHash criarTabelaSimbolos() {
    int i;
    PonteiroTabelaHash tabelaHash = malloc(sizeof(struct structHashTable));
    tabelaHash->listas = malloc(TAMANHO * sizeof(struct structNoLista));
    for (i = 0; i < TAMANHO; i++) tabelaHash->listas[i] = NULL;
    tabelaHash->flagErro = 0;
    return tabelaHash;
}

void inserirTabelaSimbolos(TabelaHash tabelaHashParametro, char *tipo, char *nome, int linhaAtual) {
    long index;
    PonteiroTabelaHash tabelaHash = (PonteiroTabelaHash) tabelaHashParametro;
    PonteiroNoLista novoNo = malloc(sizeof(struct structNoLista));
    strcpy(novoNo->tipo, tipo);
    strcpy(novoNo->nome, nome);
    novoNo->proximo = NULL;

    index = funcaoHash(nome);

    if (tabelaHash->listas[index] == NULL) {
        tabelaHash->listas[index] = novoNo;
    } else {
        PonteiroNoLista noAuxiliar = tabelaHash->listas[index];
        while (1) {
            if (strcmp(nome, noAuxiliar->nome) == 0) {
                tabelaHash->flagErro = 1;
                free(novoNo);
                if (linhaAtual > 1) {
                    printf("\n");
                }
                if (strcmp(tipo, noAuxiliar->tipo) == 0) {
                    // Erro: Ja existe mesmo nome mesmo tipo
                    printf("%d: identifier '%s' already declared", linhaAtual, nome);
                    return;
                }
                // Erro: Ja existe mesmo nome tipo diferente
                printf("%d: redefinition of identifier '%s'", linhaAtual, nome);
                return;
            } else if (noAuxiliar->proximo == NULL) {
                noAuxiliar->proximo = novoNo;
                break;
            }
            noAuxiliar = noAuxiliar->proximo;
        }
    }
}

void limparLista(PonteiroNoLista noInicial) {
    PonteiroNoLista noAuxiliar;
    noAuxiliar = noInicial;

    while (noAuxiliar != NULL) {
        noInicial = noInicial->proximo;
        free(noAuxiliar);
        noAuxiliar = noInicial;
    }
}

void limparTabela(TabelaHash tabelaHashParametro) {
    PonteiroTabelaHash tabelaHash = (PonteiroTabelaHash) tabelaHashParametro;
    for (int i = 0; i < TAMANHO; i++) {
        PonteiroNoLista noAuxiliar = tabelaHash->listas[i];
        if (noAuxiliar != NULL) {
            limparLista(noAuxiliar);
        }
    }
    free(tabelaHash->listas);
    free(tabelaHash);
}

void exibirMensagemSucesso(TabelaHash tabelaHashParametro, int linhaAtual) {
    PonteiroTabelaHash tabelaHash = (PonteiroTabelaHash) tabelaHashParametro;
    if (tabelaHash->flagErro == 0) {
        if (linhaAtual > 1) {
            printf("\n");
        }
        printf("%d: All Identifiers on Hash.", linhaAtual);
    }
}