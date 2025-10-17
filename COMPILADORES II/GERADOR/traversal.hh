#ifndef TRAVERSAL_HH
#define TRAVERSAL_HH

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "ast.hh"

namespace traversal {
    
    void general_AST_available_functions(ast::AST_Function* function);
    void traversal_AST(ast::AST_Function* function, int print_graphviz, int free_AST, int produce_MIPS);
    void print_ASTs(std::vector<ast::AST_Function*> funtions);
    void free_ASTs(std::vector<ast::AST_Function*> funtions, std::vector<ast::AST_Constant*> constants, std::vector<ast::AST_Variable*> global_variables, std::vector<ast::AST_Node_Strings*> node_strings);

    void traversal_BOP(ast::AST_Node_BOP* runner, int print_graphviz, int free_AST, int produce_MIPS);
    void traversal_UOP(ast::AST_Node_UOP* runner, int print_graphviz, int free_AST, int produce_MIPS);
    void traversal_TOP(ast::AST_Node_TOP* runner, int print_graphviz, int free_AST, int produce_MIPS);
    void traversal_Expression(ast::AST_Node_Expression* runner, int print_graphviz, int free_AST, int produce_MIPS);
    void traversal_Function_Call(ast::AST_Node_Function_Call* runner, int print_graphviz, int free_AST, int produce_MIPS);
    void traversal_Variable_Acess(ast::AST_Node_Variable_Acess* runner, int print_graphviz, int free_AST, int produce_MIPS);
    void traversal_Loop_Expressions(ast::AST_Node_Loop_Expressions* runner, int print_graphviz, int free_AST, int produce_MIPS);
    void traversal_Temp_Loop_Expressions(ast::AST_Node_Temp_Loop_Expressions* runner, int print_graphviz, int free_AST, int produce_MIPS);
    void traversal_Loop_Matrix(ast::AST_Node_Loop_Matrix* runner, int print_graphviz, int free_AST, int produce_MIPS);
    void traversal_Stop_Condition(ast::AST_Node_Stop_Con* runner, int print_graphviz, int free_AST, int produce_MIPS);
    void traversal_Values_Fit(ast::AST_Node_Values_Fit* runner, int print_graphviz, int free_AST, int produce_MIPS);
    void traversal_Init_For(ast::AST_Node_Init_For* runner, int print_graphviz, int free_AST, int produce_MIPS);
    void traversal_Temp_Expressions_Print(ast::AST_Node_Temp_Expressions_Print* runner, int print_graphviz, int free_AST, int produce_MIPS);
    void traversal_Expressions_Print(ast::AST_Node_Expressions_Print* runner, int print_graphviz, int free_AST, int produce_MIPS);
    void traversal_Var_Adress(ast::AST_Node_Var_Adress* runner, int print_graphviz, int free_AST, int produce_MIPS);
    void traversal_Return_Command(ast::AST_Node_Return* runner, int print_graphviz, int free_AST, int produce_MIPS);
    void traversal_Exit_Command(ast::AST_Node_Exit* runner, int print_graphviz, int free_AST, int produce_MIPS);
    void traversal_Scanf_Command(ast::AST_Node_Scanf* runner, int print_graphviz, int free_AST, int produce_MIPS);
    void traversal_Printf_Command(ast::AST_Node_Printf* runner, int print_graphviz, int free_AST, int produce_MIPS);
    void traversal_For_Command(ast::AST_Node_For* runner, int print_graphviz, int free_AST, int produce_MIPS);
    void traversal_While_Command(ast::AST_Node_While* runner, int print_graphviz, int free_AST, int produce_MIPS);
    void traversal_If_Command(ast::AST_Node_If* runner, int print_graphviz, int free_AST, int produce_MIPS);
    void traversal_Do_While_Command(ast::AST_Node_Do_While* runner, int print_graphviz, int free_AST, int produce_MIPS);
    void traversal_Command(ast::AST_Node_Command* runner, int print_graphviz, int free_AST, int produce_MIPS);
    void traversal_Temp_Cons_List(ast::AST_Node_Temp_Coms_List* runner, int print_graphviz, int free_AST, int produce_MIPS);
    void traversal_Cons_List(ast::AST_Node_Coms_List* runner, int print_graphviz, int free_AST, int produce_MIPS);
    void traversal_Function_Body(ast::AST_Node_Func_Body* runner, int print_graphviz, int free_AST, int produce_MIPS);

};  
#endif // TRAVERSAL_HH