#ifndef _AUTOMATO_H_
#define _AUTOMATO_H_

int get_proximo_estado(int estado, char caractere_lido);

void formatar_inicio();

int estado_final(int estado);

int retornar_token(int estado_final);

#endif