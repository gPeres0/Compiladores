#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

// Incluindo bibliotecas padrão
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Definindo macros
enum { _OFF, _ON };

// #define ON  true
// #define OFF false

#define PI_V    3.14159265
#define EULER_V 2.71828182

#define H_VIEW_LO        -6.5
#define H_VIEW_HI        6.5
#define V_VIEW_LO        -3.5
#define V_VIEW_HI        3.5
#define FLOAT_PRECISION  6
#define INTEGRAL_STEPS_V 1000
#define DRAW_AXIS        _ON
#define ERASE_PLOT       _ON
#define CONNECT_DOTS_V   _OFF

#define TABLE_SIZE 9973

// Definindo o tipo Symbol e suas funções associadas
#define MAX_ROWS 10
#define MAX_COLUMNS 10

typedef struct ASTNode ASTNode;  // Declaração antecipada

typedef enum ValueType { TYPE_FLOAT, TYPE_MATRIX } ValueType;

typedef struct Symbol {
    ValueType valueType;
    char *name;
    float floatValue;
    float matrixValue[MAX_ROWS][MAX_COLUMNS];
    int lines;
    int columns;
    struct Symbol *next;
} Symbol;

Symbol *getFloatSymbol(float value);
Symbol *createMatrixSymbol(int lines, int columns);
void addFloatSymbol(Symbol **symbolTable, char* name, ASTNode *node);
void addMatrixSymbol(Symbol **symbolTable, char* name, float matrixValue[MAX_ROWS][MAX_COLUMNS], int lines, int columns, bool hadMatrix);
float getFloatValue(Symbol **symbolTable, char *name);
void getMatrixValue(Symbol **symbolTable, char *name, float matrixValue[MAX_ROWS][MAX_COLUMNS], int *lines, int *columns);
Symbol *getSymbol(Symbol **symbolTable, char *name);
void printValue(Symbol **symbolTable, char *name);
void printAllTypes(Symbol **symbolTable);
void freeSymbolTable(Symbol **symbolTable);

// Definindo o tipo ASTNode e suas funções associadas
typedef struct Symbol Symbol;  // Declaração antecipada

typedef enum ASTNodeType { NUM_NT, ADD_NT, SUB_NT, MUL_NT, DIV_NT, POW_NT, MOD_NT, SIN_NT, COS_NT, TAN_NT, ABS_NT, UN_ADD_NT, UN_SUB_NT, NO_NT, ID_NT } ASTNodeType;

typedef enum EvalType { EXPR_ET, NUM_ET, RPN_ET, X_ET } EvalType;

typedef struct ASTNode {
    ASTNodeType type;
    float value;
    char *name;
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

ASTNode *createASTNode(ASTNodeType type, float value, char *name);
ASTNode *createAST(ASTNode *left, ASTNode *right, ASTNodeType type);
void freeAST(ASTNode *node);
Symbol *evaluateNum(ASTNode *node, Symbol **symbolTable);
float evaluateSumAST(ASTNode *node, char *sumVar, int sumValue, Symbol **symbolTable);
float evlAST(ASTNode *node, EvalType evalType, float xValue, Symbol **symbolTable);

// Funções para manipulação de matrizes
void createZeroMatrix(float mat[MAX_ROWS][MAX_COLUMNS]);
void solveDeterminant(float mat[MAX_ROWS][MAX_COLUMNS], int rows, int columns, bool hadMat);
void solveLinearSystem(float mat[MAX_ROWS][MAX_COLUMNS], int rows, int columns, bool hadMat);
void printMatrix(float mat[MAX_ROWS][MAX_COLUMNS], int rows, int columns, bool hadMat);

// Funções de plotagem
#define LINES_P   25
#define COLUMNS_P 80

void clearPlot(char plot[LINES_P][COLUMNS_P]);
void prepareXValues(float *xValues);
void prepareYValues(float *yValues);
bool drawPlot(char plot[LINES_P][COLUMNS_P], float *xValues, float *yValues, ASTNode *node, Symbol **symbolTable);

#endif
