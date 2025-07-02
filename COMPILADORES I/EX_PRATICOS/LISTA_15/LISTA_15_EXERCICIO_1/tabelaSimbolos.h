#ifndef LISTA_15_TABELASIMBOLOS_H
#define LISTA_15_TABELASIMBOLOS_H

typedef void *TabelaHash;

TabelaHash criarTabelaSimbolos();

void inserirTabelaSimbolos(TabelaHash tabelaHashParametro, char *tipo, char *nome, int linhaAtual);

void limparTabela(TabelaHash tabelaHashParametro);

void exibirMensagemSucesso(TabelaHash tabelaHashParametro, int linhaAtual);

#endif 
