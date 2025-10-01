#include "definitions.h"

extern int floatPrecision;
bool hadX = false;


ASTNode *createASTNode(ASTNodeType type, float value, char *name) {
    ASTNode *node = calloc(1, sizeof(ASTNode));
    node->type = type;
    node->value = value;
    node->name = name;
    return node;
}


ASTNode *createAST(ASTNode *left, ASTNode *right, ASTNodeType type) {
    ASTNode *node = createASTNode(type, -1, NULL);
    node->left = left;
    node->right = right;
    return node;
}


void freeAST(ASTNode *node) {
    node->left  ? freeAST(node->left) : 0;
    node->right ? freeAST(node->right) : 0;
    node->name  ? free(node->name) : 0;
    free(node);
}


Symbol *evaluateNum(ASTNode *node, Symbol **symbolTable) {
    if (!node) { return getFloatSymbol(-1); }
    Symbol *left = evaluateNum(node->left, symbolTable);
    Symbol *right = evaluateNum(node->right, symbolTable);
    if (!left || !right) { return NULL; }
    switch(node->type) {
        case NUM_NT:
            return getFloatSymbol(node->value);
        
        case ADD_NT:
            if (left->valueType == TYPE_FLOAT && right->valueType == TYPE_FLOAT) {
                return getFloatSymbol(left->floatValue + right->floatValue);
            } else if (left->valueType == TYPE_MATRIX && right->valueType == TYPE_MATRIX) {
                if (left->lines == right->lines && left->columns == right->columns) {
                    Symbol *symRet = createMatrixSymbol(left->lines, left->columns);
                    for (int i = 0; i < left->lines; i++) {
                        for (int j = 0; j < left->columns; j++) {
                            symRet->matrixValue[i][j] = left->matrixValue[i][j] + right->matrixValue[i][j];
                        }
                    }
                    return symRet;
                } else {
                    printf("Incorrect dimensions for operator '+' - have MATRIX[%d][%d] and MATRIX[%d][%d]\n", left->lines, left->columns, right->lines, right->columns);
                    return NULL;
                }
            } else if (left->valueType == TYPE_FLOAT && right->valueType == TYPE_MATRIX) {
                printf("Incorrect type for operator '+' - have FLOAT and MATRIX\n");
            } else if (left->valueType == TYPE_MATRIX && right->valueType == TYPE_FLOAT) {
                printf("Incorrect type for operator '+' - have MATRIX and FLOAT\n");
            }
            return NULL;
        
        case SUB_NT:
            if (left->valueType == TYPE_FLOAT && right->valueType == TYPE_FLOAT) {
                return getFloatSymbol(left->floatValue - right->floatValue);
            } else if (left->valueType == TYPE_MATRIX && right->valueType == TYPE_MATRIX) {
                if (left->lines == right->lines && left->columns == right->columns) {
                    Symbol *symRet = createMatrixSymbol(left->lines, left->columns);
                    for (int i = 0; i < left->lines; i++) {
                        for (int j = 0; j < left->columns; j++) {
                            symRet->matrixValue[i][j] = left->matrixValue[i][j] - right->matrixValue[i][j];
                        }
                    }
                    return symRet;
                } else {
                    printf("Incorrect dimensions for operator '-' - have MATRIX[%d][%d] and MATRIX[%d][%d]\n", left->lines, left->columns, right->lines, right->columns);
                }
            } else if (left->valueType == TYPE_FLOAT && right->valueType == TYPE_MATRIX) {
                printf("Incorrect type for operator '-' - have FLOAT and MATRIX\n");
            } else if (left->valueType == TYPE_MATRIX && right->valueType == TYPE_FLOAT) {
                printf("Incorrect type for operator '-' - have MATRIX and FLOAT\n");
            }
            return NULL;
        
        case MUL_NT:
            if (left->valueType == TYPE_FLOAT && right->valueType == TYPE_FLOAT) {
                return getFloatSymbol(left->floatValue * right->floatValue);
            } else if (left->valueType == TYPE_MATRIX && right->valueType == TYPE_MATRIX) {
                if (left->columns == right->lines) {
                    Symbol * symRet = createMatrixSymbol(left->lines, right->columns);
                    for (int i = 0; i < left->lines; i++) {
                        for (int j = 0; j < right->columns; j++) {
                            symRet->matrixValue[i][j] = 0;
                            for (int k = 0; k < left->columns; k++) {
                                symRet->matrixValue[i][j] += left->matrixValue[i][k] * right->matrixValue[k][j];
                            }
                        }
                    }
                    return symRet;
                } else {
                    printf("Incorrect dimensions for operator '*' - have MATRIX[%d][%d] and MATRIX[%d][%d]\n", left->lines, left->columns, right->lines, right->columns);
                    return NULL;
                }
            } else if (left->valueType == TYPE_FLOAT && right->valueType == TYPE_MATRIX) {
                Symbol *symRet = createMatrixSymbol(right->lines, right->columns);
                for (int i = 0; i < right->lines; i++) {
                    for (int j = 0; j < right->columns; j++) {
                        symRet->matrixValue[i][j] = left->floatValue * right->matrixValue[i][j];
                    }
                }
                return symRet;
            } else if (left->valueType == TYPE_MATRIX && right->valueType == TYPE_FLOAT) {
                Symbol *symRet = createMatrixSymbol(left->lines, left->columns);
                for (int i = 0; i < left->lines; i++) {
                    for (int j = 0; j < left->columns; j++) {
                        symRet->matrixValue[i][j] = left->matrixValue[i][j] * right->floatValue;
                    }
                }
                return symRet;
            }
        
        case DIV_NT:
            if (left->valueType == TYPE_FLOAT && right->valueType == TYPE_FLOAT) {
                if (right->floatValue == 0) {
                    printf("inf\n");
                    return NULL;
                }
                return getFloatSymbol(left->floatValue / right->floatValue);
            } else if (left->valueType == TYPE_MATRIX && right->valueType == TYPE_MATRIX) {
                printf("Incorrect type for operator '/' - have MATRIX and MATRIX\n");
            } else if (left->valueType == TYPE_FLOAT && right->valueType == TYPE_MATRIX) {
                printf("Incorrect type for operator '/' - have FLOAT and MATRIX\n");
            } else if (left->valueType == TYPE_MATRIX && right->valueType == TYPE_FLOAT) {
                printf("Incorrect type for operator '/' - have MATRIX and FLOAT\n");
            }
            return NULL;

        case POW_NT:
            if (left->valueType == TYPE_FLOAT && right->valueType == TYPE_FLOAT) {
                return getFloatSymbol(pow(left->floatValue, right->floatValue));
            } else if (left->valueType == TYPE_MATRIX && right->valueType == TYPE_MATRIX) {
                printf("Incorrect type for operator '^' - have MATRIX and MATRIX\n");
            } else if (left->valueType == TYPE_FLOAT && right->valueType == TYPE_MATRIX) {
                printf("Incorrect type for operator '^' - have FLOAT and MATRIX\n");
            } else if (left->valueType == TYPE_MATRIX && right->valueType == TYPE_FLOAT) {
                printf("Incorrect type for operator '^' - have MATRIX and FLOAT\n");
            }
            return NULL;

        case MOD_NT:
            if (left->valueType == TYPE_FLOAT && right->valueType == TYPE_FLOAT) {
                return getFloatSymbol(fmod(left->floatValue, right->floatValue));
            } else if (left->valueType == TYPE_MATRIX && right->valueType == TYPE_MATRIX) {
                printf("Incorrect type for operator '%%' - have MATRIX and MATRIX\n");
            } else if (left->valueType == TYPE_FLOAT && right->valueType == TYPE_MATRIX) {
                printf("Incorrect type for operator '%%' - have FLOAT and MATRIX\n");
            } else if (left->valueType == TYPE_MATRIX && right->valueType == TYPE_FLOAT) {
                printf("Incorrect type for operator '%%' - have MATRIX and FLOAT\n");
            }
            return NULL;
        
        case SIN_NT:
            if (left->valueType == TYPE_FLOAT) {
                return getFloatSymbol(sin(left->floatValue));
            } else if (left->valueType == TYPE_MATRIX) {
                printf("Incorrect type for operator 'SEN' - have MATRIX\n");
            }
            return NULL;

        case COS_NT:
            if (left->valueType == TYPE_FLOAT) {
                return getFloatSymbol(cos(left->floatValue));
            } else if (left->valueType == TYPE_MATRIX) {
                printf("Incorrect type for operator 'COS' - have MATRIX\n");
            }
            return NULL;
        
        case TAN_NT:
            if (left->valueType == TYPE_FLOAT) {
                return getFloatSymbol(tan(left->floatValue));
            } else if (left->valueType == TYPE_MATRIX) {
                printf("Incorrect type for operator 'TAN' - have MATRIX\n");
            }
            return NULL;
        
        case ABS_NT:
            if (left->valueType == TYPE_FLOAT) {
                return getFloatSymbol(fabs(left->floatValue));
            } else if (left->valueType == TYPE_MATRIX) {
                printf("Incorrect type for operator 'ABS' - have MATRIX\n");
            }
            return NULL;
        
        case UN_SUB_NT:
            if (left->valueType == TYPE_FLOAT) {
                return getFloatSymbol(-left->floatValue);
            } else if (left->valueType == TYPE_MATRIX) {
                Symbol *symRet = createMatrixSymbol(left->lines, left->columns);
                for (int i = 0; i < left->lines; i++) {
                    for (int j = 0; j < left->columns; j++) {
                        symRet->matrixValue[i][j] = -left->matrixValue[i][j];
                    }
                }
                return symRet;
            }
        
        case ID_NT:
        if (!strcmp(node->name, "x")){
            hadX = true;
            printf("The x variable cannot be present on expressions.\n");
            return NULL;
        } else {
            Symbol *symResult = getSymbol(symbolTable, node->name);
        }

        case UN_ADD_NT:
        case NO_NT:
        default:
            return left;        
    }
}


void evaluateRpn(ASTNode *node) {
    if (!node) { return; }
    evaluateRpn(node->left);
    evaluateRpn(node->right);
    switch(node->type) {
        case NUM_NT:    printf("%.*f ", floatPrecision, node->value); break;
        case ADD_NT:    printf("+ "); break;
        case SUB_NT:    printf("- "); break;
        case MUL_NT:    printf("* "); break;
        case DIV_NT:    printf("/ "); break;
        case POW_NT:    printf("^ "); break;
        case MOD_NT:    printf("%% "); break;
        case SIN_NT:    printf("SEN "); break;
        case COS_NT:    printf("COS "); break;
        case TAN_NT:    printf("TAN "); break;
        case ABS_NT:    printf("ABS "); break;
        case UN_ADD_NT: printf("+ "); break;
        case UN_SUB_NT: printf("- "); break;
        case ID_NT:     printf("%s ", node->name); break;
        case NO_NT:     
        default:        break;
    }
}


float evaluateX(ASTNode *node, float xValue, Symbol **symbolTable) {
    if (!node) { return -1; }
    float left = evaluateX(node->left, xValue, symbolTable);
    float right = evaluateX(node->right, xValue, symbolTable);
    if (isnan(left) || isnan(right)) { return NAN; }
    switch(node->type) {
        case NUM_NT:    return node->value;
        case ADD_NT:    return left + right;
        case SUB_NT:    return left - right;
        case MUL_NT:    return left * right;
        case POW_NT:    return pow(left, right);
        case MOD_NT:    return fmod(left, right);
        case SIN_NT:    return sin(left);
        case COS_NT:    return cos(left);
        case TAN_NT:    return tan(left);
        case ABS_NT:    return fabs(left);
        case UN_ADD_NT: return left;
        case UN_SUB_NT: return -left;
        case NO_NT:     return left;
        case DIV_NT:
            if (right == 0) {
                printf("\ninf\n\n");
                return NAN;
            }
            return left / right;
        case ID_NT:
            if (!strcmp(node->name, "x")) {
                return xValue;
            } else {
                Symbol *sym = getSymbol(symbolTable, node->name);
                if (!sym) { return NAN; }
                if (sym->valueType == TYPE_MATRIX) {
                    return NAN;
                } else {
                    return sym->floatValue;
                }
            }
    }
}


float evaluateSumAST(ASTNode *node, char *sumVar, int sumValue, Symbol **symbolTable) {
    if (!node) { return -1; }
    float left = evaluateSumAST(node->left, sumVar, sumValue, symbolTable);
    float right = evaluateSumAST(node->right, sumVar, sumValue, symbolTable);
    if (isnan(left) || isnan(right)) { return NAN; }
    switch(node->type) {
        case NUM_NT:    return node->value;
        case ADD_NT:    return left + right;
        case SUB_NT:    return left - right;
        case MUL_NT:    return left * right;
        case POW_NT:    return pow(left, right);
        case MOD_NT:    return fmod(left, right);
        case SIN_NT:    return sin(left);
        case COS_NT:    return cos(left);
        case TAN_NT:    return tan(left);
        case ABS_NT:    return fabs(left);
        case UN_ADD_NT: return left;
        case UN_SUB_NT: return -left;
        case NO_NT:     return left;
        case DIV_NT:
            if (right == 0) {
                printf("\ninf\n\n");
                return NAN;
            }
            return left / right;
        case ID_NT:
            if (!strcmp(node->name, sumVar)) {
                return sumValue;
            } else {
                return getFloatValue(symbolTable, node->name);
            }
    }
}


float evlAST(ASTNode *node, EvalType evalType, float xValue, Symbol **symbolTable) {
    float result;
    Symbol *symResult;
    switch (evalType) {
        case EXPR_ET:
            printf("\n");
            symResult = evaluateNum(node, symbolTable);
            if (hadX) { hadX = false; printf("\n"); return result; }
            if (!symResult) { printf("\n"); return NAN; }
            if (symResult->valueType == TYPE_FLOAT) {
                // printf("\n");
                printf("%.*f\n", floatPrecision, symResult->floatValue);
                printf("\n");
                result = symResult->floatValue;
            } else if (symResult->valueType == TYPE_MATRIX) {
                // printf("\n");
                printMatrix(symResult->matrixValue, symResult->lines, symResult->columns, true);
                printf("\n");
                return NAN;
            }
            break;
            
        case RPN_ET:
            printf("\n");
            printf("Expression in RPN format:\n");
            printf("\n");
            evaluateRpn(node); printf("\n");
            printf("\n");
            break;
        
        case X_ET:
            result = evaluateX(node, xValue, symbolTable);
            if (isnan(result)) { return NAN; }
            break;
    }
    return result;
}
