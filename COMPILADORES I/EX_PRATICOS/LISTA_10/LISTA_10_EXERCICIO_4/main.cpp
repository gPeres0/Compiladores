#include "main.h"

using namespace std;
bool cin_start;

vector<Estado> automata = {
        Estado(), //0
        Estado().setLowerAlphabet(2)
                .setTransition('+', 3)
                .setTransition('*', 4)
                .setTransition('(', 5)
                .setTransition(')', 6)
                .setTransition('$', 7),// 1
        Estado("id").setLowerAlphabet(2).setNumbers(2), //2
        Estado("+"), //3
        Estado("*"), //4
        Estado("("), //5
        Estado(")"), //6
        Estado("$"), //7
};

int main() {

    string entrada;
    while (getline(cin, entrada)) {

        try {
            queue<Token> tokens = lexicalParse(entrada, automata);
            syntaticalParse(tokens);
        } catch (LexicalException &e) {
            cout << breakline() << e.what();
        }

    }
}