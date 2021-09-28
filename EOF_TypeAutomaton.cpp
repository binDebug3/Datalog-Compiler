#include "EOF_TypeAutomaton.h"

void EOF_TypeAutomaton::S0(const std::string& input) {
    if (input[index] == EOF) {
        inputRead = 1;
    }
    else {
        Serr();
    }
}