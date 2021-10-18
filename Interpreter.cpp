#include "Interpreter.h"
#include "Tuple.h"
#include "Parameter.h"
#include <map>
#include <iostream>

Interpreter::Interpreter(const DatalogProgram& datalog) {
    this->datalog = datalog;
    buildSchemes(datalog.getSchemes());
    buildFacts(datalog.getFacts());
    buildQueries(datalog.getQueries());
    //std:: cout << database.toString() << std::endl << std::endl << "Start Project 3: " << std::endl;
}
Interpreter::~Interpreter() {}

void Interpreter::buildSchemes(std::vector<Predicate> inputSchemes) {
    for (unsigned int i=0; i<inputSchemes.size(); i++) {
        Relation* scheme = new Relation();
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
        for (unsigned int j=0; j<facts[i].getParameters().size(); j++) {
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
//void Interpreter::buildRules(std::vector<Rule> rules) {}

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
            int index1 = -1, index2 = -1;
            for (unsigned int i = 0; i < yaFound.size(); i++) {
                if (yaFound[i] == duplicateFoundAt) {
                    if (index1 == -1)
                        index1 = i;
                    else
                        index2 = i;
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

std::string Interpreter::toString() {
    std::vector<Predicate> printQueries = datalog.getQueries();
    std::string output;
    for (unsigned int i = 0; i < printQueries.size(); i++) {
        //for each query, print the header
        output += printQueries[i].toString() + "? ";
        Relation* eachQuery = evaluateQueries[i];
        std::vector<int> eachOutputIndex = outputIndex[eachQuery->getName()];
        //print success/the number of results
        int numEval = eachQuery->getTuples().size();
        if (numEval == 0)
            output += "No\n";
        else {
            output += "Yes(" + std::to_string(numEval) + ")\n";
            bool isVarParam = false;
            Header searchHeader = eachQuery->getHeader();
            std::set<Tuple> printTuples = eachQuery->getTuples();
            //I'm not sure this check is necessary but it's still there
            if (printTuples.size() > 0) {
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