#include "definitions.h"


extern int floatPrecision;


void createZeroMatrix(float mat[MAX_ROWS][MAX_COLUMNS]) {
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLUMNS; j++) {
            mat[i][j] = 0;
        }
    }
}


float solveRecursiveDeterminant(float mat[MAX_ROWS][MAX_COLUMNS], int rows) {
    float det = 0;
    if (rows == 1) {
        det = mat[0][0];
    } else if (rows == 2) {
        det = (mat[0][0] * mat[1][1]) - (mat[0][1] * mat[1][0]);
    } else {
        for (int i = 0; i < rows; i++) {
            float temp[MAX_ROWS][MAX_COLUMNS];
            int tempRow = 0;
            for (int j = 0; j < rows; j++) {
                if (j != i) {
                    int tempCol = 0;
                    for (int k = 0; k < rows; k++) {
                        if (k != 0) {
                            temp[tempRow][tempCol] = mat[j][k];
                            tempCol++;
                        }
                    }
                    tempRow++;
                }
            }
            det += mat[i][0] * pow(-1, i) * solveRecursiveDeterminant(temp, rows - 1);
        }
    }
    return det;
}


void solveDeterminant(float mat[MAX_ROWS][MAX_COLUMNS], int rows, int columns, bool hadMat) {
    if (hadMat) {
        if (rows == columns) {
            if (rows == 1) {
                printf("\n");
                printf("%.*f\n", floatPrecision, mat[0][0]);
                printf("\n");
            } else if (rows == 2) {
                printf("\n");
                printf("%.*f\n", floatPrecision, (mat[0][0] * mat[1][1]) - (mat[0][1] * mat[1][0]));
                printf("\n");
            } else if (rows == 3) {
                printf("\n");
                printf("%.*f\n", floatPrecision, (mat[0][0] * mat[1][1] * mat[2][2]) + (mat[0][1] * mat[1][2] * mat[2][0]) + (mat[0][2] * mat[1][0] * mat[2][1]) - (mat[0][2] * mat[1][1] * mat[2][0]) - (mat[0][0] * mat[1][2] * mat[2][1]) - (mat[0][1] * mat[1][0] * mat[2][2]));
                printf("\n");
            } else {
                printf("\n");
                printf("%.*f\n", floatPrecision, solveRecursiveDeterminant(mat, rows));
                printf("\n");
            }
        } else {
            printf("\n");
            printf("Matrix format incorrect!\n");
            printf("\n");
        }
    } else {
        printf("\n");
        printf("No Matrix defined!\n");
        printf("\n");
    }
}


int getCalculatedRank(float mat[MAX_ROWS][MAX_COLUMNS], int rows, int columns) {
    int rank = 0;
    const float EPS = 1e-6;

    for (int i = 0; i < rows; ++i) {
        bool allZeroes = true;
        for (int j = 0; j < columns; ++j) {
            if (mat[i][j] != 0) {
                allZeroes = false;
                break;
            }
        }

        if (!allZeroes) {
            ++rank;

            for (int j = 0; j < columns; ++j) {
                if (mat[i][j] != 0) {
                    float tmp = mat[i][j];

                    for (int k = 0; k < columns; ++k) {
                        mat[i][k] /= tmp;
                    }

                    break;
                }
            }

            for (int j = 0; j < rows; ++j) {
                if (j != i && mat[j][i] != 0) {
                    float tmp = mat[j][i];
                    for (int k = 0; k < columns; ++k) {
                        mat[j][k] -= tmp * mat[i][k];
                    }
                }
            }
        }
    }

    return rank;
}


void LUFactoring(float mat[MAX_ROWS][MAX_COLUMNS], int rows, int columns) {
    float lower[MAX_ROWS][MAX_COLUMNS];
    float upper[MAX_ROWS][MAX_COLUMNS];
    float Y[MAX_ROWS];
    float X[MAX_ROWS];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            lower[i][j] = 0;
            upper[i][j] = 0;
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int k = i; k < rows; k++) {
            float sum = 0;
            for (int j = 0; j < i; j++)
                sum += (lower[i][j] * upper[j][k]);
            upper[i][k] = mat[i][k] - sum;
        }

        for (int k = i; k < rows; k++) {
            if (i == k)
                lower[i][i] = 1;
            else {
                float sum = 0;
                for (int j = 0; j < i; j++)
                    sum += (lower[k][j] * upper[j][i]);
                lower[k][i] = (mat[k][i] - sum) / upper[i][i];
            }
        }
    }

    for (int i = 0; i < rows; i++) {
        Y[i] = mat[i][columns-1];
        for (int j = 0; j < i; j++) {
            Y[i] -= lower[i][j] * Y[j];
        }
        Y[i] /= lower[i][i];
    }

    for (int i = rows - 1; i >= 0; i--) {
        X[i] = Y[i];
        for (int j = i + 1; j < rows; j++) {
            X[i] -= upper[i][j] * X[j];
        }
        X[i] /= upper[i][i];
    }

    printf("\n");
    printf("Matrix x:\n");
    printf("\n");
    for (int i = 0; i < rows; i++) {
        printf("%.*f\n", floatPrecision, X[i]);
    }
    printf("\n");
}


void solveLinearSystem(float mat[MAX_ROWS][MAX_COLUMNS], int rows, int columns, bool hadMat) {
    if (!hadMat) {
        printf("\n");
        printf("No Matrix defined!\n");
        printf("\n");
        return;
    }
    if (rows != columns - 1) {
        printf("\n");
        printf("Matrix format incorrect!\n");
        printf("\n");
        return;
    }

    int rankOfAugmented, rankOfCoeff;

    rankOfAugmented = getCalculatedRank(mat, rows, columns);

    rankOfCoeff = getCalculatedRank(mat, rows, columns - 1);

    if (rankOfCoeff < rankOfAugmented) {
        printf("\n");
        printf("SI - The Linear System has no solution\n");
        printf("\n");
    } else if (rankOfCoeff == rankOfAugmented) {
        if (rankOfCoeff == columns - 1) {
            LUFactoring(mat, rows, columns);
        } else {
            printf("\n");
            printf("SPI - The Linear System has infinitely many solutions\n");
            printf("\n");
        }
    }
}


void printMatrix(float mat[MAX_ROWS][MAX_COLUMNS], int rows, int columns, bool hadMat) {
    if (hadMat) {
        bool hadNegative[columns];
        for (int i = 0; i < columns; i++) { hadNegative[i] = false; }

        int longPerColumn[columns];
        for (int i = 0; i < columns; i++) { longPerColumn[i] = 0; }
        int sumLongPerColumn = 0;
        for (int j = 0; j < columns; j++) {
            for (int i = 0; i < rows; i++) {
                int length = snprintf(NULL, 0, "%.*f", floatPrecision, mat[i][j]);
                if (mat[i][j] < 0) {
                    hadNegative[j] = true;
                }
                if (length > longPerColumn[j]) {
                    longPerColumn[j] = length;
                }
            }
            sumLongPerColumn += longPerColumn[j];
            sumLongPerColumn += 1;
        }
        sumLongPerColumn--;

        printf("+-");
        for (int i = 0; i < sumLongPerColumn; i++) {
            printf(" ");
        }
        printf("-+\n");

        for (int i = 0; i < rows; i++) {
            printf("| ");
            for (int j = 0; j < columns; j++) {
                int diffLength = longPerColumn[j] - snprintf(NULL, 0, "%.*f", floatPrecision, mat[i][j]);
                if (diffLength > 0) {
                    for (int k = 0; k < diffLength; k++) {
                        printf(" ");
                    }
                }
                printf("%.*f", floatPrecision, mat[i][j]);
                printf(" ");
            }
            printf("|\n");
        }

        printf("+-");
        for (int i = 0; i < sumLongPerColumn; i++) {
            printf(" ");
        }
        printf("-+\n");
    } else {
        printf("No Matrix defined!\n");
    }
}
