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
public:
    void setName(std::string name);
    void setHeader(Header header);
    void addTuples(Tuple tuple);
    void setTuples(std::set<Tuple> tuples);

    std::string getName() const;
    std::set<Tuple> getTuples() const;
    //Tuple getTupleAt(int index) const;
    Header getHeader() const;
    //int getLength() const;

    Relation* select(int index, std::string value);
    Relation* select(int index, int otherIndex);
    Relation* project(std::vector<int> indices);
    Relation* rename(std::vector<std::string> attributes);

    std::string toString();
};

#endif //RELACION_H
