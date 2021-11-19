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
    for (unsigned int i=0; i<inputSchemes.size(); i++) {
        auto scheme = new Relation();
        std::string keyName = inputSchemes[i].getName();
        scheme->setName(keyName);
        Header header;
        header.setAttributes(inputSchemes[i].getParameters());
        scheme->setHeader(header);
        database.addRelation(keyName, scheme);
    }
}
void Interpreter::buildFacts(std::vector<Predicate> facts) {
    for (unsigned int i=0; i<facts.size(); i++) {
        Tuple createTuple;
        for (int j=0; j<facts[i].getParameters().size(); j++) {
            createTuple.addValue(facts[i].getPofIndex(j));
        }
        database.addDatabaseTuple(facts[i].getName(), createTuple);
    }
}
void Interpreter::buildQueries(std::vector<Predicate> queries) {
    for (unsigned int i = 0; i < queries.size(); i++) {
        evaluateQueries.push_back(this->evaluatePredicate(queries[i]));
        //std::cout << evaluateQueries[i] << std::endl;
    }
}
//TODO test buildRules
void Interpreter::buildRules(std::vector<Rule> rules) {
    std::cout << "Rule Evaluation" << std::endl;
    bool somethingChanged = true;
    while (somethingChanged) {
        somethingChanged = false;
        for (unsigned int ruleIndex = 0; ruleIndex < rules.size(); ruleIndex++) {
            //evaluate the predicates on the right-hand side of the rule
            evaluateRules.clear();
            for (int j = 0; j < rules.at(ruleIndex).getLength(); j++) {
                evaluateRules.push_back(this->evaluatePredicate(*rules.at(ruleIndex).getRuleAt(j)));
            }
            std::string output;
            output += rules[ruleIndex].getHead()->toString() + " :- ";
            for (int j = 0; j < rules[ruleIndex].getLength(); j++) {
                output += rules[ruleIndex].getRuleAt(j)->toString() + ",";
            }
            output = output.substr(0, output.length() - 1);
            output += ".\n";
            //std::cout << output;

            //TODO fixed point algorithm
            //join the relations that result
            auto joinedRelations = new Relation();
            //idk if I need to do this but I defined joinedRelations with at least one actual relation explicitly
            joinedRelations->setName(evaluateRules.at(0)->getName());
            joinedRelations->setHeader(evaluateRules.at(0)->getHeader());
            joinedRelations->setTuples(evaluateRules.at(0)->getTuples());
            for (unsigned int i = 1; i < evaluateRules.size(); i++) {
                joinedRelations = joinedRelations->join(evaluateRules.at(i));
            }

            //project the columns that appear in the head predicate
            std::vector<int> projectIndex;
            Header relationHead = joinedRelations->getHeader();
            Predicate *headPred = rules.at(ruleIndex).getHead();
            for (int j = 0; j < headPred->getLength(); j++) {
                for (int i = 0; i < relationHead.getLength(); i++) {
                    if (relationHead.getAttributeAt(i) == headPred->getPofIndex(j)) {
                        projectIndex.push_back(i);
                    }
                }
            }
            //TODO figure out how to prevent project from wrecking initial tuples
            joinedRelations = joinedRelations->project(projectIndex);

            //rename the relations to make it union-compatible (at least make sure the indices are matched up)
            std::string searchKey = rules.at(ruleIndex).getHead()->getName();
            Relation *namingScheme = database.checkMap()[searchKey];
            Header renameHeader = namingScheme->getHeader();
            std::vector<std::string> renameAttributes = renameHeader.getAttributes();
            joinedRelations = joinedRelations->rename(renameAttributes);

            //union with the relation in the database
            bool lastOneChanged = false;
            std::cout << output;
            lastOneChanged = namingScheme->unionRelations(joinedRelations);
            if (lastOneChanged == true)
                somethingChanged = true;
            if (lastOneChanged)
                //output += joinedRelations->toString();
            //TODO don't forget about numPasses
            if (numPasses > 100)
                somethingChanged = false;
        }
        numPasses++;
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
            unsigned int index1 = -1, index2 = -1;
            for (unsigned int ind = 0; ind < yaFound.size(); ind++) {
                if (yaFound[ind] == duplicateFoundAt) {
                    if (index1 == -1)
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
//TODO Rule Evaluation Output
std::string Interpreter::toString() {
    std::vector<Rule> printRules = datalog.getRules();
    std::vector<Predicate> printQueries = datalog.getQueries();
    std::string output;

    output += "\nSchemes populated after " + std::to_string(numPasses) + " passes through the Rules.\n";

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
            //I'm not sure this check is necessary, but it's still there
            if (!printTuples.empty()) {
                for (Tuple searchTuple: printTuples) {
                    output += "  ";
                    std::vector<std::string> preventDuplicates;
                    int countDup = 0;
                    for (int j = 0; j < searchHeader.getLength(); j++) {
                        //check each tuple in the query I'm looking at for a variable
                        if (searchHeader.getAttributeAt(j).substr(0, 1) != "'") {
                            isVarParam = true;
                            //make sure you're not printing a duplicate. This should probably be in the select algorithm but it's here haha
                            preventDuplicates.push_back(searchHeader.getAttributeAt(j));
                            for (unsigned int k = 0; k < preventDuplicates.size(); k++) {
                                if (preventDuplicates[k] == searchHeader.getAttributeAt(j))
                                    countDup++;
                            }
                            //if there's not duplicate and it's not a constant, print the tuple
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