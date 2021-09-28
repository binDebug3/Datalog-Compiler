#include "IDAutomaton.h"

void IDAutomaton::S0(const std::string& input) {
    if (std::isalpha(input[index])) {
        inputRead++;
        index++;
        S1(input);
    }
    else {
        Serr();
    }
}

void IDAutomaton::S1(const std::string& input) {
    if (std::isalnum(input[index])) {
        inputRead++;
        index++;
        S1(input);
    }
}