#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <map>
#include "DatalogProgram.h"
#include "Database.h"
#include "Relation.h"
#include "Predicate.h"
#include "Rule.h"

class Interpreter {
private:
    DatalogProgram datalog;
    Database database;
    std::vector<Relation*> evaluateQueries{};
    std::map<std::string, std::vector<int>> outputIndex;

    void buildSchemes(std::vector<Predicate> schemes);
    void buildFacts(std::vector<Predicate> facts);
    void buildQueries(std::vector<Predicate> queries);
    //void buildRules(std::vector<Rule> rules);
    Relation* evaluatePredicate(Predicate predicate);

public:
    Interpreter(const DatalogProgram& datalog);
    ~Interpreter();
    std::string toString();
};

#endif //INTERPRETER_H