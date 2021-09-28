#include "Rule.h"

Rule::Rule(Predicate* newHeadPredicate, std::vector<Predicate*> bodyPredicates) {
    headPredicate = newHeadPredicate;
    this->bodyPredicates = bodyPredicates;
}
Rule::~Rule() {}

std::string Rule::toString() {
    std::string output = "";
    output += headPredicate->toString() + " :- ";
    for (unsigned int i=0; i<bodyPredicates.size()-1; i++) {
        output += bodyPredicates[i]->toString() + ",";
    }
    output += bodyPredicates[bodyPredicates.size()-1]->toString() + ".";
    return output;
}