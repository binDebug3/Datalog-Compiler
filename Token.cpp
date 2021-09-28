#include "Token.h"

Token::Token(TokenType type, std::string description, int line) {
    // TODO: initialize all member variables
    this->type = type;
    this->value = description;
    this->line = line;
}

std::string Token::tokenToString() {
    std::string output = "(";
    switch (type) {
        case COLON: output += "COLON"; break;
        case COLON_DASH: output += "COLON_DASH"; break;
        case COMMA: output += "COMMA"; break;
        case PERIOD: output += "PERIOD"; break;
        case Q_MARK: output += "Q_MARK"; break;
        case LEFT_PAREN: output += "LEFT_PAREN"; break;
        case RIGHT_PAREN: output += "RIGHT_PAREN"; break;
        case MULTIPLY: output += "MULTIPLY"; break;
        case ADD: output += "ADD"; break;
        case SCHEMES: output += "SCHEMES"; break;
        case FACTS: output += "FACTS"; break;
        case RULES: output += "RULES"; break;
        case QUERIES: output += "QUERIES"; break;
        case STRING: output += "STRING"; break;
        case COMMENT: output += "COMMENT"; break;
        case ID: output += "ID"; break;
        case UNDEFINED: output += "UNDEFINED"; break;
        case EOF_TYPE: output += "EOF"; break;
        default: return "ERROR"; break;
    }
    output += ",\"" + value + "\"," + std::to_string(line) + ")\n";
    return output;
}

TokenType Token::getTokenType() {
    return type;
}

std::string Token::getValue() {
    return value;
}