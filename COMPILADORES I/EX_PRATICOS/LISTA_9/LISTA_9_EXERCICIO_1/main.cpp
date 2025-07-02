#include <iostream>
#include <cstdlib>
#include "lexico.h"
#include "sintatico.h"

using namespace std;

int token;
int token_anterior;
int erro_impresso;
int cadeia_incompleta;

void S();
void L();
void E();

void erro(int t);
void eat(int t);
void avancar();
char* get_token_esperado(int t);
void avancar_cadeia();
char* get_token_errado();

void erro(int t) {
    if (erro_impresso) {
        return;
    }
    erro_impresso = 1;
    
    if (get_ultimo_caractere_lido() != 10 && leu_quebra_linha()) {
        cadeia_incompleta = 1;
        cout << "ERRO SINTATICO: CADEIA INCOMPLETA" << endl;
    } else {
        cout << "ERRO SINTATICO EM: " << get_token_errado() << " ESPERADO: " << get_token_esperado(t) << endl;
    }
    return;
}

void avancar(){
    token = get_token();
}

void eat(int t){
    if (erro_impresso) {
        avancar();
    }
    if (token == t) {
        avancar();
    }else {
        erro(t);
    }   
}

void S() {
    if (erro_impresso){
        return;
    }
    switch (token)
    {
    case IF:
        eat(IF);
        E();
        eat(THEN);
        S();
        eat(ELSE);
        S();
        break;
    
    case BEGIN:
        eat(BEGIN);
        S();
        L();
        break;

    case PRINT:
        eat(PRINT);
        E();
        break;
    
    default:
        erro_impresso = 1;
        cout << "ERRO SINTATICO EM: " << get_token_errado() << " ESPERADO: if, begin, print" << endl;
        return;
    }
}

void L() {
    if (erro_impresso) {
        return;
    }
    switch (token)
    {
    case END:
        eat(END);
        break;
    
    case SEMI:
        eat(SEMI);
        S();
        L();
        break;
    
    default:
        erro_impresso = 1; 
        cout << "ERRO SINTATICO EM: " << get_token_errado() << " ESPERADO: end, ;" << endl; 
        return;
    }
}

void E() {
    if (erro_impresso){
        return;
    }
    eat(NUM);
    eat(EQ);
    eat(NUM);
}

char* get_token_esperado(int t) {
    switch (t) {
        case IF: return "if";
        case THEN: return "then";
        case ELSE: return "else";
        case BEGIN: return "begin";
        case END: return "end";
        case PRINT: return "print";
        case SEMI: return ";";
        case NUM: return "num";
        case EQ: return "=";
    }
}

void avancar_cadeia() {
    char caractere_lido;
    caractere_lido = cin.get();
    while (caractere_lido != '\n') {
        if (caractere_lido == -1) {
            exit(0);
        }
        caractere_lido = cin.get();
    }
    avancar();
}

char* get_token_errado() {
    if (token == NUM) {
        return "num";
    }
    return get_ultimo_token_lido();
}

int main() {
    int primeiro = 1;
    avancar();
    do {
        // Reseta valores
        erro_impresso = 0;
        cadeia_incompleta = 0;
        reset_quebra_linha();
        
        // Caso não seja a primeira iteração, imprime \n no início
        if (primeiro) {
            primeiro = 0;
        } else {
            cout << "\n";
        }
        
        // Função inicial do sintático
        S();
        if (!erro_impresso) {
            cout << "CADEIA ACEITA" << endl;
        }
        if (get_ultimo_caractere_lido() != -1 && erro_impresso && !cadeia_incompleta) {
            avancar_cadeia();
        }
    } while (get_ultimo_caractere_lido() != -1);
    return 0;
}
