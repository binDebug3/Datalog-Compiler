#include "Database.h"
#include "Tuple.h"
#include <map>
#include <iostream>

Database::Database() {}
Database::~Database() {
    for(std::pair<std::string, Relation*> element : relationsMap) {
        delete element.second;
    }
}

void Database::addRelation(std::string keyName, Relation* scheme) {
    relationsMap.insert(std::pair<std::string, Relation*>(keyName, scheme));
}
void Database::addDatabaseTuple(std::string keyName, Tuple createTuple) {
    relationsMap.find(keyName)->second->addTuples(createTuple);
}
std::map<std::string, Relation*> Database::checkMap() {
    return relationsMap;
}


std::string Database::toString() {
    std::string output;
    for(std::pair<std::string, Relation*> element : relationsMap) {
        output += element.first + ":\n";
        output += element.second->toString() + "\n";
    }
    return output;
}
