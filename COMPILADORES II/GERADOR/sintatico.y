%{

    #include "ast.hh"
    #include "traversal.hh"
    #include "mips.hh"
    #include "helpers.hh"
    extern int yylex(void);
    extern char* yytext;
    extern int yychar;
    extern int yydebug;
    int yydebug = 1;
    extern void yyerror(char *s);
    extern void yylex_destroy(void);
    std::vector<ast::AST_Constant*> constantes;
    std::vector<ast::AST_Variable*> variaveis_globais;
    std::vector<ast::AST_Function*> funcoes;
    std::vector<ast::AST_Node_Strings*> node_strings;

    std::vector<ast::AST_Variable*> variaveis_locais_temp;
    std::vector<ast::AST_Parameter*> parametros_temp;

    ast::AST_Node_Func_Body *corpo_funcao_temp;
%}

%union {
    ast::AST_Node_BOP* Type_AST_Node_BOP;
    ast::AST_Node_UOP* Type_AST_Node_UOP;
    ast::AST_Node_TOP* Type_AST_Node_TOP;
    ast::AST_Node_Expression* Type_AST_Node_Expression;
    ast::AST_Node_Function_Call* Type_AST_Node_Chamada_Funcao;
    ast::AST_Node_Variable_Acess* Type_AST_Node_Acesso_Variavel;
    ast::AST_Node_Loop_Expressions* Type_AST_Node_Loop_Expressoes;
    ast::AST_Node_Temp_Loop_Expressions* Type_AST_Node_Loop_Expressoes_Temporario;
    ast::AST_Node_Loop_Matrix* Type_AST_Node_Loop_Matriz;
    ast::AST_Node_Stop_Con* Type_AST_Node_Condicao_Parada;
    ast::AST_Node_Values_Fit* Type_AST_Node_Ajuste_Valores;
    ast::AST_Node_Init_For* Type_AST_Node_Inicializacao_For;
    ast::AST_Node_Temp_Expressions_Print* Type_AST_Node_Expressoes_Printf_Temporario;
    ast::AST_Node_Expressions_Print* Type_AST_Node_Expressoes_Printf;
    ast::AST_Node_Var_Adress* Type_AST_Node_Endereco_Var;
    ast::AST_Node_Return* Type_AST_Node_Comando_Return;
    ast::AST_Node_Exit* Type_AST_Node_Comando_Exit;
    ast::AST_Node_Scanf* Type_AST_Node_Comando_Scanf;
    ast::AST_Node_Printf* Type_AST_Node_Comando_Printf;
    ast::AST_Node_For* Type_AST_Node_Comando_For;
    ast::AST_Node_While* Type_AST_Node_Comando_While;
    ast::AST_Node_If* Type_AST_Node_Comando_If;
    ast::AST_Node_Do_While* Type_AST_Node_Comando_Do_While;
    ast::AST_Node_Command* Type_AST_Node_Comando;
    ast::AST_Node_Temp_Coms_List* Type_AST_Node_Lista_Comandos_Temporario;
    ast::AST_Node_Coms_List* Type_AST_Node_Lista_Comandos;
    ast::AST_Node_Func_Body* Type_AST_Node_Corpo_Funcao;
    ast::AST_Variable* Type_AST_Variable;
    ast::AST_Constant* Type_AST_Constant;
    ast::AST_Parameter* Type_AST_Parameter;
    ast::AST_Function* Type_AST_Function;

    std::string* string_token;
    int integer_token;
    char character_token;
}

%token AST
%token VOID
%token INT
%token CHAR
%token RETURN
%token GLOBAL
%token VARIABLE
%token TYPE
%token FUNCTION
%token END_FUNCTION
%token PARAMETER
%token RETURN_TYPE
%token CONSTANT
%token IF
%token VALUE
%token WHILE
%token FOR
%token DO_WHILE
%token PRINTF
%token SCANF
%token EXIT
%token PLUS
%token MINUS
%token MULTIPLY
%token DIV
%token REMAINDER
%token INC
%token DEC
%token BITWISE_AND
%token BITWISE_OR
%token BITWISE_NOT
%token BITWISE_XOR
%token NOT
%token LOGICAL_AND
%token LOGICAL_OR
%token EQUAL
%token NOT_EQUAL
%token LESS_THAN
%token GREATER_THAN
%token LESS_EQUAL
%token GREATER_EQUAL
%token R_SHIFT
%token L_SHIFT
%token ASSIGN
%token ADD_ASSIGN
%token MINUS_ASSIGN
%token SEMICOLON
%token COMMA
%token COLON
%token L_PAREN
%token R_PAREN
%token L_SQUARE_BRACKET
%token R_SQUARE_BRACKET
%token TERNARY_CONDITIONAL
%token START_ARROW
%token<string_token> IDENTIFIER
%token<integer_token> NUM_INTEGER
%token<string_token> STRING
%token<character_token> CHARACTER
%token NEWLINE

%type <Type_AST_Node_BOP> BOP
%type <Type_AST_Node_UOP> UOP
%type <Type_AST_Node_TOP> TOP
%type <Type_AST_Node_Expression> Expression
%type <Type_AST_Node_Acesso_Variavel> Acesso_Variavel
%type <Type_AST_Node_Loop_Matriz> Loop_Matriz
%type <Type_AST_Node_Loop_Expressoes_Temporario> Loop_Expressoes_Temporario
%type <Type_AST_Node_Loop_Expressoes> Loop_Expressoes
%type <Type_AST_Node_Chamada_Funcao> Chamada_De_Funcao
%type <Type_AST_Node_Ajuste_Valores> Ajuste_Valores
%type <Type_AST_Node_Condicao_Parada> Condicao_Parada
%type <Type_AST_Node_Inicializacao_For> Inicializacao_For
%type <Type_AST_Node_Expressoes_Printf_Temporario> Expressoes_Printf_Temporario
%type <Type_AST_Node_Expressoes_Printf> Expressoes_Printf
%type <Type_AST_Node_Endereco_Var> Endereco_Var
%type <Type_AST_Node_Comando_Return> Comando_Return
%type <Type_AST_Node_Comando_Exit> Comando_Exit
%type <Type_AST_Node_Comando_Scanf> Comando_Scanf
%type <Type_AST_Node_Comando_Printf> Comando_Printf
%type <Type_AST_Node_Comando_For> Comando_For
%type <Type_AST_Node_Comando_While> Comando_While
%type <Type_AST_Node_Comando_If> Comando_If
%type <Type_AST_Node_Comando_Do_While> Comando_Do_While
%type <Type_AST_Node_Comando> Command
%type <Type_AST_Node_Lista_Comandos_Temporario> Lista_Comandos_Temporario
%type <Type_AST_Node_Lista_Comandos> Lista_Comandos
%type <Type_AST_Node_Corpo_Funcao> Corpo_Funcao

%type <string_token> Tipo_Variavel
%type <string_token> Tamanho_Variavel
%type <string_token> Loop_Colchetes
%type <string_token> Loop_Ponteiros_Temporario


%start Program

%%

Program : AST START_ARROW Consumir_Novas_Linhas Declarations { };

Consumir_Novas_Linhas: Consumir_Novas_Linhas NEWLINE {};
                    | {};

Declarations : Declara_Constante Consumir_Novas_Linhas Declarations  {};
            | Declara_Variavel_Global Consumir_Novas_Linhas Declarations {};
            | Declara_Funcao Consumir_Novas_Linhas Declarations {};
            | {};

Declara_Constante : CONSTANT COLON IDENTIFIER VALUE COLON NUM_INTEGER {

    constantes.push_back(new ast::AST_Constant($3, $6));

};

Declara_Variavel_Global : GLOBAL VARIABLE COLON IDENTIFIER TYPE COLON Tipo_Variavel {

    variaveis_globais.push_back(new ast::AST_Variable($4, $7));

};

Tipo_Variavel: INT Tamanho_Variavel NEWLINE {

    $$ = new std::string("int" + ($2 != nullptr ? *$2 : ""));
    delete $2;

};
| CHAR Tamanho_Variavel NEWLINE {

    $$ = new std::string("char" + ($2 != nullptr ? *$2 : ""));
    delete $2;

};
| VOID Tamanho_Variavel NEWLINE {
    $$ = new std::string("void" + ($2 != nullptr ? *$2 : ""));
    delete $2;
};

Tamanho_Variavel: L_SQUARE_BRACKET NUM_INTEGER R_SQUARE_BRACKET Loop_Colchetes {
    $$ = new std::string("[" + std::to_string($2) + "]" + 
    ($4 != nullptr ? *$4 : ""));
    delete $4;
};
| MULTIPLY Loop_Ponteiros_Temporario {
    $$ = new std::string("*" + ($2 != nullptr ? *$2 : ""));
    delete $2;
};
| {$$ = nullptr;};

Loop_Colchetes : L_SQUARE_BRACKET NUM_INTEGER R_SQUARE_BRACKET Loop_Colchetes {
    $$ = new std::string("[" + std::to_string($2) + "]" + ($4 != nullptr ? *$4 : ""));
    delete $4;
};
| {$$ = nullptr;};

Loop_Ponteiros_Temporario: MULTIPLY Loop_Ponteiros_Temporario {
    $$ = new std::string("*" + ($2 != nullptr ? *$2 : ""));
    delete $2;
};
| {$$ = nullptr;};

Declara_Funcao : FUNCTION COLON IDENTIFIER Consumir_Novas_Linhas RETURN_TYPE COLON Tipo_Variavel Consumir_Novas_Linhas Parametros_De_Funcao Declarar_Variaveis_Locais Corpo_Funcao Consumir_Novas_Linhas END_FUNCTION {
    funcoes.push_back(new ast::AST_Function($3, $7));

    std::reverse(parametros_temp.begin(), parametros_temp.end());
    std::reverse(variaveis_locais_temp.begin(), variaveis_locais_temp.end());

    if (parametros_temp.size() > 0) {
        for (int i = 0; i < parametros_temp.size(); i++) {
            funcoes.back()->add_parameter(parametros_temp[i]);
        }
    }

    if (variaveis_locais_temp.size() > 0) {
        for (int i = 0; i < variaveis_locais_temp.size(); i++) {
            funcoes.back()->add_variable(variaveis_locais_temp[i]);
        }
    }

    funcoes.back()->set_function_body(corpo_funcao_temp);

    parametros_temp.clear();
    variaveis_locais_temp.clear();
    corpo_funcao_temp = nullptr;


};

Parametros_De_Funcao: PARAMETER COLON IDENTIFIER TYPE COLON Tipo_Variavel Consumir_Novas_Linhas Parametros_De_Funcao {

    ast::AST_Parameter* parametro = new ast::AST_Parameter($3, $6);
    parametros_temp.push_back(parametro);

};
| {};

Declarar_Variaveis_Locais: VARIABLE COLON IDENTIFIER TYPE COLON Tipo_Variavel Consumir_Novas_Linhas Declarar_Variaveis_Locais {
    ast::AST_Variable* variavel = new ast::AST_Variable($3, $6);
    variaveis_locais_temp.push_back(variavel);
};
| {};

/* COMANDOS */

Corpo_Funcao: Lista_Comandos { corpo_funcao_temp = new ast::AST_Node_Func_Body($1); };

Lista_Comandos: Command Lista_Comandos_Temporario {$$ = new ast::AST_Node_Coms_List($1, $2);};

Lista_Comandos_Temporario: SEMICOLON Consumir_Novas_Linhas Command Lista_Comandos_Temporario {$$ = new ast::AST_Node_Temp_Coms_List($3, $4);};
            | {$$ = nullptr;};

Command: Expression {$$ = new ast::AST_Node_Command($1);};
        | Comando_Do_While {$$ = new ast::AST_Node_Command($1);};
        | Comando_If {$$ = new ast::AST_Node_Command($1);};
        | Comando_While {$$ = new ast::AST_Node_Command($1);};
        | Comando_For {$$ = new ast::AST_Node_Command($1);};
        | Comando_Printf {$$ = new ast::AST_Node_Command($1);};
        | Comando_Scanf {$$ = new ast::AST_Node_Command($1);};
        | Comando_Exit {$$ = new ast::AST_Node_Command($1);};
        | Comando_Return {$$ = new ast::AST_Node_Command($1);};

Comando_Do_While: DO_WHILE L_PAREN Lista_Comandos COMMA Condicao_Parada R_PAREN {$$ = new ast::AST_Node_Do_While($3, $5);};

Comando_If: IF L_PAREN Condicao_Parada COMMA Lista_Comandos COMMA Lista_Comandos R_PAREN {$$ = new ast::AST_Node_If($3, $5, $7);};
        | IF L_PAREN Condicao_Parada COMMA Lista_Comandos R_PAREN {$$ = new ast::AST_Node_If($3, $5);};

Comando_While: WHILE L_PAREN Condicao_Parada COMMA Lista_Comandos R_PAREN {$$ = new ast::AST_Node_While($3, $5);};

Comando_For: FOR L_PAREN Inicializacao_For COMMA Condicao_Parada COMMA Ajuste_Valores COMMA Lista_Comandos R_PAREN {$$ = new ast::AST_Node_For($3, $5, $7, $9);};

Comando_Printf: PRINTF L_PAREN STRING COMMA Expressoes_Printf R_PAREN {$$ = new ast::AST_Node_Printf($3, $5);};
            | PRINTF L_PAREN STRING R_PAREN {$$ = new ast::AST_Node_Printf($3);};

Comando_Scanf: SCANF L_PAREN STRING COMMA Endereco_Var R_PAREN {$$ = new ast::AST_Node_Scanf($3, $5);};

Comando_Exit: EXIT L_PAREN Expression R_PAREN {$$ = new ast::AST_Node_Exit($3);};

Comando_Return: RETURN L_PAREN Condicao_Parada R_PAREN {$$ = new ast::AST_Node_Return($3);};

/* Scanf */

Endereco_Var: BITWISE_AND L_PAREN IDENTIFIER R_PAREN {$$ = new ast::AST_Node_Var_Adress($3);};

/* Printf */

Expressoes_Printf: Expression Expressoes_Printf_Temporario {$$ = new ast::AST_Node_Expressions_Print($1, $2);};
                | {$$ = nullptr;};

Expressoes_Printf_Temporario: COMMA Expression Expressoes_Printf_Temporario {$$ = new ast::AST_Node_Temp_Expressions_Print($2, $3);};
                        | {$$ = nullptr;};
/* For */

Inicializacao_For: Expression {$$ = new ast::AST_Node_Init_For($1);};
                | {$$ = nullptr;};

Condicao_Parada: Expression {$$ = new ast::AST_Node_Stop_Con($1);};
                | {$$ = nullptr;};

Ajuste_Valores: Expression {$$ = new ast::AST_Node_Values_Fit($1);};
                | {$$ = nullptr;};

/* EXPRESSOES */

Expression: BOP {$$ = new ast::AST_Node_Expression($1);};
        | UOP {$$ = new ast::AST_Node_Expression($1);};
        | TOP {$$ = new ast::AST_Node_Expression($1);};
        | Chamada_De_Funcao {$$ = new ast::AST_Node_Expression($1);};
        | Acesso_Variavel {$$ = new ast::AST_Node_Expression($1);};
        | NUM_INTEGER {$$ = new ast::AST_Node_Expression($1);};
        | CHARACTER {$$ = new ast::AST_Node_Expression($1);};
        | STRING {$$ = new ast::AST_Node_Expression($1);};

Chamada_De_Funcao: IDENTIFIER L_PAREN Loop_Expressoes R_PAREN {$$ = new ast::AST_Node_Function_Call($1, $3);};

Acesso_Variavel: IDENTIFIER Loop_Matriz {$$ = new ast::AST_Node_Variable_Acess($1, $2);};

Loop_Expressoes: Expression Loop_Expressoes_Temporario { $$ = new ast::AST_Node_Loop_Expressions($1, $2);};
            | { $$ = nullptr; };

Loop_Expressoes_Temporario: COMMA Expression Loop_Expressoes_Temporario { $$ = new ast::AST_Node_Temp_Loop_Expressions($2, $3);};
            | { $$ = nullptr; };

Loop_Matriz : L_SQUARE_BRACKET Expression R_SQUARE_BRACKET Loop_Matriz { $$ = new ast::AST_Node_Loop_Matrix($2, $4); };
            | { $$ = nullptr; };

BOP: PLUS L_PAREN Expression COMMA Expression R_PAREN {
        $$ = new ast::AST_Node_BOP("PLUS", $3, $5);
    };
    | MINUS L_PAREN Expression COMMA Expression R_PAREN {
        $$ = new ast::AST_Node_BOP("MINUS", $3, $5);
    };
    | MULTIPLY L_PAREN Expression COMMA Expression R_PAREN {
        $$ = new ast::AST_Node_BOP("MULTIPLY", $3, $5);
    };
    | DIV L_PAREN Expression COMMA Expression R_PAREN {
        $$ = new ast::AST_Node_BOP("DIV", $3, $5);
    };
    | REMAINDER L_PAREN Expression COMMA Expression R_PAREN {
        $$ = new ast::AST_Node_BOP("REMAINDER", $3, $5);
    };
    | BITWISE_AND L_PAREN Expression COMMA Expression R_PAREN {
        $$ = new ast::AST_Node_BOP("BITWISE_AND", $3, $5);
    };
    | BITWISE_OR L_PAREN Expression COMMA Expression R_PAREN {
        $$ = new ast::AST_Node_BOP("BITWISE_OR", $3, $5);
    };
    | BITWISE_XOR L_PAREN Expression COMMA Expression R_PAREN {
        $$ = new ast::AST_Node_BOP("BITWISE_XOR", $3, $5);
    };
    | LOGICAL_AND L_PAREN Expression COMMA Expression R_PAREN {
        $$ = new ast::AST_Node_BOP("LOGICAL_AND", $3, $5);
    };
    | LOGICAL_OR L_PAREN Expression COMMA Expression R_PAREN {
        $$ = new ast::AST_Node_BOP("LOGICAL_OR", $3, $5);
    };
    | EQUAL L_PAREN Expression COMMA Expression R_PAREN {
        $$ = new ast::AST_Node_BOP("EQUAL", $3, $5);
    };
    | NOT_EQUAL L_PAREN Expression COMMA Expression R_PAREN {
        $$ = new ast::AST_Node_BOP("NOT_EQUAL", $3, $5);
    };
    | LESS_THAN L_PAREN Expression COMMA Expression R_PAREN {
        $$ = new ast::AST_Node_BOP("LESS_THAN", $3, $5);
    };
    | GREATER_THAN L_PAREN Expression COMMA Expression R_PAREN {
        $$ = new ast::AST_Node_BOP("GREATER_THAN", $3, $5);
    };
    | LESS_EQUAL L_PAREN Expression COMMA Expression R_PAREN {
        $$ = new ast::AST_Node_BOP("LESS_EQUAL", $3, $5);
    };
    | GREATER_EQUAL L_PAREN Expression COMMA Expression R_PAREN {
        $$ = new ast::AST_Node_BOP("GREATER_EQUAL", $3, $5);
    };
    | R_SHIFT L_PAREN Expression COMMA Expression R_PAREN {
        $$ = new ast::AST_Node_BOP("R_SHIFT", $3, $5);
    };
    | L_SHIFT L_PAREN Expression COMMA Expression R_PAREN {
        $$ = new ast::AST_Node_BOP("L_SHIFT", $3, $5);
    };
    | ASSIGN L_PAREN Expression COMMA Expression R_PAREN {
        $$ = new ast::AST_Node_BOP("ASSIGN", $3, $5);
    };
    | ADD_ASSIGN L_PAREN Expression COMMA Expression R_PAREN {
        $$ = new ast::AST_Node_BOP("ADD_ASSIGN", $3, $5);
    };
    | MINUS_ASSIGN L_PAREN Expression COMMA Expression R_PAREN {
        $$ = new ast::AST_Node_BOP("MINUS_ASSIGN", $3, $5);
    };

UOP: PLUS  L_PAREN Expression R_PAREN {
        $$ = new ast::AST_Node_UOP("PLUS", $3, false);
    };
    | MINUS L_PAREN Expression R_PAREN {
        $$ = new ast::AST_Node_UOP("MINUS", $3, false);
    };
    | MULTIPLY L_PAREN Expression R_PAREN {
        $$ = new ast::AST_Node_UOP("MULTIPLY", $3, false);
    };
    | INC L_PAREN Expression R_PAREN {
        $$ = new ast::AST_Node_UOP("INC", $3, false);
    };
    | DEC L_PAREN Expression R_PAREN {
        $$ = new ast::AST_Node_UOP("DEC", $3, false);
    };
    | BITWISE_NOT L_PAREN Expression R_PAREN {
        $$ = new ast::AST_Node_UOP("BITWISE_NOT", $3, false);
    };
    | NOT L_PAREN Expression R_PAREN {
        $$ = new ast::AST_Node_UOP("NOT", $3, false);
    };
    | L_PAREN Expression R_PAREN INC {
        $$ = new ast::AST_Node_UOP("INC", $2, true);
    };
    | L_PAREN Expression R_PAREN DEC {
        $$ = new ast::AST_Node_UOP("DEC", $2, true);
    };

TOP: TERNARY_CONDITIONAL L_PAREN Expression COMMA Expression COMMA Expression R_PAREN {
        $$ = new ast::AST_Node_TOP($3, $5, $7);
    };

%%

int main(int argc, char **argv) {
    
    int print_ASTs = 0;
    int produce_MIPS = 1;

    yyparse();

    ast::sort_functions(funcoes);

    for (auto function : funcoes) {
        traversal::traversal_AST(function, 0, 0, 0);
    }


    if(produce_MIPS) {
        mips::print_data_segment(constantes, variaveis_globais, node_strings);
        for (auto function : funcoes) {
            traversal::traversal_AST(function, 0, 0, 1);
        }
    }

    if(print_ASTs) traversal::print_ASTs(funcoes); 

    traversal::free_ASTs(funcoes, constantes, variaveis_globais, node_strings);

    yylex_destroy();

    if(!print_ASTs) std::remove("test.dot");

}