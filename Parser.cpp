#include "Parser.h"

Parser::Parser() {};
Parser::~Parser() {};

//----------------Helper Functions---------------------------------------------------

bool Parser::match(Token* current, TokenType required) {
    if (current->getTokenType() == required)
        return true;
    return false;
}
void Parser::parseToken(Token* current, TokenType required, std::string stringID) {
    if (current->getTokenType() == required)
        index++;
    else
        throw printError(required, stringID);
}
std::string Parser::printError(TokenType expectation, std::string stringID = "terminal token") {
    //return "Expected: " + typeToString.at(expectation-1) + " but got " + tokens[index]->tokenToString();
    return "  " + tokens[index]->tokenToString();
}
std::string Parser::toString() {
    return exito + datalog.toString();
}


//************************GRAMMAR********************************************************

void Parser::parseDatalogProgram(std::vector<Token*> inputTokens) {
    tokens = inputTokens;
    index = 0;
    if (match(tokens[index], SCHEMES)) {
        parseToken(tokens[index], SCHEMES);
        parseToken(tokens[index], COLON);
        parseScheme();
        parseSchemeList();
        parseToken(tokens[index], FACTS);
        parseToken(tokens[index], COLON);
        parseFactList();
        parseToken(tokens[index], RULES);
        parseToken(tokens[index], COLON);
        parseRuleList();
        parseToken(tokens[index], QUERIES);
        parseToken(tokens[index], COLON);
        parseQuery();
        parseQueryList();
        //eof
        exito = "Success!\n";
    }
    else
        throw printError(SCHEMES);
}
void Parser::parseSchemeList() {
    if (match(tokens[index], ID)) {
        parseScheme();
        parseSchemeList();
    }
    else if (match(tokens[index], FACTS)) {}
    else
        throw printError(ID);
}
void Parser::parseFactList() {
    if (match(tokens[index], ID)) {
        parseFact();
        parseFactList();
    }
    else if (match(tokens[index], RULES)) {}
    else
        throw printError(ID);
}
void Parser::parseRuleList() {
    if (match(tokens[index], ID)) {
        parseRule();
        parseRuleList();
    }
    else if (match(tokens[index], QUERIES)) {}
    else
        throw printError(ID);
}
void Parser::parseQueryList() {
    if (match(tokens[index], ID)) {
        parseQuery();
        parseQueryList();
    }
    else if (match(tokens[index], EOF_TYPE)) {}
    else
        throw printError(ID);
}

void Parser::parseScheme() {
    if (match(tokens[index], ID)) {
        std::vector<Parameter*> newParameters;
        parseToken(tokens[index], ID);
        std::string schemeID = tokens[index-1]->getValue();
        parseToken(tokens[index], LEFT_PAREN);
        parseToken(tokens[index], ID);
        newParameters.push_back(new Parameter(tokens[index-1]->getValue()));
        parseIDList(newParameters);
        parseToken(tokens[index], RIGHT_PAREN);
        Predicate newScheme(schemeID, newParameters);
        datalog.addScheme(newScheme);
    }
    else
        throw printError(ID);
}
void Parser::parseFact() {
    std::string tempFact;
    int tempIndex = index;
    if (match(tokens[index], ID)) {
        std::vector<Parameter*> newParameters;
        parseToken(tokens[index], ID);
        std::string factID = tokens[index-1]->getValue();
        parseToken(tokens[index], LEFT_PAREN);
        parseToken(tokens[index], STRING);
        newParameters.push_back(new Parameter(tokens[index-1]->getValue()));
        datalog.addDomain(tokens[index-1]->getValue());
        parseStringList(newParameters);
        parseToken(tokens[index], RIGHT_PAREN);
        parseToken(tokens[index], PERIOD);
        unsigned int factLength = index - tempIndex + 1;
        for (unsigned int i=0; i<factLength; i++) {
            tempFact += tokens[i+tempIndex]->getValue();
        }
        Predicate newFact(factID, newParameters);
        datalog.addFact(newFact);
    }
    else
        throw printError(ID);
}
void Parser::parseRule() {
    if (match(tokens[index], ID)) {
        //headPredicate
        std::vector<Parameter*> headParameters;
        std::string headID = parseHeadPredicate(headParameters);
        Predicate* newHeadPredicate = new Predicate(headID, headParameters);
        parseToken(tokens[index], COLON_DASH);
        //first predicate
        std::vector<Predicate*> newPredicates;
        std::vector<Parameter*> newParameters;
        std::string ruleID = parsePredicate(newParameters);
        newPredicates.push_back(new Predicate(ruleID, newParameters));
        //all the other predicates
        parsePredicateList(newPredicates);
        parseToken(tokens[index], PERIOD);
        Rule newRule(newHeadPredicate, newPredicates);
        datalog.addRule(newRule);
    }
    else
        throw printError(ID);
}
void Parser::parseQuery() {
    if (match(tokens[index], ID)) {
        std::vector<Parameter*> newParameters;
        std::string queryID = parsePredicate(newParameters);
        parseToken(tokens[index], Q_MARK);
        Predicate newQuery(queryID, newParameters);
        datalog.addQuery(newQuery);
    }
    else
        throw printError(ID);
}

std::string Parser::parseHeadPredicate(std::vector<Parameter*> &newParameter) {
    if (match(tokens[index], ID)) {
        parseToken(tokens[index], ID);
        std::string predicateID = tokens[index-1]->getValue();
        parseToken(tokens[index], LEFT_PAREN);
        parseToken(tokens[index], ID);
        newParameter.push_back(new Parameter(tokens[index-1]->getValue()));
        parseIDList(newParameter);
        parseToken(tokens[index], RIGHT_PAREN);
        return predicateID;
    }
    else
        throw printError(ID);
}
std::string Parser::parsePredicate(std::vector<Parameter*> &newParameter) {
    if (match(tokens[index], ID)) {
        parseToken(tokens[index], ID);
        std::string predicateID = tokens[index-1]->getValue();
        parseToken(tokens[index], LEFT_PAREN);
        parseParameter();
        parseParameterList(newParameter);
        parseToken(tokens[index], RIGHT_PAREN);
        return predicateID;
    }
    else
        throw printError(ID);
}

void Parser::parsePredicateList(std::vector<Predicate*> &newPredicate) {
    if (match(tokens[index], COMMA)) {
        std::vector<Parameter*> newParameters;
        parseToken(tokens[index], COMMA);
        std::string predicateID = parsePredicate(newParameters);
        newParameters.push_back(new Parameter(tokens[index-1]->getValue()));
        newPredicate.push_back(new Predicate(predicateID, newParameters));
        parsePredicateList(newPredicate);
    }
    else if (match(tokens[index], PERIOD)) {}
    else
        throw printError(COMMA);
}
void Parser::parseParameterList(std::vector<Parameter*> &newParameter) {
    if (match(tokens[index], COMMA)) {
        parseToken(tokens[index], COMMA);
        parseParameter();
        newParameter.push_back(new Parameter(tokens[index-1]->getValue()));
        parseParameterList(newParameter);
    }
    else if (match(tokens[index], RIGHT_PAREN)) {}
    else
        throw printError(COMMA);
}
void Parser::parseStringList(std::vector<Parameter*> &newParameter) {
    if (match(tokens[index], COMMA)) {
        parseToken(tokens[index], COMMA);
        parseToken(tokens[index], STRING);
        newParameter.push_back(new Parameter(tokens[index-1]->getValue()));
        datalog.addDomain(tokens[index-1]->getValue());
        parseStringList(newParameter);
    }
    else if (match(tokens[index], RIGHT_PAREN)) {}
    else
        throw printError(COMMA);
}
void Parser::parseIDList(std::vector<Parameter*> &newParameter) {
    if (match(tokens[index], COMMA)) {
        parseToken(tokens[index], COMMA);
        parseToken(tokens[index], ID);
        newParameter.push_back(new Parameter(tokens[index-1]->getValue()));
        parseIDList(newParameter);
    }
    else if (match(tokens[index], RIGHT_PAREN)) {}
    else
        throw printError(COMMA);
}
void Parser::parseParameter() {
    if (match(tokens[index], STRING)) {
        parseToken(tokens[index], STRING);
    }
    else if (match(tokens[index], ID))
        parseToken(tokens[index], ID);
    else
        throw printError(STRING);
}

