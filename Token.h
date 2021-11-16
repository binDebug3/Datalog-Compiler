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

public:
    Token(TokenType type, std::string description, int line);

    std::string tokenToString();
    TokenType getTokenType();
    std::string getValue();
};

#endif // TOKEN_H