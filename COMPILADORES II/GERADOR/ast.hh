#ifndef AST_HH
#define AST_HH

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "helpers.hh"

namespace ast {

    enum AST_Nonterminals {
        Program,
        ConsumeNewLine,
        Declarations,
        ConstantDeclaration,
        GlobalDeclaration,
        VariableType,
        VariableSize,
        LoopBrackets,
        LoopTempPointer,
        FunctionDeclaration,
        FunctionParameters,
        LocalDeclaration,
        FunctionBody,
        ComsList,
        TempComsList,
        Command,
        ExpressionsLoop,
        TempExpressionsLoop,
        DoWhileCom,
        WhileCom,
        ForCom,
        IfCom,
        PrintfCom,
        ScanfCom,
        ExitCom,
        ReturnCom,
        VarAdress,
        PrintfExpressions,
        PrintfTempExpressions,
        InitFor,
        StopCondition,
        FitValues,
        Expression,
        FunctionCall,
        VariableAcess,
        LoopMatrix,
        BOP,
        UOP,
        TOP
    };

    class AST_Node;
    class AST_Node_BOP;
    class AST_Node_UOP;
    class AST_Node_TOP;
    class AST_Node_Expression;
    class AST_Node_Function_Call;
    class AST_Node_Variable_Acess;
    class AST_Node_Loop_Expressions;
    class AST_Node_Temp_Loop_Expressions;
    class AST_Node_Loop_Matrix;
    class AST_Node_Stop_Con;
    class AST_Node_Values_Fit;
    class AST_Node_Init_For;
    class AST_Node_Temp_Expressions_Print;
    class AST_Node_Expressions_Print;
    class AST_Node_Var_Adress;
    class AST_Node_Return;
    class AST_Node_Exit;
    class AST_Node_Scanf;
    class AST_Node_Printf;
    class AST_Node_For;
    class AST_Node_While;
    class AST_Node_If;
    class AST_Node_Do_While;
    class AST_Node_Command;
    class AST_Node_Temp_Coms_List;
    class AST_Node_Coms_List;
    class AST_Node_Func_Body;
    class AST_Variable;
    class AST_Constant;
    class AST_Parameter;
    class AST_Function;
    class AST_Node_Strings;

    class AST_Node {
       public:
        void* node_content;
        int node_name;
        int node_number;
        AST_Node* parent;

        AST_Node() {
        }
    };

    class AST_Node_BOP : public AST_Node {
       public:
        std::string operation;
        AST_Node_Expression* left;
        AST_Node_Expression* right;
        int mapped_to_register;

        AST_Node_BOP(std::string operation, AST_Node_Expression* left, AST_Node_Expression* right) {
            init();
            this->operation = operation;
            this->left = left;
            this->right = right;
        }

       private:
        void init() {
            this->node_number = -1;
            this->mapped_to_register = -1;
            this->parent = nullptr;
            this->node_content = this;
            this->node_name = AST_Nonterminals::BOP;
            this->left = nullptr;
            this->right = nullptr;
        }
    };

    class AST_Node_UOP : public AST_Node {
       public:
        std::string operation;
        AST_Node_Expression* child;
        int is_postfix;
        int mapped_to_register;

        AST_Node_UOP(std::string operation, AST_Node_Expression* child, int is_postfix) {
            init();
            this->operation = operation;
            this->child = child;
            this->is_postfix = is_postfix;
        }

       private:
        void init() {
            this->node_number = -1;
            this->mapped_to_register = -1;
            this->parent = nullptr;
            this->node_content = this;
            this->node_name = AST_Nonterminals::UOP;
            this->child = nullptr;
        }
    };

    class AST_Node_TOP : public AST_Node {
       public:
        AST_Node_Expression* test;
        AST_Node_Expression* left;
        AST_Node_Expression* right;

        AST_Node_TOP(AST_Node_Expression* test, AST_Node_Expression* left, AST_Node_Expression* right) {
            init();
            this->test = test;
            this->left = left;
            this->right = right;
        }

       private:
        void init() {
            this->node_number = -1;
            this->parent = nullptr;
            this->node_content = this;
            this->node_name = AST_Nonterminals::TOP;
            this->test = nullptr;
            this->left = nullptr;
            this->right = nullptr;
        }
    };

    class AST_Node_Expression : public AST_Node {
       public:
        // First rule
        AST_Node_BOP* bop;
        // Second rule
        AST_Node_UOP* uop;
        // Third rule
        AST_Node_TOP* top;
        // Fourth rule
        AST_Node_Function_Call* func_call;
        // Fifth rule
        int num_int;
        // Sixth rule
        char character;
        // Seventh rule
        std::string* string;

        int using_int;
        int using_char;
        int using_string;

        int mapped_to_register;

        int strings_added;

        AST_Node_Variable_Acess* variable_acess;

        AST_Node_Expression(AST_Node_BOP* bop) {
            init();
            this->bop = bop;
        }

        AST_Node_Expression(AST_Node_UOP* uop) {
            init();
            this->uop = uop;
        }

        AST_Node_Expression(AST_Node_TOP* top) {
            init();
            this->top = top;
        }

        AST_Node_Expression(AST_Node_Function_Call* func_call) {
            init();
            this->func_call = func_call;
        }

        AST_Node_Expression(int num_int) {
            init();
            this->num_int = num_int;
            this->using_int = 1;
        }

        AST_Node_Expression(char character) {
            init();
            this->character = character;
            this->using_char = 1;
        }

        AST_Node_Expression(std::string* string) {
            init();
            this->string = string;
            this->using_string = 1;
        }

        AST_Node_Expression(AST_Node_Variable_Acess* variable_acess) {
            init();
            this->variable_acess = variable_acess;
        }

       private:
        void init() {
            this->strings_added = 0;
            this->mapped_to_register = -1;
            this->node_number = -1;
            this->parent = nullptr;
            this->node_content = this;
            this->node_name = AST_Nonterminals::Expression;
            this->bop = nullptr;
            this->uop = nullptr;
            this->top = nullptr;
            this->func_call = nullptr;
            this->num_int = -1;
            this->character = 0;
            this->string = nullptr;
            this->variable_acess = nullptr;
            this->using_int = 0;
            this->using_char = 0;
            this->using_string = 0;
        }
    };

    class AST_Node_Function_Call : public AST_Node {
       public:
        // First rule
        std::string* function_name;
        AST_Node_Loop_Expressions* loop_expressoes;
        int mapped_to_register;

        AST_Node_Function_Call(std::string* function_name, AST_Node_Loop_Expressions* loop_expressoes) {
            init();
            this->function_name = function_name;
            this->loop_expressoes = loop_expressoes;
        }

       private:
        void init() {
            this->node_number = -1;
            this->mapped_to_register = -1;
            this->parent = nullptr;
            this->node_content = this;
            this->node_name = AST_Nonterminals::FunctionCall;
            this->function_name = nullptr;
            this->loop_expressoes = nullptr;
        }
    };

    class AST_Node_Variable_Acess : public AST_Node {
       public:
        // First rule
        std::string* variable_name;

        // Second rule
        AST_Node_Loop_Matrix* loop_matriz;

        int mapped_to_register;

        AST_Node_Variable_Acess(std::string* variable_name, AST_Node_Loop_Matrix* loop_matriz) {
            init();
            this->variable_name = variable_name;
            this->loop_matriz = loop_matriz;
        }

        AST_Node_Variable_Acess(std::string* variable_name) {
            init();
            this->variable_name = variable_name;
        }

       private:
        void init() {
            this->node_number = -1;
            this->parent = nullptr;
            this->mapped_to_register = -1;
            this->node_content = this;
            this->node_name = AST_Nonterminals::VariableAcess;
            this->variable_name = nullptr;
            this->loop_matriz = nullptr;
        }
    };

    class AST_Node_Loop_Expressions : public AST_Node {
       public:
        // First rule
        AST_Node_Expression* expressao;
        AST_Node_Temp_Loop_Expressions* loop_expressoes_temporario;

        AST_Node_Loop_Expressions(AST_Node_Expression* expressao, AST_Node_Temp_Loop_Expressions* loop_expressoes_temporario) {
            init();
            this->expressao = expressao;
            this->loop_expressoes_temporario = loop_expressoes_temporario;
        }

       private:
        void init() {
            this->node_number = -1;
            this->parent = nullptr;
            this->node_content = this;
            this->node_name = AST_Nonterminals::ExpressionsLoop;
            this->expressao = nullptr;
            this->loop_expressoes_temporario = nullptr;
        }
    };

    class AST_Node_Temp_Loop_Expressions : public AST_Node {
       public:
        AST_Node_Expression* expressao;
        AST_Node_Temp_Loop_Expressions* loop_expressoes_temporario;

        AST_Node_Temp_Loop_Expressions(AST_Node_Expression* expressao, AST_Node_Temp_Loop_Expressions* loop_expressoes_temporario) {
            init();
            this->expressao = expressao;
            this->loop_expressoes_temporario = loop_expressoes_temporario;
        }

       private:
        void init() {
            this->node_number = -1;
            this->parent = nullptr;
            this->node_content = this;
            this->node_name = AST_Nonterminals::TempExpressionsLoop;
            this->expressao = nullptr;
            this->loop_expressoes_temporario = nullptr;
        }
    };

    class AST_Node_Loop_Matrix : public AST_Node {
       public:
        AST_Node_Expression* expressao;
        AST_Node_Loop_Matrix* loop_matriz;

        AST_Node_Loop_Matrix(AST_Node_Expression* expressao, AST_Node_Loop_Matrix* loop_matriz) {
            init();
            this->expressao = expressao;
            this->loop_matriz = loop_matriz;
        }

       private:
        void init() {
            this->node_number = -1;
            this->parent = nullptr;
            this->node_content = this;
            this->node_name = AST_Nonterminals::LoopMatrix;
            this->expressao = nullptr;
            this->loop_matriz = nullptr;
        }
    };

    class AST_Node_Stop_Con : public AST_Node {
       public:
        // First rule
        AST_Node_Expression* expressao;

        int mapped_to_register;

        AST_Node_Stop_Con(AST_Node_Expression* expressao) {
            init();
            this->expressao = expressao;
        }

       private:
        void init() {
            this->node_number = -1;
            this->mapped_to_register = -1;
            this->parent = nullptr;
            this->node_content = this;
            this->node_name = AST_Nonterminals::StopCondition;
            this->expressao = nullptr;
        }
    };

    class AST_Node_Values_Fit : public AST_Node {
       public:
        // First rule
        AST_Node_Expression* expressao;

        AST_Node_Values_Fit(AST_Node_Expression* expressao) {
            init();
            this->expressao = expressao;
        }

       private:
        void init() {
            this->node_number = -1;
            this->parent = nullptr;
            this->node_content = this;
            this->node_name = AST_Nonterminals::FitValues;
            this->expressao = nullptr;
        }
    };

    class AST_Node_Init_For : public AST_Node {
       public:
        // First rule
        AST_Node_Expression* expressao;

        AST_Node_Init_For(AST_Node_Expression* expressao) {
            init();
            this->expressao = expressao;
        }

       private:
        void init() {
            this->node_number = -1;
            this->parent = nullptr;
            this->node_content = this;
            this->node_name = AST_Nonterminals::InitFor;
            this->expressao = nullptr;
        }
    };

    class AST_Node_Temp_Expressions_Print : public AST_Node {
       public:
        // First rule
        AST_Node_Expression* expressao;
        AST_Node_Temp_Expressions_Print* expressoes_printf_temporario;

        AST_Node_Temp_Expressions_Print(AST_Node_Expression* expressao, AST_Node_Temp_Expressions_Print* expressoes_printf_temporario) {
            init();
            this->expressao = expressao;
            this->expressoes_printf_temporario = expressoes_printf_temporario;
        }

       private:
        void init() {
            this->node_number = -1;
            this->parent = nullptr;
            this->node_content = this;
            this->node_name = AST_Nonterminals::PrintfTempExpressions;
            this->expressao = nullptr;
            this->expressoes_printf_temporario = nullptr;
        }
    };

    class AST_Node_Expressions_Print : public AST_Node {
       public:
        // First rule
        AST_Node_Expression* expressao;
        AST_Node_Temp_Expressions_Print* expressoes_printf_temporario;

        AST_Node_Expressions_Print(AST_Node_Expression* expressao, AST_Node_Temp_Expressions_Print* expressoes_printf_temporario) {
            init();
            this->expressao = expressao;
            this->expressoes_printf_temporario = expressoes_printf_temporario;
        }

       private:
        void init() {
            this->node_number = -1;
            this->parent = nullptr;
            this->node_content = this;
            this->node_name = AST_Nonterminals::PrintfExpressions;
            this->expressao = nullptr;
            this->expressoes_printf_temporario = nullptr;
        }
    };

    class AST_Node_Var_Adress : public AST_Node {
       public:
        // First rule
        std::string* identifier;

        AST_Node_Var_Adress(std::string* identifier) {
            init();
            this->identifier = identifier;
        }

       private:
        void init() {
            this->node_number = -1;
            this->parent = nullptr;
            this->node_content = this;
            this->node_name = AST_Nonterminals::VarAdress;
            this->identifier = nullptr;
        }
    };

    class AST_Node_Return : public AST_Node {
       public:
        // First rule
        AST_Node_Stop_Con* StopCondition;

        AST_Node_Return(AST_Node_Stop_Con* StopCondition) {
            init();
            this->StopCondition = StopCondition;
        }

       private:
        void init() {
            this->node_number = -1;
            this->parent = nullptr;
            this->node_content = this;
            this->node_name = AST_Nonterminals::ReturnCom;
            this->StopCondition = nullptr;
        }
    };

    class AST_Node_Exit : public AST_Node {
       public:
        // First rule
        AST_Node_Expression* expressao;

        AST_Node_Exit(AST_Node_Expression* expressao) {
            init();
            this->expressao = expressao;
        }

       private:
        void init() {
            this->node_number = -1;
            this->parent = nullptr;
            this->node_content = this;
            this->node_name = AST_Nonterminals::ExitCom;
            this->expressao = nullptr;
        }
    };

    class AST_Node_Scanf : public AST_Node {
       public:

        int strings_added;

        // First rule
        std::string* string;
        AST_Node_Var_Adress* endereco_var;

        AST_Node_Scanf(std::string* string, AST_Node_Var_Adress* endereco_var) {
            init();
            this->string = string;
            this->endereco_var = endereco_var;
        }

       private:
        void init() {
            this->strings_added = 0;
            this->node_number = -1;
            this->parent = nullptr;
            this->node_content = this;
            this->node_name = AST_Nonterminals::ScanfCom;
            this->string = nullptr;
            this->endereco_var = nullptr;
        }
    };

    class AST_Node_Printf : public AST_Node {
       public:

        int strings_added;

        // First rule
        AST_Node_Expressions_Print* expressoes_printf;

        // First and second rule
        std::string* string;

        AST_Node_Printf(std::string* string, AST_Node_Expressions_Print* expressoes_printf) {
            init();
            this->string = string;
            this->expressoes_printf = expressoes_printf;
        }

        AST_Node_Printf(std::string* string) {
            init();
            this->string = string;
        }

       private:
        void init() {
            this->strings_added = 0;
            this->node_number = -1;
            this->parent = nullptr;
            this->node_content = this;
            this->node_name = AST_Nonterminals::PrintfCom;
            this->string = nullptr;
            this->expressoes_printf = nullptr;
        }
    };

    class AST_Node_For : public AST_Node {
       public:
        // First rule
        AST_Node_Init_For* inicializacao_for;
        AST_Node_Stop_Con* condicao_parada;
        AST_Node_Values_Fit* ajuste_valores;
        AST_Node_Coms_List* lista_comandos;

        AST_Node_For(AST_Node_Init_For* inicializacao_for, AST_Node_Stop_Con* condicao_parada, AST_Node_Values_Fit* ajuste_valores, AST_Node_Coms_List* lista_comandos) {
            init();
            this->inicializacao_for = inicializacao_for;
            this->condicao_parada = condicao_parada;
            this->ajuste_valores = ajuste_valores;
            this->lista_comandos = lista_comandos;
        }

       private:
        void init() {
            this->node_number = -1;
            this->parent = nullptr;
            this->node_content = this;
            this->node_name = AST_Nonterminals::ForCom;
            this->inicializacao_for = nullptr;
            this->condicao_parada = nullptr;
            this->ajuste_valores = nullptr;
            this->lista_comandos = nullptr;
        }
    };

    class AST_Node_While : public AST_Node {
       public:
        // First rule
        AST_Node_Stop_Con* condicao_parada;
        AST_Node_Coms_List* lista_comandos;

        AST_Node_While(AST_Node_Stop_Con* condicao_parada, AST_Node_Coms_List* lista_comandos) {
            init();
            this->condicao_parada = condicao_parada;
            this->lista_comandos = lista_comandos;
        }

       private:
        void init() {
            this->node_number = -1;
            this->parent = nullptr;
            this->node_content = this;
            this->node_name = AST_Nonterminals::WhileCom;
            this->condicao_parada = nullptr;
            this->lista_comandos = nullptr;
        }
    };

    class AST_Node_If : public AST_Node {
       public:
        // First and second rule
        AST_Node_Stop_Con* condicao_parada;
        AST_Node_Coms_List* lista_comandos_then;
        AST_Node_Coms_List* lista_comandos_else;

        AST_Node_If(AST_Node_Stop_Con* condicao_parada, AST_Node_Coms_List* lista_comandos_then, AST_Node_Coms_List* lista_comandos_else) {
            init();
            this->condicao_parada = condicao_parada;
            this->lista_comandos_then = lista_comandos_then;
            this->lista_comandos_else = lista_comandos_else;
        }

        AST_Node_If(AST_Node_Stop_Con* condicao_parada, AST_Node_Coms_List* lista_comandos_then) {
            init();
            this->condicao_parada = condicao_parada;
            this->lista_comandos_then = lista_comandos_then;
        }

       private:
        void init() {
            this->node_number = -1;
            this->parent = nullptr;
            this->node_content = this;
            this->node_name = AST_Nonterminals::IfCom;
            this->condicao_parada = nullptr;
            this->lista_comandos_then = nullptr;
            this->lista_comandos_else = nullptr;
        }
    };

    class AST_Node_Do_While : public AST_Node {
       public:
        // First rule
        AST_Node_Coms_List* lista_comandos;
        AST_Node_Stop_Con* condicao_parada;

        AST_Node_Do_While(AST_Node_Coms_List* lista_comandos, AST_Node_Stop_Con* condicao_parada) {
            init();
            this->lista_comandos = lista_comandos;
            this->condicao_parada = condicao_parada;
        }

       private:
        void init() {
            this->node_number = -1;
            this->parent = nullptr;
            this->node_content = this;
            this->node_name = AST_Nonterminals::DoWhileCom;
            this->lista_comandos = nullptr;
            this->condicao_parada = nullptr;
        }
    };

    class AST_Node_Command : public AST_Node {
       public:
        // First rule
        AST_Node_Do_While* comando_do_while;

        // Second rule
        AST_Node_If* comando_if;

        // Third rule
        AST_Node_While* comando_while;

        // Fourth rule
        AST_Node_For* comando_for;

        // Fifth rule
        AST_Node_Printf* comando_printf;

        // Sixth rule
        AST_Node_Scanf* comando_scanf;

        // Seventh rule
        AST_Node_Exit* comando_exit;

        // Eighth rule
        AST_Node_Return* comando_return;

        // Ninth rule
        AST_Node_Expression* expressao;

        AST_Node_Command(AST_Node_Do_While* comando_do_while) {
            init();
            this->comando_do_while = comando_do_while;
        }

        AST_Node_Command(AST_Node_If* comando_if) {
            init();
            this->comando_if = comando_if;
        }

        AST_Node_Command(AST_Node_While* comando_while) {
            init();
            this->comando_while = comando_while;
        }

        AST_Node_Command(AST_Node_For* comando_for) {
            init();
            this->comando_for = comando_for;
        }

        AST_Node_Command(AST_Node_Printf* comando_printf) {
            init();
            this->comando_printf = comando_printf;
        }

        AST_Node_Command(AST_Node_Scanf* comando_scanf) {
            init();
            this->comando_scanf = comando_scanf;
        }

        AST_Node_Command(AST_Node_Exit* comando_exit) {
            init();
            this->comando_exit = comando_exit;
        }

        AST_Node_Command(AST_Node_Return* comando_return) {
            init();
            this->comando_return = comando_return;
        }

        AST_Node_Command(AST_Node_Expression* expressao) {
            init();
            this->expressao = expressao;
        }

       private:
        void init() {
            this->node_number = -1;
            this->parent = nullptr;
            this->node_content = this;
            this->node_name = AST_Nonterminals::Command;
            this->comando_do_while = nullptr;
            this->comando_if = nullptr;
            this->comando_while = nullptr;
            this->comando_for = nullptr;
            this->comando_printf = nullptr;
            this->comando_scanf = nullptr;
            this->comando_exit = nullptr;
            this->comando_return = nullptr;
            this->expressao = nullptr;
        }
    };

    class AST_Node_Temp_Coms_List : public AST_Node {
       public:
        // First rule
        AST_Node_Command* comando;
        AST_Node_Temp_Coms_List* lista_comandos_temporario;

        AST_Node_Temp_Coms_List(AST_Node_Command* comando, AST_Node_Temp_Coms_List* lista_comandos_temporario) {
            init();
            this->comando = comando;
            this->lista_comandos_temporario = lista_comandos_temporario;
        }

       private:
        void init() {
            this->node_number = -1;
            this->parent = nullptr;
            this->node_content = this;
            this->node_name = AST_Nonterminals::TempComsList;
            this->comando = nullptr;
            this->lista_comandos_temporario = nullptr;
        }
    };

    class AST_Node_Coms_List : public AST_Node {
       public:
        // First rule
        AST_Node_Command* comando;
        AST_Node_Temp_Coms_List* lista_comandos_temporario;

        AST_Node_Coms_List(AST_Node_Command* comando, AST_Node_Temp_Coms_List* lista_comandos_temporario) {
            init();
            this->comando = comando;
            this->lista_comandos_temporario = lista_comandos_temporario;
        }

       private:
        void init() {
            this->node_number = -1;
            this->parent = nullptr;
            this->node_content = this;
            this->node_name = AST_Nonterminals::ComsList;
            this->comando = nullptr;
            this->lista_comandos_temporario = nullptr;
        }
    };

    class AST_Node_Func_Body : public AST_Node {
       public:
        // First rule
        AST_Node_Coms_List* lista_comandos;
        std::string* function_name;

        AST_Node_Func_Body(AST_Node_Coms_List* lista_comandos) {
            init();
            this->lista_comandos = lista_comandos;
        }

       private:
        void init() {
            this->node_number = -1;
            this->parent = nullptr;
            this->node_content = this;
            this->node_name = AST_Nonterminals::FunctionBody;
            this->lista_comandos = nullptr;
            this->function_name = nullptr;
        }
    };

    class AST_Variable {
       public:
        std::string* name;
        std::string* type;
        std::string* value;
        std::vector<int> dimensions;

        AST_Variable(std::string* name, std::string* type, std::string* value) {
            this->name = name;
            this->type = type;
            this->value = value;
        }

        AST_Variable(std::string* name, std::string* type) {
            this->name = name;
            this->type = type;
            this->value = nullptr;
            this->dimensions = std::vector<int>();

            if (type->find("[") == std::string::npos) {
                return;
            }

            std::string dimension = "";

            for (int i = 0; i < type->length(); i++) {
                if (type->at(i) == '[') {
                    int j = i + 1;
                    while (type->at(j) != ']') {
                        dimension += type->at(j);
                        j++;
                    }
                    this->dimensions.push_back(std::stoi(dimension));
                    dimension = "";
                }
            }

        }
    };

    class AST_Constant {
       public:
        std::string* name;
        int value;

        AST_Constant(std::string* name, int value) {
            this->name = name;
            this->value = value;
        }
    };

    class AST_Parameter {
       public:
        std::string* name;
        std::string* type;

        AST_Parameter(std::string* name, std::string* type) {
            this->name = name;
            this->type = type;
        }
    };

    class AST_Function {
       public:
        std::string* function_name;
        std::string* return_type;
        std::vector<AST_Parameter*> parameters;
        std::vector<AST_Variable*> variables;
        AST_Node_Func_Body* function_body;

        AST_Function(std::string* function_name, std::string* return_type) {
            this->function_name = function_name;
            this->return_type = return_type;
        }

        void add_parameter(AST_Parameter* parameter) {
            this->parameters.push_back(parameter);
        }

        void add_variable(AST_Variable* variable) {
            this->variables.push_back(variable);
        }

        void set_function_body(AST_Node_Func_Body* function_body) {
            this->function_body = function_body;
        }
    };

    class AST_Node_Strings {
        public:
         std::vector<std::string> strings;
         int node_number;

        AST_Node_Strings(std::vector<std::string> strings, int node_number) {
            this->strings = strings;
            this->node_number = node_number;
        }

    };

    class AST_Memory_Access {
        public:
            std::string name;
            int base;
            int offset;
            int is_vector;
            int mapped;

            AST_Memory_Access(std::string name, int base, int offset, int is_vector) {
                this->name = name;
                this->base = base;
                this->offset = offset;
                this->is_vector = is_vector;
                this->mapped = 0;
            }
    };
        
    void sort_functions(std::vector<ast::AST_Function*> &funcoes);

};

#endif  // AST_HH