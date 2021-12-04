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
    Rule() = default;
    Rule(Predicate* newHeadPredicate, std::vector<Predicate*> bodyPredicates);
    ~Rule();
    unsigned int getLength() const;
    Predicate* getRuleAt(int index) const;
    std::vector<Predicate*> getRules() const;
    Predicate* getHead() const;
    std::string toString();
};

#endif //RULE_H
