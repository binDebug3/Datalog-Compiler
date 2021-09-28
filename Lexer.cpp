#include <string>
#include <iostream>

#include "Lexer.h"
#include "Token.h"
#include "ColonAutomaton.h"
#include "ColonDashAutomaton.h"
#include "CommaAutomaton.h"
#include "PeriodAutomaton.h"
#include "Q_MarkAutomaton.h"
#include "Left_ParenAutomaton.h"
#include "Right_ParenAutomaton.h"
#include "MultiplyAutomaton.h"
#include "AddAutomaton.h"
#include "SchemesAutomaton.h"
#include "FactsAutomaton.h"
#include "RulesAutomaton.h"
#include "QueriesAutomaton.h"
#include "StringAutomaton.h"
#include "CommentAutomaton.h"
#include "EOF_TypeAutomaton.h"
#include "IDAutomaton.h"

Lexer::Lexer() {
    tokens = std::vector<Token*>();
    automata = std::vector<Automaton*>();
    CreateAutomata();
}

Lexer::~Lexer() {
    // TODO: need to clean up the memory in `automata` and `tokens`
    for (unsigned int i=0; i < automata.size(); i++) {
        delete automata[i];
    }
    for (unsigned int i=0; i < tokens.size(); i++) {
        delete tokens[i];
    }
}

void Lexer::CreateAutomata() {
    automata.push_back(new ColonAutomaton());
    automata.push_back(new ColonDashAutomaton());
    automata.push_back(new CommaAutomaton());
    automata.push_back(new PeriodAutomaton());
    automata.push_back(new Q_MarkAutomaton());
    automata.push_back(new Left_ParenAutomaton());
    automata.push_back(new Right_ParenAutomaton());
    automata.push_back(new MultiplyAutomaton());
    automata.push_back(new AddAutomaton());
    automata.push_back(new SchemesAutomaton());
    automata.push_back(new FactsAutomaton());
    automata.push_back(new RulesAutomaton());
    automata.push_back(new QueriesAutomaton());
    automata.push_back(new StringAutomaton());
    automata.push_back(new CommentAutomaton());
    automata.push_back(new EOF_TypeAutomaton());
    automata.push_back(new IDAutomaton());
}

void Lexer::Run(std::string& input) {
    //initialize variables
    int lineNumber = 1;
    int maxRead = 0;
    int currentRead = 0;
    Automaton* maxAutomaton = automata[0];
    int maybe = 0;
    while (input.size() > 0) {
        maybe++;
        maxRead = 0;
        maxAutomaton = automata[0];
        //check for whitespace
        while (input.size() > 0 && isspace(input.at(0))) {
            if(input[0] == '\n')
                lineNumber++;
            input.erase(0, 1);
        }
        if (input.size() == 0) {
            break;
        }
        //read in a token
        for(unsigned int i=0; i < automata.size(); i++) {
            currentRead = automata[i]->Start(input);
            if (currentRead > maxRead) {
                maxRead = currentRead;
                maxAutomaton = automata[i];
            }
        }
        //generate a token
        if (maxRead > 0) {
            Token* newToken = maxAutomaton->CreateToken(input.substr(0, maxRead), lineNumber);
            lineNumber += maxAutomaton->NewLinesRead();
            if (newToken->getTokenType() != COMMENT) {
                tokens.push_back(newToken);
            }
        }
        //undefined handling
        else {
            //maxRead needs to not be 1 when a string or comment hits the end of a file
            maxRead = 1;
            if (automata[13]->hitEndOfFile() || automata[14]->hitEndOfFile())
                maxRead = input.size();
            Token* newToken = new Token(UNDEFINED, input.substr(0, maxRead), lineNumber);
            tokens.push_back(newToken);
            if (automata[13]->hitEndOfFile())
                lineNumber += automata[13]->NewLinesRead();
            else if (automata[14]->hitEndOfFile())
                lineNumber += automata[14]->NewLinesRead();
        }
        input.erase(0, maxRead);
    }
    Token* newToken = new Token(EOF_TYPE, "", lineNumber);
    tokens.push_back(newToken);
}

std::string Lexer::printTokens() {
    std::string outputString = "";
    for (unsigned int i=0; i < tokens.size(); i++) {
        outputString += tokens[i]->tokenToString();
    }
    outputString += "Total Tokens = " + std::to_string(tokens.size());
    return outputString;
}

std::vector<Token*> Lexer::sendTokens() {
    return tokens;
}