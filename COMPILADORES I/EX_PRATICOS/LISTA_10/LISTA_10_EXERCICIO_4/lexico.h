#ifndef LEXICO_H
#define LEXICO_H

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <utility>
#include <vector>
#include <queue>

using namespace std;

class LexicalException : public exception {

public:
    explicit LexicalException(const std::string &message) {
        this->m_message.append(message);
    }

    explicit LexicalException(const char &errorChar) {
        this->m_message+= errorChar;
    }


    const char* what() const noexcept override {
        return this->m_message.c_str();
    }

private:
    string m_message = "ERRO LEXICO: ";
};

class SyntaxException : public exception {

public:
    explicit SyntaxException(const std::string &message) {
        this->m_message = message;
    }

    const char * what () const throw () {
        return this->m_message.c_str();
    }

private:
    string m_message;
};

class Estado {
public:
    Estado(vector<int> transicao, string type, bool final){
        this->transicoes = std::move(transicao);
        this->type = std::move(type);
        this->final = final;
    }

    Estado(string type, bool final) : type(std::move(type)), final(final){
        this->transicoes = vector<int>(128, 0);
    }

    explicit Estado(string type) : type(std::move(type)){
        this->transicoes = vector<int>(128, 0);
        this->final = true;
    }

    Estado(){
        this->transicoes = vector<int>(128, 0);
        this->type = "";
        this->final = false;
    }

    string getType(){
        return this->type;
    }

    bool isFinal() const{
        return this->final;
    }

    int getTransition(char a){
        return this->transicoes[a];
    }

    Estado& setTransition(char a, int i){
        this->transicoes[a] = i;
        return *this;
    }

    Estado& setNumbers(int a){
        setForInterval(a, '0', '9');
        return *this;
    }

    Estado& setLowerAlphabet(int a){
        setForInterval(a, 'a', 'z');
        return *this;
    }

    Estado& setUpperAlphabet(int a){
        setForInterval(a, 'A', 'Z');
        return *this;
    }

    Estado& setForInterval(int value, int start, int end){
        for(int i = start; i <= end; i++){
            this->transicoes[i] = value;
        }
        return *this;
    }

private:
    vector<int> transicoes;
    string type;
    bool final;
};

class Token {
public:
    Token(string cadeia, string tipo) : cadeia(std::move(cadeia)), tipo(std::move(tipo)){}

    Token()= default;

    string getCadeia(){
        if(this->null)
            throw SyntaxException("ERRO SINTATICO: CADEIA INCOMPLETA");
        return this->cadeia;
    }

    string getTipo(){
        if(this->null)
            throw SyntaxException("ERRO SINTATICO: CADEIA INCOMPLETA");
        if(this->tipo == "")
            return "";
        return this->tipo + " ";
    }

    void setNull(){
        this->null = true;
    }

    friend bool operator==(const Token& lhs, const Token& rhs)  {
        if(rhs.null || lhs.null)
            throw SyntaxException("ERRO SINTATICO: CADEIA INCOMPLETA");
        return lhs.tipo == rhs.tipo;
    }

    friend bool operator==(const Token& lhs, const string& rhs)  {
        if(lhs.null)
            throw SyntaxException("ERRO SINTATICO: CADEIA INCOMPLETA");
        return lhs.tipo == rhs;
    }

private:
    string cadeia;
    string tipo;
    bool null = false;
};

queue<Token> lexicalParse(string entrada, vector<Estado> automata);

#endif
