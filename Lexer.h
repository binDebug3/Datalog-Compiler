#ifndef LEXER_H
#define LEXER_H
#include <vector>
#include "Automaton.h"
#include "Token.h"

class Lexer
{
private:
    std::vector<Automaton*> automata;
    std::vector<Token*> tokens;
    void CreateAutomata();

public:
    Lexer();
    ~Lexer();

    void Run(std::string& input);
    std::string printTokens();
    std::vector<Token*> sendTokens();
    int getLine();

};

#endif // LEXER_H

