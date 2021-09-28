#include "StringAutomaton.h"

void StringAutomaton::S0(const std::string& input) {
    numDots = 0;
    if (input[index] == '\'') {
        inputRead++;
        index++;
        numDots++;
        S1(input);
    }
    else {
        Serr();
    }
}

void StringAutomaton::S1(const std::string& input) {
    if (input[index] != '\'' && index < input.size()) {
        if (input[index] == '\n')
            newLines++;
        inputRead++;
        index++;
        S1(input);
    }
    else if (input[index] == '\'') {
        inputRead++;
        index++;
        numDots++;
        S2(input);
    }
    else if (index >= input.size()) {
        hitEnd = true;
    }
    else {
        Serr();
    }
}

void StringAutomaton::S2(const std::string& input) {
    if (input[index] == '\'') {
        inputRead++;
        index++;
        numDots++;
        S1(input);
    }
    if (numDots % 2 != 0)
        Serr();
}