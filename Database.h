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

    void addRelation(std::string keyName, Relation* scheme);
    void addDatabaseTuple(std::string keyName, Tuple tuple);
    std::map<std::string, Relation*> checkMap();
    //TODO
    void unionWithDatabase(Relation* addRelation);

    std::string toString();
};

#endif //DATABASE_H
