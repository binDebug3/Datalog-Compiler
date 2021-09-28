#ifndef RULE_H
#define RULE_H

#include <vector>
#include "Token.h"
#include "Predicate.h"

class Rule {
private:
    Predicate* headPredicate;
    std::vector<Predicate*> bodyPredicates;

public:
    Rule(Predicate* newHeadPredicate, std::vector<Predicate*> bodyPredicates);
    ~Rule();

    std::string toString();
};

#endif //RULE_H
