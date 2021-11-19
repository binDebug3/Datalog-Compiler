#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <map>
#include "DatalogProgram.h"
#include "Database.h"
#include "Relacion.h"
#include "Predicate.h"
#include "Rule.h"

class Interpreter {
private:
    DatalogProgram datalog;
    Database database;
    std::vector<Relation*> evaluateQueries{};
    std::vector<Relation*> evaluateRules;
    std::map<std::string, std::vector<int>> outputIndex;
    int numPasses = 0;

    void buildSchemes(std::vector<Predicate> schemes);
    void buildFacts(std::vector<Predicate> facts);
    void buildQueries(const std::vector<Predicate>& queries);
    void buildRules(const std::vector<Rule>& rules);
    Relation* evaluatePredicate(Predicate predicate);

public:
    Interpreter(const DatalogProgram& datalog);
    ~Interpreter();
    std::string toString();
};

#endif //INTERPRETER_H