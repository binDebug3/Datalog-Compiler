#ifndef DATABASE_H
#define DATABASE_H

#include <map>
#include "Relacion.h"
#include "Predicate.h"
#include "Rule.h"

class Database {
private:
    std::map<std::string, Relation*> relationsMap;
public:
    Database();
    ~Database();

    void addRelation(const std::string& keyName, Relation* scheme);
    void addDatabaseTuple(const std::string& keyName, const Tuple& tuple);
    std::map<std::string, Relation*> checkMap();

    std::string toString();
};

#endif //DATABASE_H
