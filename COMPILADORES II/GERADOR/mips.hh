#ifndef MIPS_HH
#define MIPS_HH

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "ast.hh"

namespace mips {
    void print_data_segment(std::vector<ast::AST_Constant*> consts, std::vector<ast::AST_Variable*> global_vars, std::vector<ast::AST_Node_Strings*> node_strings);
    int calculate_bytes_multipler(std::vector<int> v);

    namespace ops {
        void exit();
        void print_int();
        void print_char();
        void print_string();
        void read_int();
        void read_char();
        void read_string();
        void save_context_on_stack();
        void load_context_from_stack();
    }

};
#endif  // MIPS_HH