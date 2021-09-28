#ifndef RIGHT_PARENAUTOMATON_H
#define RIGHT_PARENAUTOMATON_H

#include "Automaton.h"

class Right_ParenAutomaton : public Automaton
{
public:
    Right_ParenAutomaton() : Automaton(TokenType::RIGHT_PAREN) {}  // Call the base constructor

    void S0(const std::string& input);
};

#endif //RIGHT_PARENAUTOMATON_H
