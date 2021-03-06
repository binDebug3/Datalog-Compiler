#ifndef RELACION_H
#define RELACION_H

#include "Tuple.h"
#include "Header.h"
#include <string>
#include <set>

class Relation {
private:
    std::string name;
    Header header;
    std::set<Tuple> tuples;

    std::vector<int> sendFirstHeader;
    std::vector<int> sendSecondHeader;

    Header combineHeaders(int decision, Header headerWith);
    int isJoinable(Header first, Header second); //receives two tuples and information about where they should match
    std::set<Tuple> intersect(const std::set<Tuple>& tuplesWith);
    std::set<Tuple> crossProduct(const std::set<Tuple>& tuplesWith);
    Tuple combineTuples(const Tuple& tupleThis, const Tuple& tupleWith); //receives two tuples and information about where they should match
public:
    void setName(std::string name);
    void setHeader(Header header);
    void addTuples(const Tuple& tuple);
    void setTuples(std::set<Tuple> tuples);

    std::string getName() const;
    std::set<Tuple> getTuples() const;
    std::vector<Tuple> getVectorOfTuples() const;
    //Tuple getTupleAt(int index) const;
    Header getHeader() const;
    //int getLength() const;

    Relation* select(unsigned int index, const std::string& value);
    Relation* select(unsigned int index, unsigned int otherIndex);
    Relation* project(std::vector<int> indices);
    Relation* rename(std::vector<std::string> attributes);
    Relation* join(Relation* joinWith);
    bool unionRelations(Relation* unionWith);

    std::string toString();
};

#endif //RELACION_H
