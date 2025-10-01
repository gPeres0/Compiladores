#include "definitions.h"



extern int floatPrecision;


Symbol *getFloatSymbol(float value) {
    Symbol *symbol = calloc(1, sizeof(Symbol));
    symbol->valueType = TYPE_FLOAT;
    symbol->floatValue = value;
    return symbol;
}


Symbol *createMatrixSymbol(int lines, int columns) {
    Symbol *symbol = calloc(1, sizeof(Symbol));
    symbol->valueType = TYPE_MATRIX;
    symbol->lines = lines;
    symbol->columns = columns;
    return symbol;
}


int hash(char *name) {
    int hash_value = 0;
    for(int i = 0; i < strlen(name); i++) {
        hash_value = hash_value + name[i];
    }
    hash_value = hash_value % TABLE_SIZE;
    return hash_value;
}


void addFloatSymbol(Symbol **symbolTable, char* name, ASTNode *node) {
    printf("\n");
    Symbol *symbol = calloc(1, sizeof(Symbol));
    symbol->valueType = TYPE_FLOAT;
    symbol->name = name;

    Symbol* symValue = evaluateNum(node, symbolTable);
    if (!symValue) {
        free(symbol->name);
        free(symbol);
        printf("\n");
        return;
    }

    int hash_value = hash(name);
    if (symbolTable[hash_value] == NULL) {
        if (symValue->valueType == TYPE_FLOAT) {
            symbol->valueType = TYPE_FLOAT;
            symbol->floatValue = symValue->floatValue;
            printf("%.*f\n", floatPrecision, symbol->floatValue);
            printf("\n");
        } else {
            symbol->valueType = TYPE_MATRIX;
            symbol->lines = symValue->lines;
            symbol->columns = symValue->columns;
            for (int i = 0; i < symValue->lines; i++) {
                for (int j = 0; j < symValue->columns; j++) {
                    symbol->matrixValue[i][j] = symValue->matrixValue[i][j];
                }
            }
            printMatrix(symbol->matrixValue, symbol->lines, symbol->columns, true);
            printf("\n");
        }
        symbolTable[hash_value] = symbol;
    } else {
        Symbol *current = symbolTable[hash_value];
        do {
            if (!strcmp(current->name, name)) {
                if (symValue->valueType == TYPE_FLOAT) {
                    current->valueType = TYPE_FLOAT;
                    current->floatValue = symValue->floatValue;
                    printf("%.*f\n", floatPrecision, current->floatValue);
                    printf("\n");
                    return;
                } else {
                    current->valueType = TYPE_MATRIX;
                    current->lines = symValue->lines;
                    current->columns = symValue->columns;
                    for (int i = 0; i < symValue->lines; i++) {
                        for (int j = 0; j < symValue->columns; j++) {
                            current->matrixValue[i][j] = symValue->matrixValue[i][j];
                        }
                    }
                    printMatrix(current->matrixValue, current->lines, current->columns, true);
                    printf("\n");
                }
                free(symbol->name);
                free(symbol);
            }
            current = current->next;
        } while(current);
        current = symbol;
    }
}


void addMatrixSymbol(Symbol **symbolTable, char* name, float matrixValue[MAX_ROWS][MAX_COLUMNS], int lines, int columns, bool hadMatrix) {
    if (!hadMatrix) { return; }
    Symbol *symbol = calloc(1, sizeof(Symbol));
    symbol->valueType = TYPE_MATRIX;
    symbol->name = name;
    symbol->lines = lines;
    symbol->columns = columns;
    for(int i = 0; i < lines; i++) {
        for(int j = 0; j < columns; j++) {
            symbol->matrixValue[i][j] = matrixValue[i][j];
        }
    }

    int hash_value = hash(name);
    if (symbolTable[hash_value] == NULL) {
        symbolTable[hash_value] = symbol;
    } else {
        Symbol *current = symbolTable[hash_value];
        do {
            if (!strcmp(current->name, name)) {
                current->valueType = TYPE_MATRIX;
                current->lines = lines;
                current->columns = columns;
                for(int i = 0; i < lines; i++) {
                    for(int j = 0; j < columns; j++) {
                        current->matrixValue[i][j] = matrixValue[i][j];
                    }
                }
                free(symbol->name);
                free(symbol);
                printf("\n");
                printMatrix(current->matrixValue, lines, columns, true);
                printf("\n");
                return;
            }
            current = current->next;
        } while(current);
        current = symbol;
    }
    printf("\n");
    printMatrix(symbol->matrixValue, lines, columns, true);
    printf("\n");
}


float getFloatValue(Symbol **symbolTable, char *name) {
    int hash_value = hash(name);
    Symbol *current = symbolTable[hash_value];
    while(current) {
        if (!strcmp(current->name, name)) {
            return current->floatValue;
        }
        current = current->next;
    }
    printf("Undefined symbol [%s]\n", name);
    return NAN;
}


void getMatrixValue(Symbol **symbolTable, char *name, float matrixValue[MAX_ROWS][MAX_COLUMNS], int *lines, int *columns) {
    int hash_value = hash(name);
    Symbol *current = symbolTable[hash_value];
    while(current) {
        if (!strcmp(current->name, name)) {
            *lines = current->lines;
            *columns = current->columns;
            for(int i = 0; i < current->lines; i++) {
                for(int j = 0; j < current->columns; j++) {
                    matrixValue[i][j] = current->matrixValue[i][j];
                }
            }
            return;
        }
        current = current->next;
    }
    printf("Undefined symbol [%s]\n", name);
    matrixValue[0][0] = NAN;
    return;
}


Symbol *getSymbol(Symbol **symbolTable, char *name) {
    int hash_value = hash(name);
    Symbol *current = symbolTable[hash_value];
    while(current) {
        if (!strcmp(current->name, name)) {
            return current;
        }
        current = current->next;
    }
    printf("Undefined symbol [%s]\n", name);
    return NULL;
}


void printValue(Symbol **symbolTable, char *name) {
    int hash_value = hash(name);
    Symbol *current = symbolTable[hash_value];
    while(current) {
        if (!strcmp(current->name, name)) {
            if (current->valueType == TYPE_FLOAT) {
                printf("\n");
                printf("%s = %.*f\n", name, floatPrecision, current->floatValue);
                printf("\n");
            } else {
                printf("\n");
                printMatrix(current->matrixValue, current->lines, current->columns, true);
                printf("\n");
            }
            return;
        }
        current = current->next;
    }
    printf("\n");
    printf("Undefined symbol\n");
    printf("\n");
}


void printAllTypes(Symbol **symbolTable) {
    printf("\n");
    for(int i = 0; i < TABLE_SIZE; i++) {
        Symbol* current = symbolTable[i];
        while(current) {
            if (current->valueType == TYPE_FLOAT) {
                printf("%s - FLOAT\n", current->name);
            } else {
                printf("%s - MATRIX [%d][%d]\n", current->name, current->lines, current->columns);
            }
            current = current->next;
        }
    }
    printf("\n");
}


void freeSymbolTable(Symbol **symbolTable) {
    for(int i = 0; i < TABLE_SIZE; i++) {
        Symbol *current = symbolTable[i];
        while(current) {
            Symbol *next = current->next;
            free(current->name);
            free(current);
            current = next;
        }
        symbolTable[i] = NULL;
    }
}
