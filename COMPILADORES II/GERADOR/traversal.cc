#include "ast.hh"
#include "traversal.hh"
#include "mips.hh"

std::ofstream dotfile("test.dot");
int node_counter = 0;

extern std::vector<ast::AST_Constant*> constantes;
extern std::vector<ast::AST_Variable*> variaveis_globais;
extern std::vector<ast::AST_Node_Strings*> node_strings;
extern std::vector<ast::AST_Function*> funcoes;
std::vector<ast::AST_Memory_Access*> memory_acess;
ast::AST_Function* current_function;
bool used_registers[32] = {0};

std::vector<std::string> register_names = {
        "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
        "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
        "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
        "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"
};

enum registers {
    $ZERO,
    $AT,
    $V0,
    $V1,
    $A0,
    $A1,
    $A2,
    $A3,
    $T0,
    $T1,
    $T2,
    $T3,
    $T4,
    $T5,
    $T6,
    $T7,
    $S0,
    $S1,
    $S2,
    $S3,
    $S4,
    $S5,
    $S6,
    $S7,
    $T8,
    $T9,
    $K0,
    $K1,
    $GP,
    $SP,
    $FP,
    $RA
};

void ast::sort_functions(std::vector<ast::AST_Function*> &funcoes) {
    std::reverse(funcoes.begin(), funcoes.end());
}

void verifyEndl(bool isEnd) {
    if (isEnd) {
        std::cout ;
        verifyEndl(true);
    }
}

void traversal::general_AST_available_functions(ast::AST_Function* function) {
    std::cout << "Function name: " << *function->function_name;
    verifyEndl(true);
    std::cout << "Return type: " << *function->return_type;
    verifyEndl(true);
    std::cout << "Parameters: ";
    verifyEndl(true);
    for (auto parameter : function->parameters) {
        std::cout << "Name: " << *(parameter->name) << " Type: " << *(parameter->type);
        verifyEndl(true);
    }
    std::cout << "Variables: ";
    verifyEndl(true);
    for (auto variable : function->variables) {
        std::cout << "Name: " << *(variable->name) << " Type: " << *(variable->type) << " Value: " << (variable->value != nullptr ? *(variable->value) : "Not defined.");
        verifyEndl(true);
    }
    std::cout;
    verifyEndl(true);
}

void traversal::free_ASTs(std::vector<ast::AST_Function*> funcoes, std::vector<ast::AST_Constant*> constantes, std::vector<ast::AST_Variable*> variaveis_globais, std::vector<ast::AST_Node_Strings*> node_strings) {
    for (auto function : funcoes) {
        traversal::traversal_AST(function, 0, 1, 0);
        delete function->function_name;
        delete function->return_type;

        for (auto parameter : function->parameters) {
            delete parameter->name;
            delete parameter->type;
            delete parameter;
        }

        for (auto variable : function->variables) {
            delete variable->name;
            delete variable->type;
            delete variable->value;
            delete variable;
        }

        delete function;
    }

    for (auto constant : constantes) {
        delete constant->name;
        delete constant;
    }

    for (auto variable : variaveis_globais) {
        delete variable->name;
        delete variable->type;
        delete variable->value;
        delete variable;
    }

    for (auto node_string : node_strings) {
        delete node_string;
    }


}

void traversal::print_ASTs(std::vector<ast::AST_Function*> funcoes) {
    dotfile << "digraph ARV {";
    verifyEndl(true);
    dotfile << "graph [pad=\"0.5\", nodesep=\"0.5\", ranksep=\"2\"];";
    verifyEndl(true);

    for (auto function : funcoes) {
        traversal::traversal_AST(function, 1, 0, 0);
    }

    dotfile << "}";
    verifyEndl(true);
    dotfile.close();
}

void traversal::traversal_AST(ast::AST_Function* function, int print_graphviz, int free_AST, int produce_MIPS) {
    if (function->function_body == nullptr) {
        std::cout << "Function body is empty.";
        verifyEndl(true);
        return;
    }

    function->function_body->function_name = function->function_name;

    current_function = function;

    if (produce_MIPS) {

        std::cout << *function->function_name << ":";
        verifyEndl(true);

        if (*function->function_name != "main") {

            for (int i = 0; i < 32; i++) {
                helpers::free_register(i);
            }
            
        }

    }

    traversal::traversal_Function_Body(function->function_body, print_graphviz, free_AST, produce_MIPS);

    if (*function->function_name != "main" && produce_MIPS) {
        for (int i = 0; i < 32; i++) {
            helpers::free_register(i);
        }
        std::cout << "\tjr $ra";
        verifyEndl(true);
    }

    if(produce_MIPS) mips::ops::exit();

}

void traversal::traversal_BOP(ast::AST_Node_BOP* runner, int print_graphviz, int free_AST, int produce_MIPS) {
    if (runner == nullptr) return;

    if (runner->node_number == -1) {
        runner->node_number = node_counter++;
    }

    if (runner->left) {
        runner->left->parent = runner;
    }

    if (runner->right) {
        runner->right->parent = runner;
    }

    traversal::traversal_Expression(runner->left, print_graphviz, free_AST, produce_MIPS);
    traversal::traversal_Expression(runner->right, print_graphviz, free_AST, produce_MIPS);

    if (produce_MIPS) {

        int reg1 = runner->left->mapped_to_register;
        int reg2 = runner->right->mapped_to_register;

        if (reg1 == -1 || reg2 == -1) {
            std::cout << runner->node_number;
            verifyEndl(true);
            std::cout << "Error: Trying to perform a " << runner->operation << " with two values that are not mapped to registers.";
            verifyEndl(true);
            exit(1);
        }

        int variable_type_1 = helpers::return_register_type(reg1);
        int variable_type_2 = helpers::return_register_type(reg2);

        if (variable_type_1 == helpers::V && variable_type_2 == helpers::V) {
            reg1 = helpers::return_first_unused_register();
            reg2 = helpers::return_first_unused_register();

            std::cout << "\tlw " << register_names[reg1] << ", 0($sp)";
            verifyEndl(true);
            std::cout << "\taddi $sp, $sp, 4";
            verifyEndl(true);
            std::cout << "\tlw " << register_names[reg2] << ", 0($sp)";
            verifyEndl(true);
            std::cout << "\taddi $sp, $sp, 4";
            verifyEndl(true);

            variable_type_1 = helpers::return_register_type(reg1);
            variable_type_2 = helpers::return_register_type(reg2);

        } else if (variable_type_1 == helpers::V) {

            std::cout << "\tlw " << register_names[reg1] << ", 0($sp)";
            verifyEndl(true);
            std::cout << "\taddi $sp, $sp, 4";
            verifyEndl(true);

            variable_type_1 = helpers::return_register_type(reg1);

        } else if (variable_type_2 == helpers::V) {

            std::cout << "\tlw " << register_names[reg2] << ", 0($sp)";
            verifyEndl(true);
            std::cout << "\taddi $sp, $sp, 4";
            verifyEndl(true);

            variable_type_2 = helpers::return_register_type(reg2);

        }

        int reg_to_store = -1;

        if (variable_type_1 == helpers::T && variable_type_2 == helpers::T) {
            reg_to_store = reg1;
        }
        
        if (variable_type_1 == helpers::T || variable_type_2 == helpers::T) {
            if (variable_type_1 == helpers::T) {
                reg_to_store = reg1;
            } else {
                reg_to_store = reg2;
            }
        }

        if (reg_to_store == -1) { 
            reg_to_store = helpers::return_first_unused_register();

            if (reg_to_store == -1) {
                std::cout << "Error: No registers available.";
                verifyEndl(true);
                exit(1);
            }
        }

        int is_left_memory_access = 0;
        int is_right_memory_access = 0;
        int base_left = -1;
        int base_right = -1;
        std::string variable_name_left = "";
        int offset_left = -1;
        int offset_right = -1;
        std::string variable_name_right = "";

        if (runner->left->variable_acess != nullptr) {
            int i = 0;
            for (auto mem : memory_acess) {
                if (mem->name == *(runner->left->variable_acess->variable_name) && !mem->mapped) {

                    std::cout << "\tla " << register_names[mem->base] << ", " << *(runner->left->variable_acess->variable_name);
                    verifyEndl(true);
                    std::cout << "\tadd " << register_names[reg1] << ", " << register_names[mem->base] << ", " << register_names[mem->offset];
                    verifyEndl(true);

                    if (runner->operation == "ASSIGN" || runner->operation == "ADD_ASSIGN" || runner->operation == "MINUS_ASSIGN") {

                    } else {
                        std::cout << "\tlw " << register_names[reg1] << ", 0(" << register_names[reg1] << ")";
                        verifyEndl(true);
                    }

                    is_left_memory_access = 1;
                    base_left = mem->base;
                    offset_left = mem->offset;
                    variable_name_left = *(runner->left->variable_acess->variable_name);
                    if (reg1 != mem->base) helpers::free_register(mem->base);
                    helpers::free_register(mem->offset);
                    mem->mapped = true;
                    break;
                }
                i++;
            }
        }

        if (runner->right->variable_acess != nullptr) {
            int i = 0;
            for (auto mem : memory_acess) {
                if (mem->name == *(runner->right->variable_acess->variable_name) && !mem->mapped) {

                    std::cout << "\tla " << register_names[mem->base] << ", " << *(runner->right->variable_acess->variable_name);
                    verifyEndl(true);
                    std::cout << "\tadd " << register_names[reg2] << ", " << register_names[mem->base] << ", " << register_names[mem->offset];
                    verifyEndl(true);

                    std::cout << "\tlw " << register_names[reg2] << ", 0(" << register_names[reg2] << ")";
                    verifyEndl(true);

                    is_right_memory_access = 1;
                    base_right = mem->base;
                    offset_right = mem->offset;
                    variable_name_right = *(runner->right->variable_acess->variable_name);
                    if (reg2 != mem->base) helpers::free_register(mem->base);
                    helpers::free_register(mem->offset);
                    mem->mapped = true;
                    break;
                }
                i++;
            }
        }

        int change = 1;

        if (runner->operation == "PLUS") {

            std::cout << "\tadd " << register_names[reg_to_store] << ", " << register_names[reg1] << ", " << register_names[reg2];
            verifyEndl(true);

            if (is_left_memory_access) {
                if(reg_to_store != reg1) helpers::free_register(reg1);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_left && memory_acess[i]->base == base_left && memory_acess[i]->offset == offset_left && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (is_right_memory_access) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_right && memory_acess[i]->base == base_right && memory_acess[i]->offset == offset_right && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (variable_type_1 == helpers::T) {
                if (reg_to_store != reg1) helpers::free_register(reg1);
            }

            if (variable_type_2 == helpers::T) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
            }



        } else if (runner->operation == "MINUS") {

            std::cout << "\tsub " << register_names[reg_to_store] << ", " << register_names[reg1] << ", " << register_names[reg2];
            verifyEndl(true);

            if (is_left_memory_access) {
                if(reg_to_store != reg1) helpers::free_register(reg1);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_left && memory_acess[i]->base == base_left && memory_acess[i]->offset == offset_left && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (is_right_memory_access) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_right && memory_acess[i]->base == base_right && memory_acess[i]->offset == offset_right && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (variable_type_1 == helpers::T) {
                if (reg_to_store != reg1) helpers::free_register(reg1);
            }

            if (variable_type_2 == helpers::T) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
            }


        } else if (runner->operation == "MULTIPLY") {

            std::cout << "\tmul " << register_names[reg_to_store] << ", " << register_names[reg1] << ", " << register_names[reg2];
            verifyEndl(true);

            if (is_left_memory_access) {
                if(reg_to_store != reg1) helpers::free_register(reg1);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_left && memory_acess[i]->base == base_left && memory_acess[i]->offset == offset_left && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (is_right_memory_access) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_right && memory_acess[i]->base == base_right && memory_acess[i]->offset == offset_right && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (variable_type_2 == helpers::T) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
            }

            if (variable_type_2 == helpers::T) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
            }


        } else if (runner->operation == "DIV") {

            std::cout << "\tdiv " << register_names[reg_to_store] << ", " << register_names[reg1] << ", " << register_names[reg2];
            verifyEndl(true);
            std::cout << "\tmflo " << register_names[reg_to_store];
            verifyEndl(true);

            if (is_left_memory_access) {
                if(reg_to_store != reg1) helpers::free_register(reg1);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_left && memory_acess[i]->base == base_left && memory_acess[i]->offset == offset_left && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (is_right_memory_access) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_right && memory_acess[i]->base == base_right && memory_acess[i]->offset == offset_right && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (variable_type_1 == helpers::T) {
                if (reg_to_store != reg1) helpers::free_register(reg1);
            }

            if (variable_type_2 == helpers::T) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
            }

            
        } else if (runner->operation == "REMAINDER") {

            std::cout << "\tdiv " << register_names[reg_to_store] << ", " << register_names[reg1] << ", " << register_names[reg2];
            verifyEndl(true);
            std::cout << "\tmfhi " << register_names[reg_to_store];
            verifyEndl(true);

            if (is_left_memory_access) {
                if(reg_to_store != reg1) helpers::free_register(reg1);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_left && memory_acess[i]->base == base_left && memory_acess[i]->offset == offset_left && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (is_right_memory_access) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_right && memory_acess[i]->base == base_right && memory_acess[i]->offset == offset_right && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (variable_type_1 == helpers::T) {
                if (reg_to_store != reg1) helpers::free_register(reg1);
            }

            if (variable_type_2 == helpers::T) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
            }

            
        } else if (runner->operation == "BITWISE_AND") {

            std::cout << "\tand " << register_names[reg_to_store] << ", " << register_names[reg1] << ", " << register_names[reg2];
            verifyEndl(true);

            if (is_left_memory_access) {
                if(reg_to_store != reg1) helpers::free_register(reg1);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_left && memory_acess[i]->base == base_left && memory_acess[i]->offset == offset_left && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (is_right_memory_access) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_right && memory_acess[i]->base == base_right && memory_acess[i]->offset == offset_right && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (variable_type_1 == helpers::T) {
                if (reg_to_store != reg1) helpers::free_register(reg1);
            }

            if (variable_type_2 == helpers::T) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
            }

            
        } else if (runner->operation == "BITWISE_OR") {

            std::cout << "\tor " << register_names[reg_to_store] << ", " << register_names[reg1] << ", " << register_names[reg2];
            verifyEndl(true);

            if (is_left_memory_access) {
                if(reg_to_store != reg1) helpers::free_register(reg1);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_left && memory_acess[i]->base == base_left && memory_acess[i]->offset == offset_left && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (is_right_memory_access) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_right && memory_acess[i]->base == base_right && memory_acess[i]->offset == offset_right && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (variable_type_1 == helpers::T) {
                if (reg_to_store != reg1) helpers::free_register(reg1);
            }

            if (variable_type_2 == helpers::T) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
            }

            
        } else if (runner->operation == "BITWISE_XOR") {

            std::cout << "\txor " << register_names[reg_to_store] << ", " << register_names[reg1] << ", " << register_names[reg2];
            verifyEndl(true);

            if (is_left_memory_access) {
                if(reg_to_store != reg1) helpers::free_register(reg1);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_left && memory_acess[i]->base == base_left && memory_acess[i]->offset == offset_left && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (is_right_memory_access) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_right && memory_acess[i]->base == base_right && memory_acess[i]->offset == offset_right && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (variable_type_1 == helpers::T) {
                if (reg_to_store != reg1) helpers::free_register(reg1);
            }

            if (variable_type_2 == helpers::T) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
            }

            
        } else if (runner->operation == "LOGICAL_AND") {

            int first_temp = helpers::return_first_unused_register();
            int second_temp = helpers::return_first_unused_register();

            std::cout << "\tsltu " << register_names[first_temp] << ", $zero" << ", " << register_names[reg1];
            verifyEndl(true);
            std::cout << "\tsltu " << register_names[second_temp] << ", $zero" << ", " << register_names[reg2];
            verifyEndl(true);
            std::cout << "\tand " << register_names[reg_to_store] << ", " << register_names[first_temp] << ", " << register_names[second_temp];
            verifyEndl(true);
            helpers::free_register(first_temp);
            helpers::free_register(second_temp);

            if (is_left_memory_access) {
                if(reg_to_store != reg1) helpers::free_register(reg1);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_left && memory_acess[i]->base == base_left && memory_acess[i]->offset == offset_left && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (is_right_memory_access) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_right && memory_acess[i]->base == base_right && memory_acess[i]->offset == offset_right && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (variable_type_1 == helpers::T) {
                if (reg_to_store != reg1) helpers::free_register(reg1);
            }

            if (variable_type_2 == helpers::T) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
            }

            
        } else if (runner->operation == "LOGICAL_OR") {

            int first_temp = helpers::return_first_unused_register();
            int second_temp = helpers::return_first_unused_register();

            std::cout << "\tsltu " << register_names[first_temp] << ", $zero" << ", " << register_names[reg1];
            verifyEndl(true);
            std::cout << "\tsltu " << register_names[second_temp] << ", $zero" << ", " << register_names[reg2];
            verifyEndl(true);
            std::cout << "\tor " << register_names[reg_to_store] << ", " << register_names[first_temp] << ", " << register_names[second_temp];
            verifyEndl(true);
            helpers::free_register(first_temp);
            helpers::free_register(second_temp);

            if (is_left_memory_access) {
                if(reg_to_store != reg1) helpers::free_register(reg1);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_left && memory_acess[i]->base == base_left && memory_acess[i]->offset == offset_left && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (is_right_memory_access) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_right && memory_acess[i]->base == base_right && memory_acess[i]->offset == offset_right && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (variable_type_1 == helpers::T) {
                if (reg_to_store != reg1) helpers::free_register(reg1);
            }

            if (variable_type_2 == helpers::T) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
            }

            
        } else if (runner->operation == "EQUAL") {

            std::cout << "\tsubu " << register_names[reg_to_store] << ", " << register_names[reg1] << ", " << register_names[reg2];
            verifyEndl(true);
            std::cout << "\tsltiu " << register_names[reg_to_store] << ", " << register_names[reg_to_store] << ", 1";
            verifyEndl(true);

            if (is_left_memory_access) {
                if(reg_to_store != reg1) helpers::free_register(reg1);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_left && memory_acess[i]->base == base_left && memory_acess[i]->offset == offset_left && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (is_right_memory_access) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_right && memory_acess[i]->base == base_right && memory_acess[i]->offset == offset_right && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (variable_type_1 == helpers::T) {
                if (reg_to_store != reg1) helpers::free_register(reg1);
            }

            if (variable_type_2 == helpers::T) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
            }

            
        } else if (runner->operation == "NOT_EQUAL") {

            std::cout << "\tsubu " << register_names[reg_to_store] << ", " << register_names[reg1] << ", " << register_names[reg2];
            verifyEndl(true);
            std::cout << "\tsltu " << register_names[reg_to_store] << ", $zero" << ", " << register_names[reg1];
            verifyEndl(true);

            if (is_left_memory_access) {
                if(reg_to_store != reg1) helpers::free_register(reg1);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_left && memory_acess[i]->base == base_left && memory_acess[i]->offset == offset_left && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (is_right_memory_access) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_right && memory_acess[i]->base == base_right && memory_acess[i]->offset == offset_right && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (variable_type_1 == helpers::T) {
                if (reg_to_store != reg1) helpers::free_register(reg1);
            }

            if (variable_type_2 == helpers::T) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
            }

            
        } else if (runner->operation == "LESS_THAN") {

            std::cout << "\tslt " << register_names[reg_to_store] << ", " << register_names[reg1] << ", " << register_names[reg2];
            verifyEndl(true);

            if (is_left_memory_access) {
                if(reg_to_store != reg1) helpers::free_register(reg1);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_left && memory_acess[i]->base == base_left && memory_acess[i]->offset == offset_left && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (is_right_memory_access) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_right && memory_acess[i]->base == base_right && memory_acess[i]->offset == offset_right && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (variable_type_1 == helpers::T) {
                if (reg_to_store != reg1) helpers::free_register(reg1);
            }

            if (variable_type_2 == helpers::T) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
            }

            
        } else if (runner->operation == "GREATER_THAN") {

            std::cout << "\tslt " << register_names[reg_to_store] << ", " << register_names[reg2] << ", " << register_names[reg1];
            verifyEndl(true);

            if (is_left_memory_access) {
                if(reg_to_store != reg1) if(reg_to_store != reg1) helpers::free_register(reg1);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_left && memory_acess[i]->base == base_left && memory_acess[i]->offset == offset_left && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (is_right_memory_access) {
                if(reg_to_store != reg2) if(reg_to_store != reg2) helpers::free_register(reg2);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_right && memory_acess[i]->base == base_right && memory_acess[i]->offset == offset_right && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (variable_type_1 == helpers::T) {
                if (reg_to_store != reg1) if(reg_to_store != reg1) helpers::free_register(reg1);
            }

            if (variable_type_2 == helpers::T) {
                if(reg_to_store != reg2) if(reg_to_store != reg2) helpers::free_register(reg2);
            }

            
        } else if (runner->operation == "LESS_EQUAL") {

            int reg_temp = helpers::return_first_unused_register();

            std::cout << "\tslt " << register_names[reg_temp] << ", " << register_names[reg2] << ", " << register_names[reg1];
            verifyEndl(true);
            std::cout << "\tori " << register_names[reg_to_store] << ", $zero" << ", 1";
            verifyEndl(true);
            std::cout << "\tsubu " << register_names[reg_to_store] << ", " << register_names[reg_to_store] << ", " << register_names[reg_temp];
            verifyEndl(true);

            if (is_left_memory_access) {
                if(reg_to_store != reg1) helpers::free_register(reg1);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_left && memory_acess[i]->base == base_left && memory_acess[i]->offset == offset_left && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (is_right_memory_access) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_right && memory_acess[i]->base == base_right && memory_acess[i]->offset == offset_right && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (variable_type_1 == helpers::T) {
                if (reg_to_store != reg1) helpers::free_register(reg1);
            }

            if (variable_type_2 == helpers::T) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
            }


            helpers::free_register(reg_temp);
            
        } else if (runner->operation == "GREATER_EQUAL") {

            int reg_temp = helpers::return_first_unused_register();

            std::cout << "\tslt " << register_names[reg_temp] << ", " << register_names[reg1] << ", " << register_names[reg2];
            verifyEndl(true);
            std::cout << "\tori " << register_names[reg_to_store] << ", $zero" << ", 1";
            verifyEndl(true);
            std::cout << "\tsubu " << register_names[reg_to_store] << ", " << register_names[reg_to_store] << ", " << register_names[reg_temp];
            verifyEndl(true);

            if (is_left_memory_access) {
                if(reg_to_store != reg1) helpers::free_register(reg1);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_left && memory_acess[i]->base == base_left && memory_acess[i]->offset == offset_left && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (is_right_memory_access) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_right && memory_acess[i]->base == base_right && memory_acess[i]->offset == offset_right && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (variable_type_1 == helpers::T) {
                if (reg_to_store != reg1) helpers::free_register(reg1);
            }

            if (variable_type_2 == helpers::T) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
            }


            helpers::free_register(reg_temp);
            
        } else if (runner->operation == "R_SHIFT") {

            std::cout << "\tsrlv " << register_names[reg_to_store] << ", " << register_names[reg1] << ", " << register_names[reg2];
            verifyEndl(true);

            if (is_left_memory_access) {
                if(reg_to_store != reg1) helpers::free_register(reg1);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_left && memory_acess[i]->base == base_left && memory_acess[i]->offset == offset_left && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (is_right_memory_access) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_right && memory_acess[i]->base == base_right && memory_acess[i]->offset == offset_right && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (variable_type_1 == helpers::T) {
                if (reg_to_store != reg1) helpers::free_register(reg1);
            }

            if (variable_type_2 == helpers::T) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
            }


            
        } else if (runner->operation == "L_SHIFT") {

            std::cout << "\tsllv " << register_names[reg_to_store] << ", " << register_names[reg1] << ", " << register_names[reg2];
            verifyEndl(true);

            if (is_left_memory_access) {
                if(reg_to_store != reg1) helpers::free_register(reg1);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_left && memory_acess[i]->base == base_left && memory_acess[i]->offset == offset_left && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (is_right_memory_access) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_right && memory_acess[i]->base == base_right && memory_acess[i]->offset == offset_right && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }
            }

            if (variable_type_1 == helpers::T) {
                if (reg_to_store != reg1) helpers::free_register(reg1);
            }

            if (variable_type_2 == helpers::T) {
                if(reg_to_store != reg2) helpers::free_register(reg2);
            }


            
        } else if (runner->operation == "ASSIGN") {

            std::cout << "\tadd " << register_names[reg1] << ", " << register_names[reg2] << ", $zero";
            verifyEndl(true);

            std::cout;
            verifyEndl(true);

            ((ast::AST_Node_Expression*)runner->parent)->mapped_to_register = reg1;

            change = 0;

            if (is_left_memory_access) {
                std::cout << "\tsw " << register_names[reg1] << ", " << variable_name_left << "(" << register_names[offset_left] << ")";
                verifyEndl(true);
                helpers::free_register(reg1);
                if (variable_type_2 == helpers::T) helpers::free_register(reg2);
                helpers::free_register(offset_left);
                helpers::free_register(base_left);

                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_left && memory_acess[i]->base == base_left && memory_acess[i]->offset == offset_left && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }


            } 

            if (is_right_memory_access) {
                helpers::free_register(offset_right);
                helpers::free_register(base_right);

                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_right && memory_acess[i]->base == base_right && memory_acess[i]->offset == offset_right && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }

            }

            if (is_left_memory_access) {
                helpers::free_register(reg1);
            }

            if (is_right_memory_access) {
                helpers::free_register(reg2);
            }

            if (variable_type_2 == helpers::T) {
                helpers::free_register(reg2);
            }

            helpers::free_register(reg_to_store);


            
        } else if (runner->operation == "ADD_ASSIGN") {

            int reg_temp = reg1;

            if (is_left_memory_access) {
                reg_temp = helpers::return_first_unused_register();
                std::cout << "\tlw " << register_names[reg_temp] << ", " << variable_name_left << "(" << register_names[offset_left] << ")";
                verifyEndl(true);
            }

            std::cout << "\tadd " << register_names[reg1] << ", " << register_names[reg_temp] << ", " << register_names[reg2];
            verifyEndl(true);

            ((ast::AST_Node_Expression*)runner->parent)->mapped_to_register = reg1;

            change = 0;

            if (is_left_memory_access) {
                std::cout << "\tsw " << register_names[reg1] << ", " << variable_name_left << "(" << register_names[offset_left] << ")";
                verifyEndl(true);
                helpers::free_register(reg1);
                if (variable_type_2 == helpers::T) helpers::free_register(reg2);
                helpers::free_register(offset_left);
                helpers::free_register(base_left);
                helpers::free_register(reg_temp);

                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_left && memory_acess[i]->base == base_left && memory_acess[i]->offset == offset_left && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }

                
            }

            if (is_right_memory_access) {
                helpers::free_register(offset_right);
                helpers::free_register(base_right);

                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_right && memory_acess[i]->base == base_right && memory_acess[i]->offset == offset_right && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }

            }

            if (is_left_memory_access) {
                helpers::free_register(reg1);
            }

            if (is_right_memory_access) {
                helpers::free_register(reg2);
            }

            if (variable_type_2 == helpers::T) {
                helpers::free_register(reg2);
            }


            helpers::free_register(reg_to_store);
            
        } else if (runner->operation == "MINUS_ASSIGN") {

            int reg_temp = reg1;

            if (is_left_memory_access) {
                reg_temp = helpers::return_first_unused_register();
                std::cout << "\tlw " << register_names[reg_temp] << ", " << variable_name_left << "(" << register_names[offset_left] << ")";
                verifyEndl(true);
            }

            std::cout << "\tsub " << register_names[reg1] << ", " << register_names[reg_temp] << ", " << register_names[reg2];
            verifyEndl(true);

            ((ast::AST_Node_Expression*)runner->parent)->mapped_to_register = reg1;

            change = 0;

            if (is_left_memory_access) {
                std::cout << "\tsw " << register_names[reg1] << ", " << variable_name_left << "(" << register_names[offset_left] << ")";
                verifyEndl(true);
                helpers::free_register(reg1);
                if (variable_type_2 == helpers::T) helpers::free_register(reg2);
                helpers::free_register(offset_left);
                helpers::free_register(base_left);
                helpers::free_register(reg_temp);

                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_left && memory_acess[i]->base == base_left && memory_acess[i]->offset == offset_left && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }

                
            }

            if (is_right_memory_access) {
                helpers::free_register(offset_right);
                helpers::free_register(base_right);

                for (int i = 0; i < memory_acess.size(); i++) {
                    if (memory_acess[i]->name == variable_name_right && memory_acess[i]->base == base_right && memory_acess[i]->offset == offset_right && memory_acess[i]->mapped) {
                        delete memory_acess[i];
                        memory_acess.erase(memory_acess.begin() + i);
                        break;
                    }
                }

            }

            if (is_left_memory_access) {
                helpers::free_register(reg1);
            }

            if (is_right_memory_access) {
                helpers::free_register(reg2);
            }

            if (variable_type_2 == helpers::T) {
                helpers::free_register(reg2);
            }


            helpers::free_register(reg_to_store);
            
        }

        runner->right->mapped_to_register = -1;
        runner->left->mapped_to_register = -1;
        runner->mapped_to_register = reg_to_store;

        if (change) ((ast::AST_Node_Expression*)runner->parent)->mapped_to_register = reg_to_store;

    }

    if (print_graphviz) {
        if (runner->left != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->left->node_number << ";";
            verifyEndl(true);
        }

        if (runner->right != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->right->node_number << ";";
            verifyEndl(true);
        }

        dotfile << "\t" << runner->node_number << " [shape = box, label = \"" << runner->operation << "\"];";
        verifyEndl(true);
    }

    if (free_AST) {
        delete runner;
    }
}

void traversal::traversal_UOP(ast::AST_Node_UOP* runner, int print_graphviz, int free_AST, int produce_MIPS) {
    if (runner == nullptr) return;

    if (runner->node_number == -1) {
        runner->node_number = node_counter++;
    }

    if (runner->child) {
        runner->child->parent = runner;
    }

    traversal::traversal_Expression(runner->child, print_graphviz, free_AST, produce_MIPS);

    if (produce_MIPS) {

        int reg1 = runner->child->mapped_to_register;

        if (reg1 == -1) {
            std::cout << "ERROR: UOP child not mapped to register";
            verifyEndl(true);
            exit(1);
        }

        if (runner->operation == "PLUS") {

            std::cout << "\tadd " << register_names[reg1] << ", $zero, " << register_names[reg1];
            verifyEndl(true);

        } else if (runner->operation == "MINUS") {

            std::cout << "\tsub " << register_names[reg1] << ", $zero, " << register_names[reg1];
            verifyEndl(true);

        } else if (runner->operation == "MULTIPLY") {

            std::cout << "Acesso a memoria :D";
            verifyEndl(true);
            
        } else if (runner->operation == "INC") {
            
            if (runner->is_postfix) {

                std::cout << "\taddiu " << register_names[reg1] << ", " << register_names[reg1] << ", 1";
                verifyEndl(true);

            } else {

                std::cout << "\taddiu " << register_names[reg1] << ", " << register_names[reg1] << ", 1";
                verifyEndl(true);

            }

        } else if (runner->operation == "DEC") {

            if (runner->is_postfix) {

                std::cout << "\taddiu " << register_names[reg1] << ", " << register_names[reg1] << ", -1";
                verifyEndl(true);

            } else {

                std::cout << "\taddiu " << register_names[reg1] << ", " << register_names[reg1] << ", -1";
                verifyEndl(true);

            }
            
        } else if (runner->operation == "BITWISE_NOT") {

            std::cout << "\tnor " << register_names[reg1] << ", " << register_names[reg1] << ", $zero";
            verifyEndl(true);
            
        } else if (runner->operation == "NOT") {

            std::cout << "\txori " << register_names[reg1] << ", " << register_names[reg1] << ", 1";
            verifyEndl(true);
            
        }

        runner->child->mapped_to_register = -1;
        runner->mapped_to_register = reg1;

        ((ast::AST_Node_Expression*)runner->parent)->mapped_to_register = reg1;

    }

    if (print_graphviz) {
        if (runner->child != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->child->node_number << ";";
            verifyEndl(true);
        }

        dotfile << 
        "\t" << 
        runner->node_number << 
        " [shape = box, label = \"" 
        << runner->operation 
        << (runner->is_postfix ? "postfix" : "prefix")
        << "\"];";
        verifyEndl(true);
    }

    if (free_AST) {
        delete runner;
    }

}

void traversal::traversal_TOP(ast::AST_Node_TOP* runner, int print_graphviz, int free_AST, int produce_MIPS) {
    if (runner == nullptr) return;

    if (runner->node_number == -1) {
        runner->node_number = node_counter++;
    }

    if (runner->test) {
        runner->test->parent = runner;
    }

    if (runner->left) {
        runner->left->parent = runner;
    }

    if (runner->right) {
        runner->right->parent = runner;
    }

    traversal::traversal_Expression(runner->test, print_graphviz, free_AST, produce_MIPS);

    if (produce_MIPS) {
        int reg1 = runner->test->mapped_to_register;

        if (reg1 == -1) {
            std::cout << "ERROR: TOP test not mapped to register";
            verifyEndl(true);
            exit(1);
        }

        std::cout << "\tbeq " << register_names[reg1] << ", $zero, TOP_" << runner->node_number << "_ELSE";
        verifyEndl(true);

    }

    if (produce_MIPS) std::cout << "TOP_" << runner->node_number << "_THEN:";
    verifyEndl(true);

    traversal::traversal_Expression(runner->left, print_graphviz, free_AST, produce_MIPS);

    if (produce_MIPS) {
        std::cout << "\tj TOP_" << runner->node_number << "_END";
        verifyEndl(true);
        std::cout << "TOP_" << runner->node_number << "_ELSE:";
        verifyEndl(true);
    }

    traversal::traversal_Expression(runner->right, print_graphviz, free_AST, produce_MIPS);

    if (produce_MIPS) {
        std::cout << "TOP_" << runner->node_number << "_END:";
        verifyEndl(true);
    }


    if (print_graphviz) {
        if (runner->test != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->test->node_number << ";";
            verifyEndl(true);
        }

        if (runner->left != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->left->node_number << ";";
            verifyEndl(true);
        }

        if (runner->right != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->right->node_number << ";";
            verifyEndl(true);
        }

        dotfile << "\t" << runner->node_number << " [shape = box, label = \""
                << "?"
                << "\"];";
                verifyEndl(true);
    }

    if (free_AST) {
        delete runner;
    }
}

void traversal::traversal_Expression(ast::AST_Node_Expression* runner, int print_graphviz, int free_AST, int produce_MIPS) {
    if (runner == nullptr) return;

    if (runner->node_number == -1) {
        runner->node_number = node_counter++;
    }

    if (runner->bop) {
        runner->bop->parent = runner;
    }

    if (runner->uop) {
        runner->uop->parent = runner;
    }

    if (runner->top) {
        runner->top->parent = runner;
    }

    if (runner->func_call) {
        runner->func_call->parent = runner;
    }

    if (runner->variable_acess) {
        runner->variable_acess->parent = runner;
    }

    traversal::traversal_BOP(runner->bop, print_graphviz, free_AST, produce_MIPS);
    traversal::traversal_UOP(runner->uop, print_graphviz, free_AST, produce_MIPS);
    traversal::traversal_TOP(runner->top, print_graphviz, free_AST, produce_MIPS);
    traversal::traversal_Function_Call(runner->func_call, print_graphviz, free_AST, produce_MIPS);
    traversal::traversal_Variable_Acess(runner->variable_acess, print_graphviz, free_AST, produce_MIPS);

    if (produce_MIPS) {

        if (runner->using_int) {

            int reg1 = helpers::return_first_unused_register();
            runner->mapped_to_register = reg1;

            std::cout << "\taddi " << register_names[reg1] << ", $zero, " << runner->num_int;
            verifyEndl(true);

        } else if (runner->using_char) {

            int reg1 = helpers::return_first_unused_register();
            runner->mapped_to_register = reg1;

            std::cout << "\tadd " << register_names[reg1] << ", $zero, '" << runner->character << "'" <<std::endl;

        } else if (runner->using_string) {
            std::cout << "Usando string direto na expressao!";
            verifyEndl(true);
        }
    }

    if (print_graphviz) {
        if (runner->bop != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->bop->node_number << ";";
            verifyEndl(true);
        }

        if (runner->uop != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->uop->node_number << ";";
            verifyEndl(true);
        }

        if (runner->top != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->top->node_number << ";";
            verifyEndl(true);
        }

        if (runner->func_call != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->func_call->node_number << ";";
            verifyEndl(true);
        }

        if (runner->variable_acess != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->variable_acess->node_number << ";";
            verifyEndl(true);
        }

        std::string variable;

        if (runner->using_string) {
            variable = std::string(*(runner->string));
        } else if (runner->using_char) {
            variable = std::string(1, runner->character);
        } else if (runner->using_int) {
            variable = std::to_string(runner->num_int);
        }

        dotfile << "\t" << runner->node_number << " [shape = box, label = \""
                << "Expressão\n"
                << variable
                << "\"];";
                verifyEndl(true);
    }

    if (runner->using_string && !runner->strings_added) {
        helpers::split_format_string(*(runner->string), runner->node_number);
        runner->strings_added = true;
    }

    if (free_AST) {

        if(runner->string != nullptr) delete runner->string;
        delete runner;

    }

}

void traversal::traversal_Function_Call(ast::AST_Node_Function_Call* runner, int print_graphviz, int free_AST, int produce_MIPS) {
    if (runner == nullptr) return;

    if (runner->node_number == -1) {
        runner->node_number = node_counter++;
    }

    if (runner->loop_expressoes) {
        runner->loop_expressoes->parent = runner;
    }

    traversal::traversal_Loop_Expressions(runner->loop_expressoes, print_graphviz, free_AST, produce_MIPS);

    if (produce_MIPS) {

        std::cout << "\tjal " << *(runner->function_name);
        verifyEndl(true);
        
        mips::ops::load_context_from_stack();
        runner->mapped_to_register = $V0;

        ((ast::AST_Node_Expression*)(runner->parent))->mapped_to_register = $V0;

        helpers::free_register($V0);

        ast::AST_Function* function_temp = nullptr;

        for (auto function : funcoes) {
            if (*(function->function_name) == *(runner->function_name)) {
                function_temp = function;
                break;
            }
        }

        if (*(function_temp->return_type) != "void") {
            std::cout << "\taddiu $sp, $sp, -4";
            verifyEndl(true);
            std::cout << "\tsw " << register_names[$V0] << ", 0($sp)";
            verifyEndl(true);
        }


    }

    if (print_graphviz) {
        if (runner->loop_expressoes != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->loop_expressoes->node_number << ";";
            verifyEndl(true);
        }

        dotfile << "\t" << runner->node_number << " [shape = box, label = \"Chamada Funcao " << *(runner->function_name) << "\"];";
        verifyEndl(true);
    }

    if (free_AST) {
        delete runner->function_name;
        delete runner;
    }

}

void traversal::traversal_Variable_Acess(ast::AST_Node_Variable_Acess* runner, int print_graphviz, int free_AST, int produce_MIPS) {

    if (runner == nullptr) return;

    if (runner->node_number == -1) {
        runner->node_number = node_counter++;
    }

    if (runner->loop_matriz) {
        runner->loop_matriz->parent = runner;
    }

    traversal::traversal_Loop_Matrix(runner->loop_matriz, print_graphviz, free_AST, produce_MIPS);

    if (produce_MIPS) {
        
        if(runner->loop_matriz == nullptr) {

            int is_global_var = 0;
            int is_local_var = 0;

            for (auto var_global: variaveis_globais) {

                if (*(var_global->name) == *(runner->variable_name)) {

                    int reg1 = helpers::return_first_unused_register();

                    if (reg1 == -1) {
                        std::cout << "Error: No registers available!";
                        verifyEndl(true);
                        exit(1);
                    }

                    runner->mapped_to_register = reg1;
                    is_global_var = 1;
                    ((ast::AST_Node_Expression*)runner->parent)->mapped_to_register = reg1;
                    std::cout << "\tlw " << register_names[reg1] << ", " << *(var_global->name);
                    verifyEndl(true);

                    memory_acess.push_back(new ast::AST_Memory_Access(*(runner->variable_name), reg1, 0, false));

                    break;
                }

            }

            if (!is_global_var) {

                for (int i = 0; i < current_function->variables.size(); i++) {

                    if (*(current_function->variables[i]->name) == *(runner->variable_name)) {

                        runner->mapped_to_register = $S0 + i;

                        ((ast::AST_Node_Expression*)(runner->parent))->mapped_to_register = $S0 + i;

                        is_local_var = 1;

                        used_registers[$S0 + i] = 1;

                        break;
                    }

                }

            } 

            if (!is_local_var && !is_global_var) {

                for (int i = 0; i < current_function->parameters.size(); i++) {

                    if (*(current_function->parameters[i]->name) == *(runner->variable_name)) {

                        runner->mapped_to_register = $A0 + i;

                        ((ast::AST_Node_Expression*)(runner->parent))->mapped_to_register = $A0 + i;

                        break;
                    }

                }

            }

        } else {

            int offset = runner->loop_matriz->expressao->mapped_to_register;

            if (helpers::return_register_type(offset) != helpers::T) {
                int temp = helpers::return_first_unused_register();
                std::cout << "\tadd " << register_names[temp] << ", " << register_names[offset] << ", $zero";
                verifyEndl(true);
                offset = temp;
            }

            int is_global_var = 0;
            int is_local_var = 0;

            for (auto var_global: variaveis_globais) {

                if (*(var_global->name) == *(runner->variable_name)) {

                    if (var_global->dimensions.size() == 0) {
                        std::cout << "Trying to access the global variable " << *(var_global->name) << " as an array";
                        verifyEndl(true);
                        exit(1);
                    }

                    int reg1 = helpers::return_first_unused_register();

                    if (reg1 == -1) {
                        std::cout << "Error: No registers available!";
                        verifyEndl(true);
                        exit(1);
                    }

                    runner->mapped_to_register = reg1;
                    is_global_var = 1;

                    ((ast::AST_Node_Expression*)(runner->parent))->mapped_to_register = reg1;

                    if (var_global->type->find("int") != std::string::npos) {
                        std::cout << "\tsll " << register_names[offset] << ", " << register_names[offset] << ", 2";
                        verifyEndl(true);
                    }

                    memory_acess.push_back(new ast::AST_Memory_Access(*(runner->variable_name), reg1, offset, true));

                    break;
                }

            }

            if (!is_global_var) {

                for (int i = 0; i < current_function->variables.size(); i++) {

                    if (*(current_function->variables[i]->name) == *(runner->variable_name)) {

                        if (current_function->variables[i]->dimensions.size() == 0) {
                            std::cout << "Trying to access the local variable " << *(current_function->variables[i]->name) << " as an array";
                            verifyEndl(true);
                            exit(1);
                        }

                        runner->mapped_to_register = $S0 + i;

                        used_registers[$S0 + i] = 1;

                        ((ast::AST_Node_Expression*)(runner->parent))->mapped_to_register = $S0 + i;

                        is_local_var = 1;

                        if (current_function->variables[i]->type->find("int") != std::string::npos) {
                            std::cout << "\tsll " << register_names[offset] << ", " << register_names[offset] << ", 2";
                            verifyEndl(true);
                        }

                        memory_acess.push_back(new ast::AST_Memory_Access(*(runner->variable_name), $S0 + i, offset, true));

                        break;
                    }

                }

            } 

            if (!is_local_var && !is_global_var) {

                for (int i = 0; i < current_function->parameters.size(); i++) {

                    if (*(current_function->parameters[i]->name) == *(runner->variable_name)) {

                        std::cout << "Trying to access the parameter " << *(current_function->parameters[i]->name) << " as an array";
                        verifyEndl(true);
                        exit(1);

                        runner->mapped_to_register = $A0 + i;

                        ((ast::AST_Node_Expression*)(runner->parent))->mapped_to_register = $A0 + i;

                        break;
                    }

                }

            }

        }
    }  

    if (print_graphviz) {
        if (runner->loop_matriz != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->loop_matriz->node_number << ";";
            verifyEndl(true);
        }

        dotfile << "\t" << runner->node_number << " [shape = box, label = \"Acesso Variavel " << *(runner->variable_name) << "\"];";
        verifyEndl(true);
    }

    if (free_AST) {
        delete runner->variable_name;
        delete runner;
    }
}

void traversal::traversal_Loop_Expressions(ast::AST_Node_Loop_Expressions* runner, int print_graphviz, int free_AST, int produce_MIPS) {
    if (runner == nullptr) return;

    if (runner->node_number == -1) {
        runner->node_number = node_counter++;
    }

    if (runner->expressao) {
        runner->expressao->parent = runner;
    }

    if (runner->loop_expressoes_temporario) {
        runner->loop_expressoes_temporario->parent = runner;
    }

    if (produce_MIPS) mips::ops::save_context_on_stack();

    traversal::traversal_Expression(runner->expressao, print_graphviz, free_AST, produce_MIPS);

    traversal::traversal_Temp_Loop_Expressions(runner->loop_expressoes_temporario, print_graphviz, free_AST, produce_MIPS);

    if (produce_MIPS) {

        if (runner->expressao != nullptr) {
            
            if (runner->expressao->mapped_to_register == -1) {
                std::cout << "Error: Trying to pass an expression that is not mapped to a register to a function call on " << register_names[$A0];
                verifyEndl(true);
                exit(1);
            }

            if (runner->expressao->variable_acess) {

                if(!memory_acess.empty()) {

                    if (memory_acess[0]->name == *(runner->expressao->variable_acess->variable_name) && memory_acess[0]->is_vector) {

                        std::cout << "\tla " << register_names[$A0] << ", " << *(runner->expressao->variable_acess->variable_name);
                        verifyEndl(true);
                        std::cout << "\tadd " << register_names[$A0] << ", " << register_names[$A0] << "," << register_names[memory_acess[0]->offset];
                        verifyEndl(true);
                        std::cout << "\tlw " << register_names[$A0] << ", " << *(runner->expressao->variable_acess->variable_name) << "(" << register_names[memory_acess[0]->offset] << ")";
                        verifyEndl(true);                        
                        delete memory_acess[0];
                        memory_acess.erase(memory_acess.begin());
                    }

                } else {
                    std::cout << "\tadd " << register_names[$A0] << ", " << register_names[runner->expressao->mapped_to_register] << ", $zero";
                    verifyEndl(true);    
                }
                
            } else {
                std::cout << "\tadd " << register_names[$A0] << ", " << register_names[runner->expressao->mapped_to_register] << ", $zero";
                verifyEndl(true);
            }

            if (helpers::return_register_type(runner->expressao->mapped_to_register) == helpers::T) helpers::free_register(runner->expressao->mapped_to_register);

            if (runner->loop_expressoes_temporario != nullptr) {

                if (runner->loop_expressoes_temporario->expressao->mapped_to_register == -1) {
                    std::cout << "Error: Trying to pass an expression that is not mapped to a register to a function call on " << register_names[$A1];
                    verifyEndl(true);
                    exit(1);
                }

                std::cout << "\tadd " << register_names[$A1] << ", " << register_names[runner->loop_expressoes_temporario->expressao->mapped_to_register] << ", $zero";
                verifyEndl(true);

                if (helpers::return_register_type(runner->expressao->mapped_to_register) == helpers::T) helpers::free_register(runner->loop_expressoes_temporario->expressao->mapped_to_register);

                if (runner->loop_expressoes_temporario->loop_expressoes_temporario != nullptr) {

                    if (runner->loop_expressoes_temporario->loop_expressoes_temporario->expressao->mapped_to_register == -1) {
                        std::cout << "Error: Trying to pass an expression that is not mapped to a register to a function call on " << register_names[$A2];
                        verifyEndl(true);
                        exit(1);
                    }

                    std::cout << "\tadd " << register_names[$A2] << ", " << register_names[runner->loop_expressoes_temporario->loop_expressoes_temporario->expressao->mapped_to_register] << ", $zero";
                    verifyEndl(true);

                    if (helpers::return_register_type(runner->expressao->mapped_to_register) == helpers::T) helpers::free_register(runner->loop_expressoes_temporario->loop_expressoes_temporario->expressao->mapped_to_register);

                }
            }

                
        }
    }

    if (print_graphviz) {
        if (runner->expressao != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->expressao->node_number << ";";
            verifyEndl(true);
        }

        if (runner->loop_expressoes_temporario != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->loop_expressoes_temporario->node_number << ";";
            verifyEndl(true);
        }

        dotfile << "\t" << runner->node_number << " [shape = box, label = \""
                << "Loop Expressoes"
                << "\"];";
                verifyEndl(true);
    }

    if (free_AST) {
        delete runner;
    }
}

void traversal::traversal_Temp_Loop_Expressions(ast::AST_Node_Temp_Loop_Expressions* runner, int print_graphviz, int free_AST, int produce_MIPS) {
    if (runner == nullptr) return;

    if (runner->node_number == -1) {
        runner->node_number = node_counter++;
    }

    if (runner->expressao) {
        runner->expressao->parent = runner;
    }

    if (runner->loop_expressoes_temporario) {
        runner->loop_expressoes_temporario->parent = runner;
    }

    traversal::traversal_Expression(runner->expressao, print_graphviz, free_AST, produce_MIPS);

    traversal::traversal_Temp_Loop_Expressions(runner->loop_expressoes_temporario, print_graphviz, free_AST, produce_MIPS);

    if (print_graphviz) {
        if (runner->expressao != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->expressao->node_number << ";";
            verifyEndl(true);
        }

        if (runner->loop_expressoes_temporario != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->loop_expressoes_temporario->node_number << ";";
            verifyEndl(true);
        }

        dotfile << "\t" << runner->node_number << " [shape = box, label = \""
                << "Loop Expressoes Temporario"
                << "\"];";
                verifyEndl(true);
    }

    if (free_AST) {
        delete runner;
    }
}

void traversal::traversal_Loop_Matrix(ast::AST_Node_Loop_Matrix* runner, int print_graphviz, int free_AST, int produce_MIPS) {
    if (runner == nullptr) return;

    if (runner->node_number == -1) {
        runner->node_number = node_counter++;
    }

    if (runner->expressao) {
        runner->expressao->parent = runner;
    }

    if (runner->loop_matriz) {
        runner->loop_matriz->parent = runner;
    }

    traversal::traversal_Expression(runner->expressao, print_graphviz, free_AST, produce_MIPS);
    traversal::traversal_Loop_Matrix(runner->loop_matriz, print_graphviz, free_AST, produce_MIPS);

    if (print_graphviz) {
        if (runner->expressao != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->expressao->node_number << ";";
            verifyEndl(true);
        }

        if (runner->loop_matriz != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->loop_matriz->node_number << ";";
            verifyEndl(true);
        }

        dotfile << "\t" << runner->node_number << " [shape = box, label = \""
                << "Loop Matriz"
                << "\"];";
                verifyEndl(true);
    }

    if (free_AST) {
        delete runner;
    }

}

void traversal::traversal_Stop_Condition(ast::AST_Node_Stop_Con* runner, int print_graphviz, int free_AST, int produce_MIPS) {
    if (runner == nullptr) return;

    if (runner->node_number == -1) {
        runner->node_number = node_counter++;
    }

    if (runner->expressao) {
        runner->expressao->parent = runner;
    }

    traversal::traversal_Expression(runner->expressao, print_graphviz, free_AST, produce_MIPS);

    if (produce_MIPS) {

        if (!runner->expressao) {
            runner->mapped_to_register = -1;
        } else {
            runner->mapped_to_register = runner->expressao->mapped_to_register;
        }

    }

    if (print_graphviz) {
        if (runner->expressao != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->expressao->node_number << ";";
            verifyEndl(true);
        }

        dotfile << "\t" << runner->node_number << " [shape = box, label = \""
                << "Condicao Parada"
                << "\"];";
                verifyEndl(true);
    }

    if (free_AST) {
        delete runner;
    }

}

void traversal::traversal_Values_Fit(ast::AST_Node_Values_Fit* runner, int print_graphviz, int free_AST, int produce_MIPS) {
    if (runner == nullptr) return;

    if (runner->node_number == -1) {
        runner->node_number = node_counter++;
    }

    if (runner->expressao) {
        runner->expressao->parent = runner;
    }

    traversal::traversal_Expression(runner->expressao, print_graphviz, free_AST, produce_MIPS);

    if (print_graphviz) {
        if (runner->expressao != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->expressao->node_number << ";";
            verifyEndl(true);
        }

        dotfile << "\t" << runner->node_number << " [shape = box, label = \""
                << "Ajuste Valores"
                << "\"];";
                verifyEndl(true);
    }

    if (free_AST) {
        delete runner;
    }

}

void traversal::traversal_Init_For(ast::AST_Node_Init_For* runner, int print_graphviz, int free_AST, int produce_MIPS) {
    if (runner == nullptr) return;

    if (runner->node_number == -1) {
        runner->node_number = node_counter++;
    }

    if (runner->expressao) {
        runner->expressao->parent = runner;
    }

    traversal::traversal_Expression(runner->expressao, print_graphviz, free_AST, produce_MIPS);

    if (print_graphviz) {
        if (runner->expressao != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->expressao->node_number << ";";
            verifyEndl(true);
        }

        dotfile << "\t" << runner->node_number << " [shape = box, label = \""
                << "Inicializacao For"
                << "\"];";
                verifyEndl(true);
    }

    if (free_AST) {
        delete runner;
    }
}

void traversal::traversal_Temp_Expressions_Print(ast::AST_Node_Temp_Expressions_Print* runner, int print_graphviz, int free_AST, int produce_MIPS) {
    if (runner == nullptr) return;

    if (runner->node_number == -1) {
        runner->node_number = node_counter++;
    }

    if (runner->expressao) {
        runner->expressao->parent = runner;
    }

    traversal::traversal_Expression(runner->expressao, print_graphviz, free_AST, produce_MIPS);
    traversal::traversal_Temp_Expressions_Print(runner->expressoes_printf_temporario, print_graphviz, free_AST, produce_MIPS);

    if (print_graphviz) {
        if (runner->expressao != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->expressao->node_number << ";";
            verifyEndl(true);
        }

        if (runner->expressoes_printf_temporario != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->expressoes_printf_temporario->node_number << ";";
            verifyEndl(true);
        }

        dotfile << "\t" << runner->node_number << " [shape = box, label = \""
                << "Expressoes Printf Temporario"
                << "\"];" ;
                verifyEndl(true);
    }

    if (free_AST) {
        delete runner;
    }
}

void traversal::traversal_Expressions_Print(ast::AST_Node_Expressions_Print* runner, int print_graphviz, int free_AST, int produce_MIPS) {
    if (runner == nullptr) return;

    if (runner->node_number == -1) {
        runner->node_number = node_counter++;
    }

    if (runner->expressao) {
        runner->expressao->parent = runner;
    }

    if(runner->expressoes_printf_temporario) {
        runner->expressoes_printf_temporario->parent = runner;
    }

    traversal::traversal_Expression(runner->expressao, print_graphviz, free_AST, produce_MIPS);
    traversal::traversal_Temp_Expressions_Print(runner->expressoes_printf_temporario, print_graphviz, free_AST, produce_MIPS);

    if (print_graphviz) {
        if (runner->expressao != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->expressao->node_number << ";" ;
            verifyEndl(true);
        }

        if (runner->expressoes_printf_temporario != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->expressoes_printf_temporario->node_number << ";" ;
            verifyEndl(true);
        }

        dotfile << "\t" << runner->node_number << " [shape = box, label = \""
                << "Expressoes Printf"
                << "\"];" ;
                verifyEndl(true);
    }

    if (free_AST) {
        delete runner;
    }
}

void traversal::traversal_Var_Adress(ast::AST_Node_Var_Adress* runner, int print_graphviz, int free_AST, int produce_MIPS) {
    if (runner == nullptr) return;

    if (runner->node_number == -1) {
        runner->node_number = node_counter++;
    }

    if (print_graphviz) {
        dotfile << "\t" << runner->node_number << " [shape = box, label = \"" << *(runner->identifier) << "\"];" ;
        verifyEndl(true);
    }

    if (free_AST) {
        delete runner->identifier;
        delete runner;
    }
}

void traversal::traversal_Return_Command(ast::AST_Node_Return* runner, int print_graphviz, int free_AST, int produce_MIPS) {
    if (runner == nullptr) return;

    if (runner->node_number == -1) {
        runner->node_number = node_counter++;
    }

    if (runner->StopCondition) {
        runner->StopCondition->parent = runner;
    }

    traversal::traversal_Stop_Condition(runner->StopCondition, print_graphviz, free_AST, produce_MIPS);

    if (produce_MIPS) {
        if (runner->StopCondition) {
            if(runner->StopCondition->expressao != nullptr) {
                std::cout << "\tadd $v0, " << register_names[runner->StopCondition->expressao->mapped_to_register] << ", $zero" ;
                verifyEndl(true);
                used_registers[$V0] = true;
                helpers::free_register(runner->StopCondition->expressao->mapped_to_register);
                
                if (*(current_function->function_name) != "main") {
                    std::cout << "\tjr $ra" ;
                    verifyEndl(true);
                }
            }
        } else {
            if (*(current_function->function_name) != "main") {
                std::cout << "\tjr $ra" ;
                verifyEndl(true);
            }
        }
    }

    if (print_graphviz) {
        if (runner->StopCondition != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->StopCondition->node_number << ";" ;
            verifyEndl(true);
        }

        dotfile << "\t" << runner->node_number << " [shape = box, label = \""
                << "Command Return"
                << "\"];" ;
                verifyEndl(true);
    }

    if (free_AST) {
        delete runner;
    }

}

void traversal::traversal_Exit_Command(ast::AST_Node_Exit* runner, int print_graphviz, int free_AST, int produce_MIPS) {
    if (runner == nullptr) return;

    if (runner->node_number == -1) {
        runner->node_number = node_counter++;
    }

    if (runner->expressao) {
        runner->expressao->parent = runner;
    }

    traversal::traversal_Expression(runner->expressao, print_graphviz, free_AST, produce_MIPS);

    if (produce_MIPS) {
        std::cout << "\tadd $a0, $zero, " << register_names[runner->expressao->mapped_to_register] ;
        verifyEndl(true);
        helpers::free_register(runner->expressao->mapped_to_register);
        std::cout << "\taddi $v0, $zero, 17" ;
        verifyEndl(true); 
        std::cout << "\tsyscall" ;
        verifyEndl(true);
    }

    if (print_graphviz) {
        if (runner->expressao != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->expressao->node_number << ";" ;
            verifyEndl(true);
        }

        dotfile << "\t" << runner->node_number << " [shape = box, label = \""
                << "Command Exit"
                << "\"];" ;
                verifyEndl(true);
    }

    if (free_AST) {
        delete runner;
    }
}

void traversal::traversal_Scanf_Command(ast::AST_Node_Scanf* runner, int print_graphviz, int free_AST, int produce_MIPS) {
    if (runner == nullptr) return;

    if (runner->node_number == -1) {
        runner->node_number = node_counter++;
    }

    if (runner->endereco_var) {
        runner->endereco_var->parent = runner;
    }

    traversal::traversal_Var_Adress(runner->endereco_var, print_graphviz, free_AST, produce_MIPS);

    if (produce_MIPS) {

        int is_const = 0;
        int is_global_var = 0;
        int is_local_var = 0;

        for (auto constante : constantes) {

            if (runner->endereco_var->identifier == constante->name) {

                is_const = 1;

                std::cout << "Error: Cannot read constant" ;
                verifyEndl(true);
                exit(1);

                break;
            }
        }

        if (!is_const) {
            for (auto var_global : variaveis_globais) {

                if (*(runner->endereco_var->identifier) == *(var_global->name)) {

                    is_global_var = 1;

                    std::string* var_type = var_global->type;

                    if (var_type->find("int") != std::string::npos) {

                        if (var_type->find("*") != std::string::npos) {
                            std::cout << "Error: Cannot read int pointer variable" ;
                            verifyEndl(true);
                            exit(1);
                        } else if (var_type->find("[") != std::string::npos) {
                            std::cout << "Error: Cannot read int array variable" ;
                            verifyEndl(true);
                            exit(1);
                        }
                        
                        int reg1 = helpers::return_first_unused_register();

                        if (reg1 == -1) {
                            std::cout << "Error: No available registers" ;
                            verifyEndl(true);
                            exit(1);
                        }

                        std::cout << "\tla " << register_names[reg1] << ", " << *(var_global->name) ;
                        verifyEndl(true);
                        
                        mips::ops::read_int();

                        std::cout << "\tsw $v0, 0(" << register_names[reg1] << ")" ;
                        verifyEndl(true);

                        helpers::free_register(reg1);

                    } else if (var_type->find("char") != std::string::npos) {
                        
                        if (var_type->find("*") != std::string::npos) {

                            std::cout << "Cannot read string" ;
                            verifyEndl(true);
                            exit(1);

                        } else {

                            int reg1 = helpers::return_first_unused_register();

                            if (reg1 == -1) {
                                std::cout << "Error: No available registers" ;
                                verifyEndl(true);
                                exit(1);
                            }

                            std::cout << "\tla " << register_names[reg1] << ", " << *(var_global->name) ;
                            verifyEndl(true);

                            mips::ops::read_char();

                            std::cout << "\tsw $v0, 0(" << register_names[reg1] << ")" ;
                            verifyEndl(true);

                            helpers::free_register(reg1);

                        }

                    } else if (var_type->find("void") != std::string::npos) {

                        std::cout << "Error: Cannot read void variable" ;
                        verifyEndl(true);
                        exit(1);
                        
                    }

                    break;
                }
            }
        }

        if (!is_const && !is_global_var) {

            for (int i = 0; i < current_function->variables.size(); i++) {

                if(*(current_function->variables[i]->name) == *(runner->endereco_var->identifier)) {
                        
                    is_local_var = 1;

                    std::string* var_type = current_function->variables[i]->type;

                    if (var_type->find("int") != std::string::npos) {

                        mips::ops::read_int();

                        std::cout << "\tadd $s" << i << ", $v0, $zero" ;
                        verifyEndl(true);

                    } else if (var_type->find("char") != std::string::npos) {
                        
                        if (var_type->find("*") != std::string::npos) {

                            std::cout << "Cannot read string" ;
                            verifyEndl(true);
                            exit(1);

                        } else {

                            int reg1 = helpers::return_first_unused_register();

                            mips::ops::read_char();

                            std::cout << "\tadd $s" << i << ", $v0, $zero" ;
                            verifyEndl(true);

                            helpers::free_register(reg1);

                        }

                    } else if (var_type->find("void") != std::string::npos) {

                        std::cout << "Error: Cannot read void variable" ;
                        verifyEndl(true);
                        exit(1);
                        
                    }

                    break;
                }

            }
        }

        if (!is_const && !is_global_var && !is_local_var) {
            std::cout << "Error: Variable " << *(runner->endereco_var->identifier) << " not declared" ;
            verifyEndl(true);
            exit(1);
        }

    }

    if (print_graphviz) {
        if (runner->endereco_var != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->endereco_var->node_number << ";" ;
            verifyEndl(true);
        }

        dotfile << "\t" << runner->node_number << " [shape = box, label = \""
                << "Command Scanf : "
                << runner->string->substr(1, runner->string->size() - 2)
                << "\"];" ;
                verifyEndl(true);
    }

    if (!runner->strings_added) {
        helpers::split_format_string(runner->string->substr(1, runner->string->size() - 2), runner->node_number);
        runner->strings_added = true;
    }

    if (free_AST) {
        delete runner->string;
        delete runner;
    }
}

void traversal::traversal_Printf_Command(ast::AST_Node_Printf* runner, int print_graphviz, int free_AST, int produce_MIPS) {
    if (runner == nullptr) return;

    if (runner->node_number == -1) {
        runner->node_number = node_counter++;
    }

    if (runner->expressoes_printf) {
        runner->expressoes_printf->parent = runner;
    }

    traversal::traversal_Expressions_Print(runner->expressoes_printf, print_graphviz, free_AST, produce_MIPS);

    if (produce_MIPS) {

        if (runner->expressoes_printf == nullptr) {

            std::cout << "\tla $a0, string_" << runner->node_number << "_0" ;
            verifyEndl(true);
            mips::ops::print_string();

        } else {

            int position = -1;

            for (int i = 0; i < node_strings.size(); i++) {
                if (node_strings[i]->node_number == runner->node_number) {
                    position = i;
                    break;
                }
            }

            if (position == -1) {
                std::cout << "Error: Could not find node number " << runner->node_number << " in node_strings" ;
                verifyEndl(true);
                exit(1);
            }

            ast::AST_Node_Expressions_Print* current_printf_expression = runner->expressoes_printf;

            ast::AST_Node_Temp_Expressions_Print* next_printf_expression = nullptr;

            for (int i = 0; i < node_strings[position]->strings.size(); i++) {

                std::string current_string = node_strings[position]->strings[i];

                if (current_string == "%d" || current_string == "%c") {

                    if (next_printf_expression == nullptr) {

                        if (current_printf_expression == nullptr) {
                            std::cout << "Error: Not enough expressions to print" ;
                            verifyEndl(true);
                            exit(1);
                        }

                        if (current_printf_expression->expressao->mapped_to_register == -1) {
                            std::cout << "Error: Expression not mapped to register" ;
                            verifyEndl(true);
                            exit(1);
                        }

                        if (current_printf_expression->expressao->mapped_to_register == $V0) {
                            std::cout << "\tlw $v0, 0($sp)" ;
                            verifyEndl(true);
                            std::cout << "\taddi $sp, $sp, 4" ;
                            verifyEndl(true);
                        }

                        if (!memory_acess.empty()) {
                            std::cout << "\tlw $a0, " << memory_acess[0]->name << "(" << register_names[memory_acess[0]->offset] << ")" ;
                            verifyEndl(true);
                            delete memory_acess[0];
                            memory_acess.pop_back();
                        } else {
                            std::cout << "\tadd $a0, " << register_names[current_printf_expression->expressao->mapped_to_register] << ", $zero" ;
                            verifyEndl(true);
                        }

                        if (helpers::return_register_type(current_printf_expression->expressao->mapped_to_register) == helpers::T) {
                            helpers::free_register(current_printf_expression->expressao->mapped_to_register);
                        }

                    } else {
                            
                        if (next_printf_expression == nullptr) {
                            std::cout << "Error: Not enough expressions to print" ;
                            verifyEndl(true);
                            exit(1);
                        }

                        if (next_printf_expression->expressao->mapped_to_register == -1) {
                            std::cout << "Error: Expression not mapped to register" ;
                            verifyEndl(true);
                            exit(1);
                        }

                        if (next_printf_expression->expressao->mapped_to_register == $V0) {
                            std::cout << "\tlw $v0, 0($sp)" ;
                            verifyEndl(true);
                            std::cout << "\taddi $sp, $sp, 4" ;
                            verifyEndl(true);
                        }

                        if (!memory_acess.empty()) {
                            std::cout << "\tlw $a0, " << memory_acess[0]->name << "(" << register_names[memory_acess[0]->offset] << ")" ;
                            verifyEndl(true);
                            memory_acess.pop_back();
                        } else {
                            std::cout << "\tadd $a0, " << register_names[next_printf_expression->expressao->mapped_to_register] << ", $zero" ;
                            verifyEndl(true);
                            
                        }

                        if (helpers::return_register_type(next_printf_expression->expressao->mapped_to_register) == helpers::T) {
                            helpers::free_register(next_printf_expression->expressao->mapped_to_register);
                        }


                    }

                    if (current_string == "%d") {
                        mips::ops::print_int();
                    } else if (current_string == "%c") {
                        mips::ops::print_char();
                    } else {
                        std::cout << "Error: Invalid format string" ;
                        verifyEndl(true);
                        exit(1);
                    }

                    if (next_printf_expression == nullptr) {
                        next_printf_expression = current_printf_expression->expressoes_printf_temporario;
                    } else {
                        next_printf_expression = next_printf_expression->expressoes_printf_temporario;
                    }

                } else {

                    std::cout << "\tla $a0, string_" << runner->node_number << "_" << i ;
                    verifyEndl(true);
                    mips::ops::print_string();
                }
                    

            }

        }

    }

    if (print_graphviz) {
        if (runner->expressoes_printf != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->expressoes_printf->node_number << ";" ;
            verifyEndl(true);
        }

        dotfile << "\t" << runner->node_number << " [shape = box, label = \""
                << "Command Printf \n"
                << runner->string->substr(1, runner->string->size() - 2)
                << "\"];" ;
                verifyEndl(true);
    }

    if(!runner->strings_added) {

        helpers::split_format_string(runner->string->substr(1, runner->string->size() - 2), runner->node_number);
        runner->strings_added = true;
    }

    if (free_AST) {
        delete runner->string;
        delete runner;
    }
}

void traversal::traversal_For_Command(ast::AST_Node_For* runner, int print_graphviz, int free_AST, int produce_MIPS) {
    if (runner == nullptr) return;

    if (runner->node_number == -1) {
        runner->node_number = node_counter++;
    }

    if (runner->inicializacao_for) {
        runner->inicializacao_for->parent = runner;
    }

    if (runner->condicao_parada) {
        runner->condicao_parada->parent = runner;
    }

    if (runner->ajuste_valores) {
        runner->ajuste_valores->parent = runner;
    }

    if (runner->lista_comandos) {
        runner->lista_comandos->parent = runner;
    }

    if (produce_MIPS) {
        std::cout << "for_" << runner->node_number << ":" ;
        verifyEndl(true);
    }

    traversal::traversal_Init_For(runner->inicializacao_for, print_graphviz, free_AST, produce_MIPS);

    if (produce_MIPS) {
        std::cout << "for_cond_" << runner->node_number << ":" ;
        verifyEndl(true);
    }

    traversal::traversal_Stop_Condition(runner->condicao_parada, print_graphviz, free_AST, produce_MIPS);

    if (produce_MIPS) {
        int reg1 = runner->condicao_parada->mapped_to_register;

        if (reg1 == -1) {
            std::cout << "Error: Condition not mapped to register" ;
            verifyEndl(true);
            exit(1);
        }

        std::cout << "\tbeq " << register_names[reg1] << ", $zero, for_end_" << runner->node_number ;
        verifyEndl(true);

        helpers::free_register(reg1);

    }

    traversal::traversal_Cons_List(runner->lista_comandos, print_graphviz, free_AST, produce_MIPS);

    traversal::traversal_Values_Fit(runner->ajuste_valores, print_graphviz, free_AST, produce_MIPS);


    if (produce_MIPS) {
        std::cout << "\tj for_cond_" << runner->node_number ;
        verifyEndl(true);
        std::cout << "for_end_" << runner->node_number << ":" ;
        verifyEndl(true);
    }

    if (print_graphviz) {
        if (runner->inicializacao_for != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->inicializacao_for->node_number << ";" ;
            verifyEndl(true);
        }

        if (runner->condicao_parada != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->condicao_parada->node_number << ";" ;
            verifyEndl(true);
        }

        if (runner->ajuste_valores != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->ajuste_valores->node_number << ";" ;
            verifyEndl(true);
        }

        if (runner->lista_comandos != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->lista_comandos->node_number << ";" ;
            verifyEndl(true);
        }

        dotfile << "\t" << runner->node_number << " [shape = box, label = \""
                << "Command For"
                << "\"];" ;
                verifyEndl(true);
    }

    if (free_AST) {
        delete runner;
    }
}

void traversal::traversal_While_Command(ast::AST_Node_While* runner, int print_graphviz, int free_AST, int produce_MIPS) {
    if (runner == nullptr) return;

    if (runner->node_number == -1) {
        runner->node_number = node_counter++;
    }

    if (runner->condicao_parada) {
        runner->condicao_parada->parent = runner;
    }

    if (runner->lista_comandos) {
        runner->lista_comandos->parent = runner;
    }

    if (produce_MIPS) { 
        std::cout << "while_test_" << runner->node_number << ":" ;
        verifyEndl(true);
    }

    traversal::traversal_Stop_Condition(runner->condicao_parada, print_graphviz, free_AST, produce_MIPS);

    if (produce_MIPS) {

        int reg1 = runner->condicao_parada->mapped_to_register;

        if (reg1 == -1) {
            std::cout << "ERROR: While condition not mapped to register" ;
            verifyEndl(true);
            exit(1);
        }

        std::cout << "\tbeq " << register_names[reg1] << ", $zero, " << "while_end_" << runner->node_number ;
        verifyEndl(true);

        if (helpers::return_register_type(reg1) == helpers::T) helpers::free_register(reg1);

    }

    if (produce_MIPS) {
        std::cout << "while_start_" << runner->node_number << ":" ;
        verifyEndl(true);
    }

    traversal::traversal_Cons_List(runner->lista_comandos, print_graphviz, free_AST, produce_MIPS);

    if (produce_MIPS) {
        std::cout << "\tj " << "while_test_" << runner->node_number ;
        verifyEndl(true);
    }

    if (produce_MIPS) {
        std::cout << "while_end_" << runner->node_number << ":" ;
        verifyEndl(true);
    }



    if (print_graphviz) {
        if (runner->condicao_parada != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->condicao_parada->node_number << ";" ;
            verifyEndl(true);
        }

        if (runner->lista_comandos != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->lista_comandos->node_number << ";" ;
            verifyEndl(true);
        }

        dotfile << "\t" << runner->node_number << " [shape = box, label = \""
                << "Command While"
                << "\"];" ;
                verifyEndl(true);
    }

    if (free_AST) {
        delete runner;
    }
}

void traversal::traversal_If_Command(ast::AST_Node_If* runner, int print_graphviz, int free_AST, int produce_MIPS) {
    if (runner == nullptr) return;

    if (runner->node_number == -1) {
        runner->node_number = node_counter++;
    }

    if (runner->condicao_parada) {
        runner->condicao_parada->parent = runner;
    }

    if (runner->lista_comandos_then) {
        runner->lista_comandos_then->parent = runner;
    }

    if (runner->lista_comandos_else) {
        runner->lista_comandos_else->parent = runner;
    }

    traversal::traversal_Stop_Condition(runner->condicao_parada, print_graphviz, free_AST, produce_MIPS);

    if (produce_MIPS) {

        int reg1 = runner->condicao_parada->mapped_to_register;

        if (reg1 == -1) {

            std::cout << "ERROR: IF condition not mapped to register" ;
            verifyEndl(true);
            exit(1);
        }

        std::cout << "\tbeq " << register_names[reg1] << ", $zero, else_" << runner->node_number ;
        verifyEndl(true);
        if (helpers::return_register_type(reg1) == helpers::T) helpers::free_register(reg1);

    }

    if (produce_MIPS) {
        std::cout << "then_" << runner->node_number << ":" ;
        verifyEndl(true);
    }

    traversal::traversal_Cons_List(runner->lista_comandos_then, print_graphviz, free_AST, produce_MIPS);

    if (produce_MIPS) {
        std::cout << "\tj endif_" << runner->node_number ;
        verifyEndl(true);
    }

    if (produce_MIPS) {
        std::cout << "else_" << runner->node_number << ":" ;
        verifyEndl(true);
    }

    traversal::traversal_Cons_List(runner->lista_comandos_else, print_graphviz, free_AST, produce_MIPS);

    if (produce_MIPS) {
        std::cout << "endif_" << runner->node_number << ":" ;
        verifyEndl(true);
    }

    if (print_graphviz) {
        if (runner->condicao_parada != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->condicao_parada->node_number << ";" ;
            verifyEndl(true);
        }

        if (runner->lista_comandos_then != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->lista_comandos_then->node_number << ";" ;
            verifyEndl(true);
        }

        if (runner->lista_comandos_else != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->lista_comandos_else->node_number << ";" ;
            verifyEndl(true);
        }

        dotfile << "\t" << runner->node_number << " [shape = box, label = \""
                << "Command If"
                << "\"];" ;
                verifyEndl(true);
    }

    if (free_AST) {
        delete runner;
    }
}

void traversal::traversal_Do_While_Command(ast::AST_Node_Do_While* runner, int print_graphviz, int free_AST, int produce_MIPS) {
    if (!runner) return;

    if (runner->node_number == -1) {
        runner->node_number = node_counter++;
    }

    if (runner->condicao_parada) {
        runner->condicao_parada->parent = runner;
    }

    if (runner->lista_comandos) {
        runner->lista_comandos->parent = runner;
    }

    if (produce_MIPS) {

        std::cout << "do_while_start_" << runner->node_number << ":" ;
        verifyEndl(true);

    }

    traversal::traversal_Cons_List(runner->lista_comandos, print_graphviz, free_AST, produce_MIPS);

    if (produce_MIPS) {

        std::cout << "do_while_test_" << runner->node_number << ":" ;
        verifyEndl(true);

    }

    traversal::traversal_Stop_Condition(runner->condicao_parada, print_graphviz, free_AST, produce_MIPS);

    if (produce_MIPS) {

        int reg1 = runner->condicao_parada->mapped_to_register;

        if (reg1 == -1) {

            std::cout << "ERROR: DO WHILE condition not mapped to register" ;
            verifyEndl(true);
            exit(1);
        }

        std::cout << "\tbeq " << register_names[reg1] << ", $zero, do_while_end_" << runner->node_number ;
        verifyEndl(true);
        if (helpers::return_register_type(reg1) == helpers::T) helpers::free_register(reg1);
        std::cout << "\tj do_while_start_" << runner->node_number ;
        verifyEndl(true);
        std::cout << "do_while_end_" << runner->node_number << ":" ;
        verifyEndl(true);

    }

    if (print_graphviz) {
        if (runner->condicao_parada != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->condicao_parada->node_number << ";" ;
            verifyEndl(true);
        }

        if (runner->lista_comandos != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->lista_comandos->node_number << ";" ;
            verifyEndl(true);
        }

        dotfile << "\t" << runner->node_number << " [shape = box, label = \""
                << "Command Do While"
                << "\"];" ;
                verifyEndl(true);
    }

    if (free_AST) {
        delete runner;
    }
}

void traversal::traversal_Command(ast::AST_Node_Command* runner, int print_graphviz, int free_AST, int produce_MIPS) {
    if (runner == nullptr) return;

    if (runner->node_number == -1) {
        runner->node_number = node_counter++;
    }

    if (runner->expressao) {
        runner->expressao->parent = runner;
    }

    if (runner->comando_do_while) {
        runner->comando_do_while->parent = runner;
    }

    if (runner->comando_for) {
        runner->comando_for->parent = runner;
    }

    if (runner->comando_if) {
        runner->comando_if->parent = runner;
    }

    if (runner->comando_while) {
        runner->comando_while->parent = runner;
    }

    if (runner->comando_printf) {
        runner->comando_printf->parent = runner;
    }

    if (runner->comando_scanf) {
        runner->comando_scanf->parent = runner;
    }

    if (runner->comando_exit) {
        runner->comando_exit->parent = runner;
    }

    if (runner->comando_return) {
        runner->comando_return->parent = runner;
    }

    traversal::traversal_Expression(runner->expressao, print_graphviz, free_AST, produce_MIPS);
    traversal::traversal_Do_While_Command(runner->comando_do_while, print_graphviz, free_AST, produce_MIPS);
    traversal::traversal_If_Command(runner->comando_if, print_graphviz, free_AST, produce_MIPS);
    traversal::traversal_While_Command(runner->comando_while, print_graphviz, free_AST, produce_MIPS);
    traversal::traversal_For_Command(runner->comando_for, print_graphviz, free_AST, produce_MIPS);
    traversal::traversal_Printf_Command(runner->comando_printf, print_graphviz, free_AST, produce_MIPS);
    traversal::traversal_Scanf_Command(runner->comando_scanf, print_graphviz, free_AST, produce_MIPS);
    traversal::traversal_Exit_Command(runner->comando_exit, print_graphviz, free_AST, produce_MIPS);
    traversal::traversal_Return_Command(runner->comando_return, print_graphviz, free_AST, produce_MIPS);

    if (print_graphviz) {
        if (runner->expressao != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->expressao->node_number << ";" ;
            verifyEndl(true);
        }

        if (runner->comando_do_while != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->comando_do_while->node_number << ";" ;
            verifyEndl(true);
        }

        if (runner->comando_if != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->comando_if->node_number << ";" ;
            verifyEndl(true);
        }

        if (runner->comando_while != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->comando_while->node_number << ";" ;
            verifyEndl(true);
        }

        if (runner->comando_for != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->comando_for->node_number << ";" ;
            verifyEndl(true);
        }

        if (runner->comando_printf != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->comando_printf->node_number << ";" ;
            verifyEndl(true);
        }

        if (runner->comando_scanf != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->comando_scanf->node_number << ";" ;
            verifyEndl(true);
        }

        if (runner->comando_exit != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->comando_exit->node_number << ";" ;
            verifyEndl(true);
        }

        if (runner->comando_return != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->comando_return->node_number << ";" ;
            verifyEndl(true);
        }

        dotfile << "\t" << runner->node_number << " [shape = box, label = \""
                << "Command"
                << "\"];" ;
                verifyEndl(true);
    }

    if (free_AST) {
        delete runner;
    }
}

void traversal::traversal_Temp_Cons_List(ast::AST_Node_Temp_Coms_List* runner, int print_graphviz, int free_AST, int produce_MIPS) {
    if (runner == nullptr) return;

    if (runner->node_number == -1) {
        runner->node_number = node_counter++;
    }

    if (runner->comando) {
        runner->comando->parent = runner;
    }

    if (runner->lista_comandos_temporario) {
        runner->lista_comandos_temporario->parent = runner;
    }

    traversal::traversal_Command(runner->comando, print_graphviz, free_AST, produce_MIPS);
    traversal::traversal_Temp_Cons_List(runner->lista_comandos_temporario, print_graphviz, free_AST, produce_MIPS);

    if (print_graphviz) {
        dotfile << "\t" << runner->node_number << " -> " << runner->comando->node_number << ";" ;
        verifyEndl(true);

        if (runner->lista_comandos_temporario != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->lista_comandos_temporario->node_number << ";" ;
            verifyEndl(true);
        }

        dotfile << "\t" << runner->node_number << " [shape = box, label = \""
                << "Lista Comandos Temporario"
                << "\"];" ;
                verifyEndl(true);
    }

    if (free_AST) {
        delete runner;
    }
}

void traversal::traversal_Cons_List(ast::AST_Node_Coms_List* runner, int print_graphviz, int free_AST, int produce_MIPS) {
    if (runner == nullptr) return;

    if (runner->node_number == -1) {
        runner->node_number = node_counter++;
    }

    if (runner->comando) {
        runner->comando->parent = runner;
    }

    if (runner->lista_comandos_temporario) {
        runner->lista_comandos_temporario->parent = runner;
    }

    traversal::traversal_Command(runner->comando, print_graphviz, free_AST, produce_MIPS);
    traversal::traversal_Temp_Cons_List(runner->lista_comandos_temporario, print_graphviz, free_AST, produce_MIPS);

    if (print_graphviz) {
        dotfile << "\t" << runner->node_number << " -> " << runner->comando->node_number << ";" ;
        verifyEndl(true);

        if (runner->lista_comandos_temporario != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->lista_comandos_temporario->node_number << ";" ;
            verifyEndl(true);
        }

        dotfile << "\t" << runner->node_number << " [shape = box, label = \""
                << "Lista Comandos"
                << "\"];" ;
                verifyEndl(true);
    }

    if (free_AST) {
        delete runner;
    }
}

void traversal::traversal_Function_Body(ast::AST_Node_Func_Body* runner, int print_graphviz, int free_AST, int produce_MIPS) {
    if (runner == nullptr) return;

    if (runner->node_number == -1) {
        runner->node_number = node_counter++;
    }

    if (runner->lista_comandos) {
        runner->lista_comandos->parent = runner;
    }

    traversal::traversal_Cons_List(runner->lista_comandos, print_graphviz, free_AST, produce_MIPS);

    if (print_graphviz) {
        if (runner->lista_comandos != nullptr) {
            dotfile << "\t" << runner->node_number << " -> " << runner->lista_comandos->node_number << ";" ;
            verifyEndl(true);
        }

        dotfile << "\t" << runner->node_number << " [shape = box, label = \""
                << "Corpo Funcao\n"
                << *(runner->function_name)
                << "\"];" ;
                verifyEndl(true);
    }

    if (free_AST) {
        delete runner;
    }
}