#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "Token.h"
#include "DatalogProgram.h"

class Parser
{
private:
    std::vector<Token*> tokens;
    int index;
    DatalogProgram datalog;
    std::string exito = "";
    std::vector<std::string> typeToString = {
        "COLON,"
        "COLON_DASH",
        "COMMA",
        "PERIOD",
        "Q_MARK",
        "LEFT_PAREN",
        "RIGHT_PAREN",
        "MULTIPLY",
        "ADD",
        "SCHEMES",
        "FACTS",
        "RULES",
        "QUERIES",
        "STRING",
        "COMMENT",
        "ID",
        "UNDEFINED",
        "EOF_TYPE"
    };

public:
    Parser();
    ~Parser();

    std::string printTokens();

    bool match(Token* current, TokenType required);
    void parseToken(Token* current, TokenType required, std::string stringID = "terminal token");
    std::string printError(TokenType expectation, std::string stringID);
    std::string toString();

    //TERMINALS are ALL CAPS and nonterminals (the interesting ones) are lowercase

    void parseDatalogProgram(std::vector<Token*> inputTokens);

    void parseSchemeList();
    void parseFactList();
    void parseRuleList();
    void parseQueryList();

    void parseScheme();
    void parseFact();
    void parseRule();
    void parseQuery();

    std::string parseHeadPredicate(std::vector<Parameter*> &newParameter);
    std::string parsePredicate(std::vector<Parameter*> &newParameter);

    void parsePredicateList(std::vector<Predicate*> &newPredicate);
    void parseParameterList(std::vector<Parameter*> &newParameter);
    void parseStringList(std::vector<Parameter*> &newParameter);
    void parseIDList(std::vector<Parameter*> &newParameter);
    void parseParameter();
};

#endif //PARSER_H
