#ifndef SINTATICO_H
#define SINTATICO_H

#include "lexico.h"

extern bool cin_start;

void advance();
void eat(const string& tipo);
string breakline();
void syntaticalParse(queue<Token> lexTokens);

#endif