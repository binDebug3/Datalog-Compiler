#include "CommentAutomaton.h"

void CommentAutomaton::S0(const std::string& input) {
    if (input[index] == '#') {
        inputRead++;
        index++;
        S1(input);
    }
    else {
        Serr();
    }
}

void CommentAutomaton::S1(const std::string& input) {
    if (input[index] == '|') {
        inputRead++;
        index++;
        S3(input);
    }
    else if (input[index] != '\n') {
        inputRead++;
        index++;
        S2(input);
    }
    else if (index >= input.size()) {
        hitEnd = true;
        Serr();
    }
    else {
        Serr();
    }
}

void CommentAutomaton::S2(const std::string& input) {
    if (input[index] != '\n') {
        inputRead++;
        index++;
        S2(input);
    }
}

void CommentAutomaton::S3(const std::string& input) {
    if (input[index] == '|' && index < input.size()) {
        inputRead++;
        index++;
        S4(input);
    }
    else if (index < input.size()) {
        if (input[index] == '\n')
            newLines++;
        inputRead++;
        index++;
        S3(input);
    }
    else if (index >= input.size()) {
        hitEnd = true;
        Serr();
    }
    else {
        Serr();
    }
}

void CommentAutomaton::S4(const std::string& input) {
    if (input[index] == '#') {
        inputRead++;
    }
    else if (index < input.size()){
        if (input[index] == '\n')
            newLines++;
        inputRead++;
        index++;
        S3(input);
    }
    else if (index >= input.size()) {
        hitEnd = true;
        Serr();
    }
}