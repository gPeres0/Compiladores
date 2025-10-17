#include "ast.hh"
#include "mips.hh"

namespace mips {

    void print_data_segment(std::vector<ast::AST_Constant*> consts, std::vector<ast::AST_Variable*> global_vars, std::vector<ast::AST_Node_Strings*> node_strings) {
        std::cout << ".data" << std::endl;

        for (auto v : global_vars) {
            std::string type = *(v->type);

            if (type.find("*") != std::string::npos) {
                std::cout << "\t" << *(v->name) << ": .word 0" << std::endl;
            } else {
                int bytes = 0;
                int multiplier = calculate_bytes_multipler(v->dimensions);

                if (type.find("int") != std::string::npos) {
                    bytes = 4 * multiplier;
                } else if (type.find("char") != std::string::npos) {
                    bytes = 1 * multiplier;
                } else if (type.find("void") != std::string::npos) {
                    std::cout << "Error: Incomplete void type not allowed" << std::endl;
                } else {
                    std::cout << "Error: Unknown type " << type << std::endl;
                }

                if (multiplier != 1) {
                    std::cout << "\t" << *(v->name) << ": .space " << bytes << std::endl;
                } else {
                    std::cout << "\t" << *(v->name) << ": .word 0" << std::endl;
                }
            }

        }

        for (auto c : consts) {
            std::cout << "\t" << *(c->name) << ": .word " << c->value << std::endl;
        }

        for (auto s : node_strings) {
            for (int i = 0; i < s->strings.size(); i++) {
                if(s->strings[i][0] != '%') {
                    std::cout << "\tstring_" << s->node_number << "_" <<  i << ": .asciiz \"" << s->strings[i] << "\"" << std::endl;
                }
            }
        }

        std::cout << ".text" << std::endl;
    }

    int calculate_bytes_multipler(std::vector<int> dimensions) {
        if (dimensions.size() == 0) {
            return 1;
        }
        int multiplier = 1;
        for (auto d : dimensions) {
            multiplier *= d;
        }

        return multiplier;
    }

    namespace ops {
        void exit() {
            std::cout << "\taddi $v0, $zero, 10" << std::endl;
            std::cout << "\tsyscall" << std::endl;
        }
        void print_int() {
            std::cout << "\taddi $v0, $zero, 1" << std::endl;
            std::cout << "\tsyscall" << std::endl;
        }
        void print_char() {
            std::cout << "\taddi $v0, $zero, 11" << std::endl;
            std::cout << "\tsyscall" << std::endl;
        }
        void print_string() {
            std::cout << "\taddi $v0, $zero, 4" << std::endl;
            std::cout << "\tsyscall" << std::endl;
        }
        void read_int() {
            std::cout << "\taddi $v0, $zero, 5" << std::endl;
            std::cout << "\tsyscall" << std::endl;
        }
        void read_char() {
            std::cout << "\taddi $v0, $zero, 12" << std::endl;
            std::cout << "\tsyscall" << std::endl;
        }
        void read_string() {
            std::cout << "\taddi $v0, $zero, 8" << std::endl;
            std::cout << "\tsyscall" << std::endl;
        }
        void save_context_on_stack() {
            std::cout << std::endl;
            std::cout << "\taddiu $sp, $sp, -52" << std::endl;
            std::cout << "\tsw $a0, 0($sp)" << std::endl;
            std::cout << "\tsw $a1, 4($sp)" << std::endl;
            std::cout << "\tsw $a2, 8($sp)" << std::endl;
            std::cout << "\tsw $a3, 12($sp)" << std::endl;
            std::cout << "\tsw $s0, 16($sp)" << std::endl;
            std::cout << "\tsw $s1, 20($sp)" << std::endl;
            std::cout << "\tsw $s2, 24($sp)" << std::endl;
            std::cout << "\tsw $s3, 28($sp)" << std::endl;
            std::cout << "\tsw $s4, 32($sp)" << std::endl;
            std::cout << "\tsw $s5, 36($sp)" << std::endl;
            std::cout << "\tsw $s6, 40($sp)" << std::endl;
            std::cout << "\tsw $s7, 44($sp)" << std::endl;
            std::cout << "\tsw $ra, 48($sp)" << std::endl;
        }
        void load_context_from_stack() {
            std::cout << std::endl;
            std::cout << "\tlw $a0, 0($sp)" << std::endl;
            std::cout << "\tlw $a1, 4($sp)" << std::endl;
            std::cout << "\tlw $a2, 8($sp)" << std::endl;
            std::cout << "\tlw $a3, 12($sp)" << std::endl;
            std::cout << "\tlw $s0, 16($sp)" << std::endl;
            std::cout << "\tlw $s1, 20($sp)" << std::endl;
            std::cout << "\tlw $s2, 24($sp)" << std::endl;
            std::cout << "\tlw $s3, 28($sp)" << std::endl;
            std::cout << "\tlw $s4, 32($sp)" << std::endl;
            std::cout << "\tlw $s5, 36($sp)" << std::endl;
            std::cout << "\tlw $s6, 40($sp)" << std::endl;
            std::cout << "\tlw $s7, 44($sp)" << std::endl;
            std::cout << "\tlw $ra, 48($sp)" << std::endl;
            std::cout << "\taddiu $sp, $sp, 52" << std::endl;
        }
    };
}