#ifndef HELPERS_HH
#define HELPERS_HH

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "ast.hh"

namespace helpers {
    void split_format_string(std::string str, int node_number);
    int return_first_unused_register();
    int return_register_type(int reg);
    void free_register(int reg);

    enum register_types {
        ZERO,
        AT,
        V,
        A,
        T,
        S,
        K,
        GP,
        SP,
        FP,
        RA
    };

};

#endif // HELPERS_HH