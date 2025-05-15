#include <bits/stdc++.h>

using namespace std;


const int transitions[23][20] = {
                /* +  -  a  c  e  h  i  r  s  t  0  1  2  3  4  5  6  7  8  9 */
    /* state 0 */ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    /* state 1 */ {17, 18, 0, 11, 0, 2, 0, 0, 4, 0, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19},
    /* state 2 */ {0, 0, 0, 0, 3, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    /* state 3 */ {0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    /* state 4 */ {0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    /* state 5 */ {0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    /* state 6 */ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    /* state 7 */ {0, 0, 8, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    /* state 8 */ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    /* state 9 */ {0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   /* state 10 */ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   /* state 11 */ {0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   /* state 12 */ {0, 0, 0, 0, 0, 0, 0, 14, 0, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   /* state 13 */ {0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   /* state 14 */ {0, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   /* state 15 */ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   /* state 16 */ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   /* state 17 */ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   /* state 18 */ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   /* state 19 */ {0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19},
   /* state 20 */ {21, 21, 0, 0, 0, 0, 0, 0, 0, 0, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22},
   /* state 21 */ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22},
   /* state 22 */ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22},
    };

int getIndex(char input) {
    if(input == '+') return 0;
    else if(input == '.') return 1;
    else if(input == 'a') return 2;
    else if(input == 'c') return 3;
    else if(input == 'e') return 4;
    else if(input == 'h') return 5;
    else if(input == 'i') return 6;
    else if(input == 'r') return 7;
    else if(input == 's') return 8;
    else if(input == 't') return 9;
    else if(input >= '0' && input <= '9') return 9 + (input - '0');
    else return -1;
}

bool isFinal(int state) {
    if(state == 3 || state == 6 || state == 8 || state == 10 || state == 13 || state == 14 || state == 15 || state == 16 || state == 17 || state == 18 || state == 19 || state == 22 ) return true;
    else return false;
}

string getToken(int state) {
    switch(state) {
        case 3:
            return "ELE";
        case 6:
            return "ELA";
        case 8:
            return "DELE";
        case 10:
            return "DELA";
        case 13:
            return "GATO";
        case 14:
            return "CARRO";
        case 15:
            return "GATOS";
        case 16:
            return "CARROS";
        case 17:
            return "MAIS";
        case 18:
            return "MENOS";
        case 19:
            return "INTEIRO";
        case 22:
            return "REAL";
        default:
            return "default case";
    }
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
            // Verifica se é possível realizar a transição
            if(transitions[current_state][index] != 0) {
                next_state = transitions[current_state][index];
                current_state = next_state;
                
                current_sequence += character;
            } else { // Nenhuma transição possível
                if(isFinal(current_state) && (current_state == 19 || current_state == 22)) {
                    cout << getToken(current_state) << " " << greatest_sequence << "\n";
                    break;
                } else if(isFinal(current_state)) {
                    cout << getToken(current_state) << "\n";
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