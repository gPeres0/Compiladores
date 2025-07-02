#include "sintatico.h"

#include <utility>

Token token;
queue<Token> tokens;

void E();
void E1();
void T();
void T1();
void F();
void S();

void syntaticalParse(queue<Token> lexTokens){
    tokens = std::move(lexTokens);
    if(tokens.empty()){
        token = Token("", "");
    }else {
        token = tokens.front();
    }
    try {
        S();
        cout << breakline() << "CADEIA ACEITA";
    } catch (SyntaxException &e) {
        cout << breakline() << e.what();
    }
}

void advance() {
    if (tokens.empty()) {
        token = Token("", "");
    }else {
        tokens.pop();
        if(tokens.empty())
            token = Token("", "");
        else
            token = tokens.front();
    }
}

void eat(const string& tipo) {
    if (token == tipo) {
        advance();
    } else {
        throw SyntaxException("ERRO SINTATICO EM: " + token.getTipo() + "ESPERADO: " + tipo);
    }
}

void S(){
    if(token == "id"){
        E();
        eat("$");
    } else if(token == "("){
        E();
        eat("$");
    } else {
        throw SyntaxException("ERRO SINTATICO EM: " + token.getTipo() + "ESPERADO: id, (");
    }
}

void E(){
    if(token == "id"){
        T();
        E1();
    }else if(token == "("){
        T();
        E1();
    }else {
        throw SyntaxException("ERRO SINTATICO EM: " + token.getTipo() + "ESPERADO: id, (");
    }
}

void E1(){
    if(token == "+"){
        eat("+");
        T();
        E1();
    }else if(token == ")"){
        return;
    }else if(token == "$"){
        return;
    }else{
        throw SyntaxException("ERRO SINTATICO EM: " + token.getTipo() + "ESPERADO: +, ), $");
    }
}

void T(){
    if(token == "id"){
        F();
        T1();
    }else if(token == "("){
        F();
        T1();
    }else {
        throw SyntaxException("ERRO SINTATICO EM: " + token.getTipo() + "ESPERADO: id, (");
    }
}

void T1(){
    if(token == "+"){
        return;
    }else if(token == "*"){
        eat("*");
        F();
        T1();
    }else if(token == ")"){
        return;
    }else if(token == "$") {
        return;
    }else{
        throw SyntaxException("ERRO SINTATICO EM: " + token.getTipo() + "ESPERADO: +, *, ), $");
    }
}

void F(){
    if(token == "id"){
        eat("id");
    }else if(token == "("){
        eat("(");
        E();
        eat(")");
    }else{
        throw SyntaxException("ERRO SINTATICO EM: " + token.getTipo() + "ESPERADO: id, (");
    }
}

string breakline(){
    if(!cin_start) {
        cin_start = true;
        return "";
    }else
        return "\n";
}
