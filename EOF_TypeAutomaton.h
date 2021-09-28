#ifndef EOF_TYPEAUTOMATON_H
#define EOF_TYPEAUTOMATON_H

#include "Automaton.h"

class EOF_TypeAutomaton : public Automaton
{
public:
    EOF_TypeAutomaton() : Automaton(TokenType::EOF_TYPE) {}  // Call the base constructor

    void S0(const std::string& input);
};

#endif //EOF_TYPEAUTOMATON_H
