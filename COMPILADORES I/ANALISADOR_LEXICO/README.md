# Compilador C: Analsiador Léxico

## Visão Geral:
Utilizando a ferramenta Flex e a linguagem C, implemente um analisador léxico para um subconjunto
de tokens da linguagem C. Para cada token reconhecido, você deverá imprimir a classificação do mesmo. 

## Tokens:
Identificadores: IDENTIFIER
Inteiros: NUM_INTEGER
Octais: NUM_OCTAL
Hexadecimal: NUM_HEXA
String: STRING
Caracter: CHARACTER

void VOID
int INT
char CHAR
return RETURN
break BREAK
switch SWITCH
case CASE
default DEFAULT
do DO
while WHILE
for FOR
if IF
else ELSE
typedef TYPEDEF
struct STRUCT
/+ PLUS
/- MINUS
/* MULTIPLY
// DIV
% REMAINDER
++ INC
-- DEC
& BITWISE_AND
| BITWISE_OR
~ BITWISE_NOT
^ BITWISE_XOR
! NOT
&& LOGICAL_AND
|| LOGICAL_OR
== EQUAL
!= NOT_EQUAL
< LESS_THAN
/> GREATER_THAN
<= LESS_EQUAL
/>= GREATER_EQUAL
/>> R_SHIFT
<< L_SHIFT
= ASSIGN
+= ADD_ASSIGN
-= MINUS_ASSIGN
; SEMICOLON
, COMMA
: COLON
( L_PAREN
) R_PAREN
{ L_CURLY_BRACKET
} R_CURLY_BRACKET
[ L_SQUARE_BRACKET
] R_SQUARE_BRACKET
? TERNARY_CONDITIONAL
/# NUMBER_SIGN
-> POINTER
printf PRINTF
scanf SCANF
define DEFINE
exit EXIT

## Especificações:
#### Identificadores:
Os identificadores podem ser iniciados por letras maiúsculas ou minúsculas ou ainda o caractere underscore. A partir do segundo símbolo, números de 0 até 9 também podem aparecer na formação do
identificador. Os identificadores podem possuir um tamanho máximo de 255 caracteres.
#### Inteiros e Octais:
Números octais sempre são iniciados pelo número 0 seguido pelos números de 1 até 7 e depois pelos
números de 0 até 7. Um número iniciado por dois zeros, como 007, deve ser tratado como inteiro.
#### Hexadecimais:
Números hexadecimais são iniciados por 0x ou 0X.
#### Strings e Caracteres:
Strings são reconhecidas como um conjunto de símbolos contidos entre aspas duplas. Já Caracteres são compreendidos como o conjunto de símbolos contidos entre aspas simples.

## Exemplos de Entrada e Saída:
#### Exemplo 1:
###### Entrada:
if(@)
{
    a
}
###### Saída esperada:
IF
L_PAREN
error:lexical:1:4: @
R_PAREN
L_CURLY_BRACKET
IDENTIFIER(a)
R_CURLY_BRACKET

#### Exemplo 2:
###### Entrada:
if(1)
{
/* isto eh um
comentario iniciado
e nao terminado
###### Saída esperada:
IF
L_PAREN
NUM_INTEGER(1)
R_PAREN
L_CURLY_BRACKET
error:lexical:3:1: unterminated comment

#### Exempo 3:
###### Entrada:
if(@)
{
    a;
    printf("Adeus mundo \"cruel!\"")
    //Oi, eu sou um comentario de linha
    a->[666] += 0x34 + 07 << !2;
    "cadeia" != ’@’ + variavel_nao_declarada;
    /* oi, eu sou um
    comentario de bloco */
    for(;;){$} +45=-78,0X78+08;
    07; // sou octal
    007; // sou inteiro
    0xa; // sou hexa
    0XA; // tambem sou hexa
}
comment++;/*Isto eh um
comentario sem fim...
###### Saída esperada:
IF
L_PAREN
error:lexical:1:4: @
R_PAREN
L_CURLY_BRACKET
IDENTIFIER(a)
SEMICOLON
PRINTF
L_PAREN
STRING(Adeus mundo \"cruel!\")
R_PAREN
IDENTIFIER(a)
POINTER
L_SQUARE_BRACKET
NUM_INTEGER(666)
R_SQUARE_BRACKET
ADD_ASSIGN
NUM_HEXA(0x34)
PLUS
NUM_OCTAL(07)
L_SHIFT
NOT
NUM_INTEGER(2)
SEMICOLON
STRING(cadeia)
NOT_EQUAL
CHARACTER(@)
PLUS
IDENTIFIER(variavel_nao_declarada)
SEMICOLON
FOR
L_PAREN
SEMICOLON
SEMICOLON
R_PAREN
L_CURLY_BRACKET
error:lexical:10:12: $
R_CURLY_BRACKET
PLUS
NUM_INTEGER(45)
ASSIGN
MINUS
NUM_INTEGER(78)
COMMA
NUM_HEXA(0X78)
PLUS
NUM_INTEGER(08)
SEMICOLON
NUM_OCTAL(07)
SEMICOLON
NUM_INTEGER(007)
SEMICOLON
NUM_HEXA(0xa)
SEMICOLON
NUM_HEXA(0XA)
SEMICOLON
R_CURLY_BRACKET
IDENTIFIER(comment)
INC
SEMICOLON
error:lexical:16:11: unterminated comment

## Observações:


## Warnings:

