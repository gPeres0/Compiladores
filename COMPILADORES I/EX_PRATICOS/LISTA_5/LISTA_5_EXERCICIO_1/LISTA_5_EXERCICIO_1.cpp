#include <bits/stdc++.h>

using namespace std;

const int transitions[9][38] = {
                /* +  -  a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p  q  r  s  t  u  v  w  x  y  z  0  1  2  3  4  5  6  7  8  9 */
    /* state 0 */ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    /* state 1 */ {3, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
    /* state 2 */ {0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    /* state 3 */ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    /* state 4 */ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    /* state 5 */ {0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
    /* state 6 */ {7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
    /* state 7 */ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
    /* state 8 */ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8}
    };

int getIndex(char input) {
    if(input == '+')   return 0;
    else if(input == '-')   return 1;
    else if(input >= 'a' && input <= 'z') return 2 + (input - 'a');
    else if(input >= '0' && input <= '9') return 2 + 26 + (input - '0');
    else return -1;
}

bool isFinal(int state) {
    if(state == 2 || state == 3 || state == 4 || state == 5 || state == 8) return true;
    else return false;
}

int main(void) {
    string input;

    while(getline(cin, input)) {
        string current_sequence;
        string greatest_sequence;
        
        int current_state = 1;
        int next_state = 0;
        int index = 0;


        for(int i = 0; i <= input.length(); i++) {
            char character = input[i];
            
            index = getIndex(character);
            if(index < 0) {
                if (isFinal(current_state)) {
                    cout << greatest_sequence << "\n";
                } else {
                    cout << "ERRO\n";
                }
            }

            // Verifica se é possível realizar a transição
            if(transitions[current_state][index] != 0) {
                next_state = transitions[current_state][index];
                current_state = next_state;
                
                current_sequence += character;
            } else { // Nenhuma transição possível
                if (isFinal(current_state)) {
                    cout << greatest_sequence << "\n";
                    break;
                } else {
                    cout << "ERRO\n";
                }
            }

            greatest_sequence = current_sequence;
        }
    }

    return 0;
}