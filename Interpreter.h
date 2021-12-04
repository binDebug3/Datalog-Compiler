#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <map>
#include "DatalogProgram.h"
#include "Database.h"
#include "Relacion.h"
#include "Predicate.h"
#include "Rule.h"
#include "Graph.h"

class Interpreter {
private:
    DatalogProgram datalog;
    Database database;
    std::vector<Relation*> evaluateQueries{};
    std::vector<Relation*> evaluateRules;
    std::map<std::string, std::vector<int>> outputIndex;
    int numPasses = 0;
    int count = 0;

    Graph graph;
    Graph reverseGraph;

    void buildSchemes(std::vector<Predicate> schemes);
    void buildFacts(std::vector<Predicate> facts);
    void buildQueries(const std::vector<Predicate>& queries);
    void buildRules(const std::vector<Rule>& rules);
    Relation* evaluatePredicate(Predicate predicate);

    std::vector<std::set<int>> optimizeRules(const std::vector<Rule>& rules);
    void buildGraph(const std::vector<Rule>& rules);
    void buildReverseGraph(const std::vector<Rule>& rules);
    std::vector<int> DFS(int node);
    std::set<int> DFS(int node, int countSCC);
    std::vector<int> dfsForest();
    std::vector<std::set<int>> dfsForest(bool searchForest);

public:
    Interpreter(const DatalogProgram& datalog);
    ~Interpreter();
    std::string toString();
};

#endif //INTERPRETER_H