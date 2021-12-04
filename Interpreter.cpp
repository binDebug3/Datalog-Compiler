#include "Interpreter.h"
#include <iostream>

Interpreter::Interpreter(const DatalogProgram& datalog) {
    this->datalog = datalog;
    buildSchemes(datalog.getSchemes());
    buildFacts(datalog.getFacts());
    buildRules(datalog.getRules());
    buildQueries(datalog.getQueries());
}
Interpreter::~Interpreter() = default;

void Interpreter::buildSchemes(std::vector<Predicate> inputSchemes) {
    for (auto & inputScheme : inputSchemes) {
        auto scheme = new Relation();
        std::string keyName = inputScheme.getName();
        scheme->setName(keyName);
        Header header;
        header.setAttributes(inputScheme.getParameters());
        scheme->setHeader(header);
        database.addRelation(keyName, scheme);
    }
}
void Interpreter::buildFacts(std::vector<Predicate> facts) {
    for (auto & fact : facts) {
        Tuple createTuple;
        for (unsigned int j=0; j<fact.getParameters().size(); j++) {
            createTuple.addValue(fact.getPofIndex(j));
        }
        database.addDatabaseTuple(fact.getName(), createTuple);
    }
}
void Interpreter::buildQueries(const std::vector<Predicate>& queries) {
    for (auto & querie : queries) {
        evaluateQueries.push_back(this->evaluatePredicate(querie));
    }
}
void Interpreter::buildRules(const std::vector<Rule>& rules) {
    std::vector<std::set<int>> strongCCs;
    strongCCs = optimizeRules(rules);
    //std::cout << graph.printSCC();
    std::cout << "Rule Evaluation" << std::endl;
    std::string output;
    unsigned int component = 0;
    while (component < strongCCs.size()) {
        bool somethingChanged = true;
        numPasses = 0;
        std::cout << "SCC: " << graph.printComponent(component);
        while (somethingChanged) {
            somethingChanged = false;
            for (auto vertex = strongCCs.at(component).begin(); vertex != strongCCs.at(component).end(); vertex++) {
                Rule rule = rules[*vertex];
                //evaluate the predicates on the right-hand side of the rule
                evaluateRules.clear();
                for (unsigned int j = 0; j < rule.getLength(); j++) {
                    evaluateRules.push_back(this->evaluatePredicate(*rule.getRuleAt(j)));
                }
                std::cout << rule.getHead()->toString() << " :- ";
                output = "";
                for (unsigned int j = 0; j < rule.getLength(); j++) {
                    output += rule.getRuleAt(j)->toString() + ",";
                }
                output = output.substr(0, output.length() - 1);
                std::cout << output << "." << std::endl;

                //join the relations that result
                auto joinedRelations = new Relation();
                //joinedRelations with at least one actual relation explicitly
                joinedRelations->setName(evaluateRules.at(0)->getName());
                joinedRelations->setHeader(evaluateRules.at(0)->getHeader());
                joinedRelations->setTuples(evaluateRules.at(0)->getTuples());
                for (unsigned int i = 1; i < evaluateRules.size(); i++) {
                    joinedRelations = joinedRelations->join(evaluateRules.at(i));
                }

                //project the columns that appear in the head predicate
                std::vector<int> projectIndex;
                Header relationHead = joinedRelations->getHeader();
                Predicate *headPred = rule.getHead();
                for (unsigned int j = 0; j < headPred->getLength(); j++) {
                    for (unsigned int i = 0; i < relationHead.getLength(); i++) {
                        if (relationHead.getAttributeAt(i) == headPred->getPofIndex(j)) {
                            projectIndex.push_back(i);
                        }
                    }
                }
                joinedRelations = joinedRelations->project(projectIndex);

                //rename the relations to make it union-compatible (at least make sure the indices are matched up)
                std::string searchKey = rule.getHead()->getName();
                Relation *namingScheme = database.checkMap()[searchKey];
                Header renameHeader = namingScheme->getHeader();
                std::vector<std::string> renameAttributes = renameHeader.getAttributes();
                joinedRelations = joinedRelations->rename(renameAttributes);

                //union with the relation in the database
                bool lastOneChanged;
                //std::cout << output;
                lastOneChanged = namingScheme->unionRelations(joinedRelations);
                if (lastOneChanged)
                    somethingChanged = true;
                if (numPasses > 100)
                    somethingChanged = false;
            }
            if (strongCCs.at(component).size() == 1) {
                auto vert = strongCCs.at(component).begin();
                Rule checkDepend = rules[*vert];
                bool eachOneFails = true;
                for (unsigned int eachPred = 0; eachPred < checkDepend.getLength(); eachPred++) {
                    if (checkDepend.getHead()->getName() == checkDepend.getRuleAt(eachPred)->getName())
                        eachOneFails = false;
                }
                if (eachOneFails)
                    somethingChanged = false;
            }
            numPasses++;
        }
        std::cout << std::to_string(numPasses) << " passes: " << graph.printComponent(component);
        component++;
    }
}

Relation* Interpreter::evaluatePredicate(Predicate predicate) {
    //lots of data members
    std::string searchKey = predicate.getName();
    std::vector<Parameter*> searchParameters = predicate.getParameters(true);
    Relation *createRelation = database.checkMap()[searchKey];
    //and lots more for checking duplicates
    std::map<std::string, int> findDuplicates;
    std::vector<std::string> yaFound;
    std::string duplicateFoundAt;
    bool select2 = false;

    //SELECT
    for (unsigned int i = 0; i < searchParameters.size(); i++) {
        //if the parameter is a constant
        if (searchParameters[i]->isConstant()) {
            //create a new relation and set it equal to the correct relation. we find the correct relation by searching the relationsMap in the database Once we find the correct relation, perform the select operation using the index and corresponding value of the query parameter we discovered is a constant
            createRelation = createRelation->select(i, searchParameters[i]->toString());
            yaFound.push_back("Constant");
        }
        //if the parameter is a variable, update tracking data structures
        else {
            yaFound.push_back(searchParameters[i]->toString());
            auto result = findDuplicates.insert(std::pair<std::string, int>(searchParameters[i]->toString(), 1));
            if (!result.second)
                result.first->second++;
        }
        //check for duplicate variables
        for (auto &elem: findDuplicates) {
            if (elem.second > 1) {
                select2 = true;
                duplicateFoundAt = elem.first;
            }
        }
        //if you find two identical variables
        if (select2) {
            unsigned int index1 = 987657, index2 = 987657;
            for (unsigned int ind = 0; ind < yaFound.size(); ind++) {
                if (yaFound[ind] == duplicateFoundAt) {
                    if (index1 == 987657)
                        index1 = ind;
                    else
                        index2 = ind;
                }
            }
            createRelation = createRelation->select(index1, index2);
        }
    }

    //PROJECT
    std::vector<int> projectOn;
    std::vector<std::string> renameString;
    for (unsigned int i = 0; i < yaFound.size(); i++) {
        if (yaFound[i] != "Constant") {
            projectOn.push_back(i);
            renameString.push_back(yaFound[i]);
        }
    }
    outputIndex.insert(std::pair<std::string, std::vector<int>>(searchKey,projectOn));
    createRelation = createRelation->project(projectOn);

    //RENAME
    createRelation = createRelation->rename(renameString);
    return createRelation;
}

std::vector<std::set<int>> Interpreter::optimizeRules(const std::vector<Rule>& rules) {
    std::vector<std::set<int>> strongCCs;
    buildGraph(rules);
    buildReverseGraph(rules);
    dfsForest();
    strongCCs = dfsForest(true);
    std::cout << graph.toString();
    return strongCCs;
}
void Interpreter::buildGraph(const std::vector<Rule>& rules) {
    for (unsigned int i = 0; i < rules.size(); i++) {
        std::set<int> edges;
        std::vector<Predicate *> preEdges = rules.at(i).getRules();
        //find any matching rules
        for (auto & preEdge : preEdges) {
            std::string edgeName = preEdge->getName();
            for (unsigned int k = 0; k < rules.size(); k++)
                if (rules.at(k).getHead()->getName() == edgeName)
                    edges.insert(k);
        }
        graph.insertNode(i, edges);
    }
}
void Interpreter::buildReverseGraph(const std::vector<Rule>& rules) {
    std::vector<int> visited;
    bool yaVisited;
    for (unsigned int i = 0; i < rules.size(); i++) {
        std::set<int> edges = graph.getEdgesOfNode(i);
        auto iterEdge = edges.begin();
        for (unsigned int j = 0; j < edges.size(); j++) {
            std::set<int> reverseEdges;
            yaVisited = false;
            for (int k : visited)
                if (k == *iterEdge)
                    yaVisited = true;
            if (!yaVisited) {
                reverseEdges.insert(i);
                reverseGraph.insertNode(*iterEdge, reverseEdges);
                visited.push_back(*iterEdge);
            }
            else
                reverseGraph.insertEdge(*iterEdge,i);
            iterEdge++;
        }
    }
    //catch any nodes that are sinks
    if (reverseGraph.getLength() != rules.size()) {
        for (unsigned int i = 0; i < rules.size(); i++) {
            std::set<int> reverseEdges;
            if (i >= reverseGraph.getLength() || reverseGraph.getNode(i) != i)
                reverseGraph.insertNode(i, reverseEdges);
        }
    }
}
std::vector<int> Interpreter::DFS(int node) {
    //return the post-order
    reverseGraph.markNode(node, true);
    std::set<int> edges = reverseGraph.getEdgesOfNode(node);
    auto iterEdges = edges.begin();
    for (unsigned int i = 0; i < edges.size(); i++) {
        if (!reverseGraph.checkVisited(*iterEdges)) {
            DFS(*iterEdges);
            //std::cout << std::to_string(node) << ": " << graph.printPostOrder();
        }
        iterEdges++;
    }
    reverseGraph.pushBack(node);
    return reverseGraph.getPostOrder();
}
std::set<int> Interpreter::DFS(int node, int countSCC) {
    //returns the search tree
    graph.markNode(node, true);
    std::set<int> edges = graph.getEdgesOfNode(node);
    auto iterEdges = edges.begin();
    for (unsigned int i = 0; i < edges.size(); i++) {
        if (!graph.checkVisited(*iterEdges)) {
            DFS(*iterEdges, countSCC);
        }
        iterEdges++;
    }
    graph.insertElement(countSCC,node);
    return graph.getSCC(countSCC);
}
std::vector<int> Interpreter::dfsForest() {
    reverseGraph.resetVisits();
    for (unsigned int i = 0; i < reverseGraph.getLength(); i++) {
        if (!reverseGraph.checkVisited(i))
            DFS(i);
    }
    return reverseGraph.getPostOrder();
}
std::vector<std::set<int>> Interpreter::dfsForest(bool searchForest) {
    //reverse the post-order of the reverse graph
    std::vector<int> runOrder;
    unsigned int index = reverseGraph.getPostOrder().size();
    while (index > 0) {
        index--;
        runOrder.push_back(reverseGraph.getPostOrder().at(index));
    }
    graph.resetVisits();
    int countSCC = 0;
    for (int i : runOrder) {
        if (!graph.checkVisited(i)) {
            std::set<int> newSCC;
            graph.insertSCC(newSCC);
            newSCC = DFS(i, countSCC);
            if (!newSCC.empty())
                countSCC++;
            else
                graph.deleteSCC();
        }
    }
    return graph.getAllSCC();
}

std::string Interpreter::toString() {
    std::vector<Rule> printRules = datalog.getRules();
    std::vector<Predicate> printQueries = datalog.getQueries();
    std::string output;
    output += "\nQuery Evaluation\n";
    for (unsigned int i = 0; i < printQueries.size(); i++) {
        //for each query, print the header
        output += printQueries[i].toString() + "? ";
        Relation* eachQuery = evaluateQueries[i];
        std::vector<int> eachOutputIndex = outputIndex[eachQuery->getName()];
        //print success/the number of results
        unsigned int numEval = eachQuery->getTuples().size();
        if (numEval == 0)
            output += "No\n";
        else {
            output += "Yes(" + std::to_string(numEval) + ")\n";
            bool isVarParam = false;
            Header searchHeader = eachQuery->getHeader();
            std::set<Tuple> printTuples = eachQuery->getTuples();
            if (!printTuples.empty()) {
                for (const Tuple& searchTuple: printTuples) {
                    output += "  ";
                    std::vector<std::string> preventDuplicates;
                    int countDup = 0;
                    for (unsigned int j = 0; j < searchHeader.getLength(); j++) {
                        //check each tuple in the query I'm looking at for a variable
                        if (searchHeader.getAttributeAt(j).substr(0, 1) != "'") {
                            isVarParam = true;
                            //make sure you're not printing a duplicate. This should probably be in the select algorithm, but it's here haha
                            preventDuplicates.push_back(searchHeader.getAttributeAt(j));
                            for (auto & preventDuplicate : preventDuplicates) {
                                if (preventDuplicate == searchHeader.getAttributeAt(j))
                                    countDup++;
                            }
                            //if there's not a duplicate, and it's not a constant, print the tuple
                            if (countDup == 1)
                                output += searchHeader.getAttributeAt(j) + "=" + searchTuple.getValueAt(j) + ", ";
                        }
                        countDup = 0;
                    }
                    //fix extra comma
                    output = output.substr(0, output.length() - 2);
                    //conditional new line
                    if (isVarParam)
                        output += "\n";
                }
            }
        }
    }
    return output;
}