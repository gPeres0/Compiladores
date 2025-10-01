#include "definitions.h"


extern float hViewLo;
extern float hViewHi;
extern float vViewLo;
extern float vViewHi;
extern bool drawAxis;
extern bool connectDots;
extern bool erasePlot;


void printPlot(char plot[LINES_P][COLUMNS_P]) {
    printf("\n");
    for(int i = LINES_P-1; i >= 0; i--) {
        for(int j = 0; j < COLUMNS_P; j++) {
            printf("%c", plot[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


void clearPlot(char plot[LINES_P][COLUMNS_P]) {
    for(int i = 0; i < LINES_P; i++) {
        for(int j = 0; j < COLUMNS_P; j++) {
            plot[i][j] = ' ';
        }
    }
}


void setAxisPlot(char plot[LINES_P][COLUMNS_P]) {
    float dx = (hViewHi - hViewLo) / COLUMNS_P;
    float dy = (vViewHi - vViewLo) / LINES_P;
    float acc = vViewHi;
    int xAxis = -1;
    int yAxis = -1;
    for (int i = 0; i < LINES_P; i++) {
        xAxis++;
        acc -= dy;
        if (acc <= 0) {
            break;
        }
    }

    acc = hViewLo;
    for (int i = 0; i < COLUMNS_P; i++) {
        yAxis++;
        acc += dx;
        if (acc >= 0) {
            break;
        }
    }

    if (xAxis < 0) {
        xAxis = 0;
    }
    if (yAxis < 0) {
        yAxis = 0;
    }

    xAxis = LINES_P - xAxis - 1;

    if (drawAxis) {
        if (hViewHi >= 0 && hViewLo <= 0) {
            for (int i = 0; i < LINES_P; i++) {
                if (plot[i][yAxis] == ' ') {
                    plot[i][yAxis] = '|';
                }
            }
        }
        if (vViewHi >= 0 && vViewLo <= 0) {
            for (int i = 0; i < COLUMNS_P; i++) {
                if (plot[xAxis][i] == ' ') {
                    plot[xAxis][i] = '-';
                }
            }
        }
        if (hViewHi >= 0 && hViewLo <= 0 && vViewHi >= 0 && vViewLo <= 0) {
            if (plot[xAxis][yAxis] != '*') {
                plot[xAxis][yAxis] = '+';
            }
        }
    } else {
        for (int i = 0; i < LINES_P; i++) {
            if (plot[i][yAxis] == '|') {
                plot[i][yAxis] = ' ';
            }
        }
        for (int i = 0; i < COLUMNS_P; i++) {
            if (plot[xAxis][i] == '-') {
                plot[xAxis][i] = ' ';
            }
        }
        if (plot[xAxis][yAxis] == '+') {
            plot[xAxis][yAxis] = ' ';
        }
    }
}


void prepareXValues(float *xValues) {
    float deltaX = (hViewHi - hViewLo) / COLUMNS_P;
    float x = hViewLo;
    for(int i = 0; i < COLUMNS_P; i++) {
        xValues[i] = x;
        x += deltaX;
    }
}


void prepareYValues(float *yValues) {
    float deltaY = (vViewHi - vViewLo) / LINES_P;
    float y = vViewLo;
    for(int i = 0; i < LINES_P; i++) {
        yValues[i] = y;
        y += deltaY;
    }
}


void drawLine(char plot[LINES_P][COLUMNS_P], int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while(true) {
        plot[y1][x1] = '*';

        if (x1 == x2 && y1 == y2) {
            break;
        }

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}


bool drawPlot(char plot[LINES_P][COLUMNS_P], float *xValues, float *yValues, ASTNode *node, Symbol **symbolTable) {
    if (!node) {
        printf("\n");
        printf("No Function defined!\n");
        printf("\n");
        return false;
    }
    if (erasePlot) {
        clearPlot(plot);
    }
    setAxisPlot(plot);

    int prevX = -1;
    int prevY = -1;

    for (int i = 0; i < COLUMNS_P; i++) {
        float x = xValues[i];
        float y = evlAST(node, X_ET, x, symbolTable);
        if (isnan(y)) { return false; }
        int j = (int) ((y - vViewLo) / (vViewHi - vViewLo) * LINES_P);
        if (j >= 0 && j < LINES_P) {
            plot[j][i] = '*';
            if (connectDots && prevX != -1 && prevY != -1) {
                drawLine(plot, prevX, prevY, i, j);
            }
            prevX = i;
            prevY = j;
        }
    }

    printPlot(plot);
    return true;
}
