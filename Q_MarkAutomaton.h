#ifndef Q_MarkAUTOMATON_H
#define Q_MarkAUTOMATON_H

#include "Automaton.h"

class Q_MarkAutomaton : public Automaton
{
public:
    Q_MarkAutomaton() : Automaton(TokenType::Q_MARK) {}  // Call the base constructor

    void S0(const std::string& input);
};


#endif //Q_MarkAUTOMATON_H
