#ifndef TOKEN_H
#define TOKEN_H
#include <string>

enum TokenType {
    COLON,
    COLON_DASH,
    COMMA,
    PERIOD,
    Q_MARK,
    LEFT_PAREN,
    RIGHT_PAREN,
    MULTIPLY,
    ADD,
    SCHEMES,
    FACTS,
    RULES,
    QUERIES,
    STRING,
    COMMENT,
    ID,
    UNDEFINED,
    EOF_TYPE
};

class Token
{
private:
    std::string value;
    int line;
    TokenType type;
    // TODO: add member variables for information needed by Token

public:
    Token(TokenType type, std::string description, int line);

    std::string tokenToString();
    TokenType getTokenType();
    std::string getValue();
    // TODO: add other needed methods
};

#endif // TOKEN_H