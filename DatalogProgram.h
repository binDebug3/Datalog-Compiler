#ifndef DATALOGPROGRAM_H
#define DATALOGPROGRAM_H

#include <vector>
#include <set>
#include "Token.h"
#include "Predicate.h"
#include "Rule.h"

class DatalogProgram {
private:
    std::vector<Predicate> schemes;
    std::vector<Predicate> facts;
    std::vector<Predicate> queries;
    std::vector<Rule> rules;
    std::set<std::string> domain;

public:
    DatalogProgram();
    ~DatalogProgram();

    std::string toString();
    void addScheme(Predicate createScheme);
    void addFact(Predicate createFact);
    void addQuery(Predicate createQuery);
    void addRule(Rule createRule);
    void addDomain(std::string createDomain);
};

#endif //DATALOGPROGRAM_H
