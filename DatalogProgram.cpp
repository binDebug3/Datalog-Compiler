#include "DatalogProgram.h"

DatalogProgram::DatalogProgram() {};
DatalogProgram::~DatalogProgram() {};

void DatalogProgram::addScheme(Predicate createScheme) {
    schemes.push_back(createScheme);
}
void DatalogProgram::addFact(Predicate createFact) {
    facts.push_back(createFact);
}
void DatalogProgram::addQuery(Predicate createQuery) {
    queries.push_back(createQuery);
}
void DatalogProgram::addRule(Rule createRule) {
    rules.push_back(createRule);
}
void DatalogProgram::addDomain(std::string createDomain) {
    domain.insert(createDomain);
}

std::vector<Predicate> DatalogProgram::getSchemes() const {
    return schemes;
}
std::vector<Predicate> DatalogProgram::getFacts() const {
    return facts;
}
//std::vector<Rule> DatalogProgram::getRules() const {
//    return rules;
//}
std::vector<Predicate> DatalogProgram::getQueries() const {
    return queries;
}

std::string DatalogProgram::toString() {
    std::string outputString = "";
    outputString += "Schemes(" + std::to_string(schemes.size()) + "):\n";
    for (unsigned int i=0; i<schemes.size(); i++) {
        outputString += "  " + schemes.at(i).toString() + "\n";
    }
    outputString += "Facts(" + std::to_string(facts.size()) + "):\n";
    for (unsigned int i=0; i<facts.size(); i++) {
        outputString += "  " + facts.at(i).toString() + ".\n";
    }
    outputString += "Rules(" + std::to_string(rules.size()) + "):\n";
    for (unsigned int i=0; i<rules.size(); i++) {
        outputString += "  " + rules.at(i).toString() + "\n";
    }
    outputString += "Queries(" + std::to_string(queries.size()) + "):\n";
    for (unsigned int i=0; i<queries.size(); i++) {
        outputString += "  " + queries.at(i).toString() + "?\n";
    }
    outputString += "Domain(" + std::to_string(domain.size()) + "):\n";
    std::set<std::string>::iterator it=domain.begin();
    for (unsigned int i=0; i<domain.size(); i++) {
        outputString += "  " + *it + "\n";
        std::advance(it, 1);
    }
    return outputString;
}


