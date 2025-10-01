%{
#include "definitions.h"

#include <sintatico.tab.h> 


extern int yylex();
extern int yychar;
extern char *yytext;


float hViewLo = H_VIEW_LO;
float hViewHi = H_VIEW_HI;
float vViewLo = V_VIEW_LO;
float vViewHi = V_VIEW_HI;
int floatPrecision = FLOAT_PRECISION;
int integralSteps = INTEGRAL_STEPS_V;
bool drawAxis = DRAW_AXIS;
bool erasePlot = ERASE_PLOT;
bool connectDots = CONNECT_DOTS_V;

Symbol *symbolTable[TABLE_SIZE];

ASTNode* glblExpr = NULL;

char plot[LINES_P][COLUMNS_P];
float plotXValues[COLUMNS_P];
float plotYValues[LINES_P];

bool readMatOutBounds = false;
bool hadReadMat = false;
float readMat[MAX_ROWS][MAX_COLUMNS];
int readRows = 0;
int readColumns = 0;
int maxReadColumns = 0;

bool opMatOutBounds = false;
bool hadOpMat = false;
float opMat[MAX_ROWS][MAX_COLUMNS];
int opRows = 0;
int opColumns = 0;
int maxOpColumns = 0;


void yyerror(void *s) {
    if (yychar == EOL) {  // yychar == 0
        printf("\n");
        printf("SYNTAX ERROR: Incomplete Command\n");
        printf("\n");
    } else {
        printf("\n");
        // printf("SYNTAX ERROR: [%d]\n", yychar);
        printf("SYNTAX ERROR: [%s]\n", yytext);
        printf("\n");
    }
}

void showSettings() {
    printf("\n");
    printf("h_view_lo = %.6f\n", hViewLo);
    printf("h_view_hi = %.6f\n", hViewHi);
    printf("v_view_lo = %.6f\n", vViewLo);
    printf("v_view_hi = %.6f\n", vViewHi);
    printf("float_precision = %d\n", floatPrecision);
    printf("integral_steps = %d\n", integralSteps);
    printf("\n");
    printf("Draw Axis: %s\n", drawAxis ? "ON" : "OFF");
    printf("Erase Plot: %s\n", erasePlot ? "ON" : "OFF");
    printf("Connect Dots: %s\n", connectDots ? "ON" : "OFF");
    printf("\n");
}

void resetSettings() {
    hViewLo = H_VIEW_LO;
    hViewHi = H_VIEW_HI;
    vViewLo = V_VIEW_LO;
    vViewHi = V_VIEW_HI;
    floatPrecision = FLOAT_PRECISION;
    integralSteps = INTEGRAL_STEPS_V;
    drawAxis = DRAW_AXIS;
    erasePlot = ERASE_PLOT;
    connectDots = CONNECT_DOTS_V;

    clearPlot(plot);
    prepareXValues(plotXValues);
    prepareYValues(plotYValues);
}

void setHView(float lo, float hi) {
    if (lo >= hi) {
        printf("\n");
        printf("ERROR: h_view_lo must be smaller than h_view_hi\n");
        printf("\n");
    } else {
        hViewLo = lo;
        hViewHi = hi;
        prepareXValues(plotXValues);
        clearPlot(plot);
    }
}

void setVView(float lo, float hi) {
    if (lo >= hi) {
        printf("\n");
        printf("ERROR: v_view_lo must be smaller than v_view_hi\n");
        printf("\n");
    } else {
        vViewLo = lo;
        vViewHi = hi;
        prepareYValues(plotYValues);
        clearPlot(plot);
    }
}

void setAxis(bool state) {
    drawAxis = state;
}

void setErasePlot(bool state) {
    erasePlot = state;
}

void setIntegralSteps(int steps) {
    if (steps < 1) {
        printf("\n");
        printf("ERROR: integral_steps must be a positive non-zero integer\n");
        printf("\n");
    } else {
        integralSteps = steps;
    }
}

void setConnectDots(bool state) {
    connectDots = state;
}

void setFloatPrecision(int precision) {
    if (precision < 0 || precision > 8) {
        printf("\n");
        printf("ERROR: float precision must be from 0 to 8.\n");
        printf("\n");
    } else {
        floatPrecision = precision;
    }
}


void about() {
    printf("\n");
    printf("               ___________               \n");
    printf("              |           |              \n");
    printf("              |     2     |              \n");
    printf("              |     0     |              \n");
    printf("              |     2     |              \n");
    printf("              |     3     |              \n");
    printf("              |     0     |              \n");
    printf("              |     0     |              \n");
    printf("              |     5     |              \n");
    printf("              |     6     |              \n");
    printf("              |     0     |              \n");
    printf("              |     3     |              \n");
    printf("              |     1     |              \n");
    printf("              |     4     |              \n");
    printf("   ___________|           |___________   \n");
    printf("  |                                   |  \n");
    printf("  |         BEM-VINDO AO DCMAT        |  \n");
    printf("  |             By G.Peres            |  \n");
    printf("  |___________             ___________|  \n");
    printf("  \\___________|           |___________/  \n");
    printf("              |     6     |              \n");
    printf("              |     6     |              \n");
    printf("              |     6     |              \n");
    printf("              |___________|              \n");
    printf("              \\___________/              \n");
    printf("\n");
}


void sommation(char *id, int a, int b, ASTNode *expr) {
    printf("\n");
    float result = 0;
    for (int i = a; i <= b; i++) {
        float evlResult = evaluateSumAST(expr, id, i, symbolTable);
        if (!isnan(evlResult)) {
            result += evlResult;
        } else {
            printf("\n");
            return;
        }
    }
    printf("%.*f\n", floatPrecision, result);
    printf("\n");
}

void integrate(float a, float b, ASTNode *expr) {
    if (a >= b) {
        printf("\n");
        printf("ERROR: lower limit must be smaller than upper limit\n");
        printf("\n");
        return;
    }
    float result = 0;
    float h = (b - a) / integralSteps;
    float x = a;
    for (int i = 0; i < integralSteps; i++) {
        result += evlAST(expr, X_ET, x, symbolTable);
        x += h;
    }
    result *= h;
    printf("\n");
    printf("%.*f\n", floatPrecision, result);
    printf("\n");
}


void updateOpRows() {
    if (opRows >= MAX_ROWS-1) {
        if (!opMatOutBounds) {
            printf("\n");
            printf("ERROR: Matrix limits out of boundaries.\n");
            printf("\n");
            opMatOutBounds = true;
            hadOpMat = false;
        }
        opRows = MAX_ROWS - 1;
    }
    opRows++;

    if (opColumns > maxOpColumns) {
        maxOpColumns = opColumns;
    }
    opColumns = 0;
}

void updateOpColumns() {
    if (opColumns >= MAX_COLUMNS-1) {
        if (!opMatOutBounds) {
            printf("\n");
            printf("ERROR: Matrix limits out of boundaries.\n");
            printf("\n");
            opMatOutBounds = true;
            hadOpMat = false;
        }
        opColumns = MAX_COLUMNS - 1;
    }
    opColumns++;
    maxOpColumns = opColumns;
}

void restartOpMat() {
    createZeroMatrix(readMat);
    opRows = 0;
    opColumns = 0;
    maxOpColumns = 0;
    opMatOutBounds = false;
    hadOpMat = false;
}

void updateReadRows() {
    if (readRows >= MAX_ROWS-1) {
        if (!readMatOutBounds) {
            printf("\n");
            printf("ERROR: Matrix limits out of boundaries.\n");
            printf("\n");
            readMatOutBounds = true;
            hadReadMat = false;
        }
        readRows = MAX_ROWS - 1;
    }
    readRows++;

    if (readColumns > maxReadColumns) {
        maxReadColumns = readColumns;
    }
    readColumns = 0;
}

void updateReadColumns() {
    if (readColumns >= MAX_COLUMNS-1) {
        if (!readMatOutBounds) {
            printf("\n");
            printf("ERROR: Matrix limits out of boundaries.\n");
            printf("\n");
            readMatOutBounds = true;
            hadReadMat = false;
        }
        readColumns = MAX_COLUMNS - 1;
    }
    readColumns++;
    maxReadColumns = readColumns;
}

void restartReadMat() {
    createZeroMatrix(readMat);
    readRows = 0;
    readColumns = 0;
    maxReadColumns = 0;
    readMatOutBounds = false;
    hadReadMat = false;
}
%}


%union {
    char *name;
    int intValue;
    float floatValue;
    ASTNode *astNode;
}

%token ABOUT
%token ABS
%token AXIS
%token CONNECT_DOTS
%token COS
%token DETERMINANT
%token EULER
%token ERASE
%token FLOAT
%token H_VIEW
%token INTEGRAL_STEPS
%token INTEGRATE
%token LINEAR_SYSTEM
%token MATRIX
%token OFF
%token ON
%token PI
%token PLOT
%token PRECISION
%token QUIT
%token RESET
%token RPN
%token SEN
%token SET
%token SETTINGS
%token SHOW
%token SOLVE
%token SUM
%token SYMBOLS
%token TAN
%token V_VIEW

%token PLUS
%token MINUS
%token TIMES
%token DIVIDE
%token POWER
%token REMAINDER
%token L_PAREN
%token R_PAREN
%token COLON
%token EQUAL
%token ASSIGN
%token L_BRACKET
%token R_BRACKET
%token SEMICOLON
%token COMMA

%token <intValue> NUM_INT
%token <floatValue> NUM_FLOAT
%token <name> ID
%token <name> X

%token EOL

%type <astNode> expr
%type <astNode> mul_expr
%type <astNode> un_expr
%type <astNode> prim_expr
%type <floatValue> smpl_num

%start program


%%


program: func EOL { return 1; }
       | expr EOL { evlAST($1, EXPR_ET, -1, symbolTable); return 1; }
       | QUIT EOL { return 0; }
       | EOL      { return 1; }
;


func: settings
    | plot
    | rpn
    | integrate
    | sum
    | mat
    | var
;


settings: SHOW SETTINGS SEMICOLON                      { showSettings(); }
        | RESET SETTINGS SEMICOLON                     { resetSettings(); }
        | SET H_VIEW smpl_num COLON smpl_num SEMICOLON { setHView($3, $5); }
        | SET V_VIEW smpl_num COLON smpl_num SEMICOLON { setVView($3, $5); }
        | SET AXIS ON SEMICOLON                        { setAxis(_ON); }
        | SET AXIS OFF SEMICOLON                       { setAxis(_OFF); }
        | SET ERASE PLOT ON SEMICOLON                  { setErasePlot(_ON); }
        | SET ERASE PLOT OFF SEMICOLON                 { setErasePlot(_OFF); }
        | SET INTEGRAL_STEPS smpl_num SEMICOLON        { setIntegralSteps($3); }
        | SET FLOAT PRECISION smpl_num SEMICOLON       { setFloatPrecision($4); }
        | SET CONNECT_DOTS ON SEMICOLON                { setConnectDots(_ON); }
        | SET CONNECT_DOTS OFF SEMICOLON               { setConnectDots(_OFF); }
        | ABOUT SEMICOLON                              { about(); }
        | SHOW SYMBOLS SEMICOLON                       { printAllTypes(symbolTable); }
        | SHOW MATRIX SEMICOLON                        { printf("\n"); printMatrix(opMat, opRows+1, maxOpColumns+1, hadOpMat); printf("\n"); }
;


plot: PLOT SEMICOLON                      { drawPlot(plot, plotXValues, plotYValues, glblExpr, symbolTable); }
    | PLOT L_PAREN expr R_PAREN SEMICOLON { drawPlot(plot, plotXValues, plotYValues, $3, symbolTable) ? glblExpr = $3 : 0; }
;


rpn: RPN L_PAREN expr R_PAREN SEMICOLON { evlAST($3, RPN_ET, -1, symbolTable); }
;


integrate: INTEGRATE L_PAREN smpl_num COLON smpl_num COMMA expr R_PAREN SEMICOLON { integrate($3, $5, $7); }
;


sum: SUM L_PAREN ID COMMA smpl_num COLON smpl_num COMMA expr R_PAREN SEMICOLON { sommation($3, $5, $7, $9); }
;


mat: SOLVE DETERMINANT SEMICOLON               { solveDeterminant(opMat, opRows+1, maxOpColumns+1, hadOpMat); }
   | SOLVE LINEAR_SYSTEM SEMICOLON             { solveLinearSystem(opMat, opRows+1, maxOpColumns+1, hadOpMat); }
   | MATRIX EQUAL L_BRACKET L_BRACKET smpl_num { hadOpMat ? restartOpMat() : 0; hadOpMat = true; opMat[0][0] = $5; } mat_val R_BRACKET mat_line R_BRACKET SEMICOLON
;

mat_line: COMMA L_BRACKET smpl_num { updateOpRows(); opMat[opRows][opColumns] = $3; } mat_val R_BRACKET mat_line
        |
;

mat_val: COMMA smpl_num { updateOpColumns(); opMat[opRows][opColumns] = $2; } mat_val
       |
;


var: ID SEMICOLON                  { printValue(symbolTable, $1); }
   | ID ASSIGN expr SEMICOLON      { addFloatSymbol(symbolTable, $1, $3); }
   | ID ASSIGN read_mat SEMICOLON  { addMatrixSymbol(symbolTable, $1, readMat, readRows+1, maxReadColumns+1, hadReadMat); }
;


read_mat: L_BRACKET L_BRACKET smpl_num { hadReadMat ? restartReadMat() : 0; hadReadMat = true; readMat[0][0] = $3; } read_mat_val R_BRACKET read_mat_line R_BRACKET
;

read_mat_line: COMMA L_BRACKET smpl_num { updateReadRows(); readMat[readRows][readColumns] = $3; } read_mat_val R_BRACKET read_mat_line
             |
;

read_mat_val: COMMA smpl_num { updateReadColumns(); readMat[readRows][readColumns] = $2; } read_mat_val
            |
;


expr: mul_expr            { $$ = $1; }
    | expr PLUS mul_expr  { $$ = createAST($1, $3, ADD_NT); }
    | expr MINUS mul_expr { $$ = createAST($1, $3, SUB_NT); }
;

mul_expr: un_expr                    { $$ = $1; }
        | mul_expr TIMES un_expr     { $$ = createAST($1, $3, MUL_NT); }
        | mul_expr DIVIDE un_expr    { $$ = createAST($1, $3, DIV_NT); }
        | mul_expr REMAINDER un_expr { $$ = createAST($1, $3, MOD_NT); }
        | mul_expr POWER un_expr     { $$ = createAST($1, $3, POW_NT); }
;

un_expr: prim_expr       { $$ = $1; }
       | PLUS prim_expr  { $$ = createAST($2, NULL, UN_ADD_NT); }
       | MINUS prim_expr { $$ = createAST($2, NULL, UN_SUB_NT); }
;

prim_expr: ABS L_PAREN expr R_PAREN { $$ = createAST($3, NULL, ABS_NT); }
         | COS L_PAREN expr R_PAREN { $$ = createAST($3, NULL, COS_NT); }
         | SEN L_PAREN expr R_PAREN { $$ = createAST($3, NULL, SIN_NT); }
         | TAN L_PAREN expr R_PAREN { $$ = createAST($3, NULL, TAN_NT); }
         | L_PAREN expr R_PAREN     { $$ = createAST($2, NULL, NO_NT); }
         | NUM_INT                  { $$ = createASTNode(NUM_NT, $1, NULL); }
         | NUM_FLOAT                { $$ = createASTNode(NUM_NT, $1, NULL); }
         | ID                       { $$ = createASTNode(ID_NT, -1, $1); }
         | PI                       { $$ = createASTNode(NUM_NT, PI_V, NULL); }
         | EULER                    { $$ = createASTNode(NUM_NT, EULER_V, NULL); }
         | X                        { $$ = createASTNode(ID_NT, -1, $1); }
;

smpl_num: NUM_INT         { $$ = $1; }
        | NUM_FLOAT       { $$ = $1; }
        | PLUS NUM_INT    { $$ = $2; }
        | PLUS NUM_FLOAT  { $$ = $2; }
        | MINUS NUM_INT   { $$ = -$2; }
        | MINUS NUM_FLOAT { $$ = -$2; }
;

    
%%


int main(int argc, char **argv) {
    prepareXValues(plotXValues);
    prepareYValues(plotYValues);

    createZeroMatrix(opMat);
    createZeroMatrix(readMat);

    printf(">");
    while(yyparse()) {
        printf(">");
    }

    freeSymbolTable(symbolTable);

    return 0;
}
