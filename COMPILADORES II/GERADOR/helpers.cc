#include "ast.hh"
#include "helpers.hh"

extern std::vector<ast::AST_Node_Strings*> node_strings;
extern bool used_registers[32];
extern std::vector<std::string> register_names;

namespace helpers {

    void split_format_string(std::string str, int node_number) {
        std::vector<std::string> tokens;
        std::string part = "";

        for(int i = 0; i < str.length(); i++) {
            if(str[i] == '%') {
                if(part == "") {
                    part += str[i];
                    part += str[i + 1];
                    tokens.push_back(part);
                    part = "";
                    i++;
                } else {
                    tokens.push_back(part);
                    part = "";
                    part += str[i];
                    part += str[i + 1];
                    tokens.push_back(part);
                    part = "";
                    i++;
                }
            } else {
                part += str[i];
            }
        }

        if(part != "") {
            tokens.push_back(part);
        }

        ast::AST_Node_Strings* node = new ast::AST_Node_Strings(tokens, node_number);
        node_strings.push_back(node);
    }

    int return_first_unused_register() {
        for(int i = 8; i < 16; i++) {
            if(!used_registers[i]) {
                used_registers[i] = true;
                return i;
            }
        }

        for(int i = 24; i < 26; i++) {
            if(!used_registers[i]) {
                used_registers[i] = true;
                return i;
            }
        }

        return -1;
    }

    void free_register(int reg) {
        used_registers[reg] = false;
    }

    int return_register_type(int reg) {
        if (reg == 0) {
            return ZERO;
        } else if (reg == 1) {
            return AT;
        } else if (reg >= 2 && reg <= 3) {
            return V;
        } else if (reg >= 4 && reg <= 7) {
            return A;
        } else if (reg >= 8 && reg <= 15) {
            return T;
        } else if (reg >= 16 && reg <= 23) {
            return S;
        } else if (reg >= 24 && reg <= 25) {
            return T;
        } else if (reg == 26 || reg == 27) {
            return K;
        } else if (reg == 28) {
            return GP;
        } else if (reg == 29) {
            return SP;
        } else if (reg == 30) {
            return FP;
        } else if (reg == 31) {
            return RA;
        } else {
            return -1;
        }
    }

};