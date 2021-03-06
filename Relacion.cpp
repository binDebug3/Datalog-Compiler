#include "Relacion.h"
#include <iostream>

void Relation::setName(std::string name) {
    this->name = name;
}
void Relation::setHeader(Header header) {
    this->header = header;
}
void Relation::addTuples(const Tuple& tuple) {
    tuples.insert(tuple);
}
void Relation::setTuples(std::set<Tuple> tuples) {
    this->tuples = tuples;
}

std::string Relation::getName() const {
    return name;
}
std::set<Tuple> Relation::getTuples() const {
    return tuples;
}
std::vector<Tuple> Relation::getVectorOfTuples() const {
    std::vector<Tuple> sendTuples;
    auto iterTuple = tuples.begin();
    for(Tuple tempTuple : this->tuples) {
        sendTuples.push_back(*iterTuple);
        iterTuple++;
    }
    return sendTuples;
}
Header Relation::getHeader() const {
    return header;
}


Relation* Relation::select(unsigned int index, const std::string& value) {
    auto* createRelation = new Relation();
    createRelation->setName(name);
    createRelation->setHeader(header);
    for (const Tuple& checkThis : tuples) {
        if (checkThis.getValueAt(index) == value) {
            Tuple createTuple;
            createTuple.setValues(checkThis.getValues());
            createRelation->addTuples(createTuple);
        }
    }
    return createRelation;
}
Relation* Relation::select(unsigned int index, unsigned int otherIndex) {
    auto* createRelation = new Relation();
    createRelation->setName(name);
    createRelation->setHeader(header);
    for (const Tuple& checkThis : tuples) {
        if (checkThis.getValueAt(index) == checkThis.getValueAt(otherIndex)) {
            Tuple createTuple;
            createTuple.setValues(checkThis.getValues());
            createRelation->addTuples(createTuple);
        }
    }
    //prevent duplicates
    std::vector<int> yaIndex;
    std::vector<std::string> yaFound;
    bool found;
    for(unsigned int i=0; i<header.getLength(); i++) {
        for (auto & j : yaFound) {
            if (j == header.getAttributeAt(i))
                found = true;
        }
        if (!found) {
            yaFound.push_back(header.getAttributeAt(i));
            yaIndex.push_back(i);
        }
        found = false;
    }
    return createRelation;
}
Relation* Relation::project(std::vector<int> indices) {
    auto* createRelation = new Relation();
    createRelation->setName(name);
    Header createHeader;
    std::vector<Tuple> createTuples;
    //build the new header
    for (int indice : indices) {
        createHeader.addAttribute(header.getAttributeAt(indice));
    }
    //modify the vector. This is the project part
    std::vector<std::string> redoTuple;
    std::vector<int> tupleIndex;
    std::vector<int> toRemoveIndex;
    auto iterTup = tuples.begin();
    for (unsigned int tupleIndex=0; tupleIndex < tuples.size(); tupleIndex++) {
        Tuple tempTuple;
        createTuples.push_back(tempTuple);
        for (unsigned int keepIndex=0; keepIndex < indices.size(); keepIndex++) {
            for (int attributeIndex=0; attributeIndex < iterTup->getLength(); attributeIndex++) {
                //TODO signed vs unsigned comparison
                if (attributeIndex == indices[keepIndex]) {
                    createTuples.at(tupleIndex).addValue(iterTup->getValueAt(attributeIndex));
                }
            }
        }
        iterTup++;
    }
    //update the header
    createRelation->setHeader(createHeader);
    //convert the vector to a set and update the tuples
    std::set<Tuple> sendNewTuples;
    for (auto & createTuple : createTuples) {
        sendNewTuples.insert(createTuple);
    }
    createRelation->setTuples(sendNewTuples);
    return createRelation;
}
Relation* Relation::rename(std::vector<std::string> attributes) {
    auto* createRelation = new Relation();
    createRelation->setName(name);
    Header createHeader;
    createHeader.setAttributes(attributes);
    createRelation->setHeader(createHeader);
    createRelation->setTuples(tuples);
    return createRelation;
}
int Relation::isJoinable(Header first, Header second) {
    std::vector<int> firstIndices;
    std::vector<int> secondIndices;
    for (unsigned int i=0; i<first.getLength(); i++) {
        for (unsigned int j = 0; j < second.getLength(); j++) {
            if (first.getAttributeAt(i) == second.getAttributeAt(j)) {
                firstIndices.push_back(i);
                secondIndices.push_back(j);
            }
        }
    }
    for (int & firstIndice : firstIndices) {
        sendFirstHeader.push_back(firstIndice);
    }
    for (int & secondIndice : secondIndices) {
        sendSecondHeader.push_back(secondIndice);
    }
    if (firstIndices.size() == first.getLength()) {
        return 1; //intersect
    }
    else if (firstIndices.empty()) {
        return 2; //cross product
    }
    else {
        return 3;
    }
}
Header Relation::combineHeaders(int decision, Header headerWith) {
    bool insertAtt;
    Header newHeader;
    std::vector<std::string> buildAttributes = this->header.getAttributes();
    if (decision == 2) { //cross product
        for (unsigned int i=0; i<headerWith.getLength(); i++) {
            buildAttributes.push_back(headerWith.getAttributeAt(i));
        }
    }
    else if (decision == 3){ //natural join
        for (unsigned int i = 0; i < headerWith.getLength(); i++) {
            insertAtt = false;
            for (unsigned int j : sendSecondHeader) {
                if (i == j)
                    insertAtt = true;
            }
            if (!insertAtt)
                buildAttributes.push_back(headerWith.getAttributeAt(i));
        }
    }
    newHeader.setAttributes(buildAttributes);
    return newHeader;
}
std::set<Tuple> Relation::intersect(const std::set<Tuple>& tuplesWith) {
    int addTupleCount;
    std::set<Tuple> newTuples;
    auto iterThis = this->tuples.begin();
    auto iterWith = tuplesWith.begin();
    for (unsigned int rowT=0; rowT < this->tuples.size(); rowT++) {
        iterWith = tuplesWith.begin();
        for (unsigned int rowJ=0; rowJ < tuplesWith.size(); rowJ++) {
            addTupleCount = 0;
            for (unsigned int colT=0; colT < sendFirstHeader.size(); colT++) {
                if (iterThis->getValueAt(sendFirstHeader[colT]) == iterWith->getValueAt(sendSecondHeader[colT]))
                    addTupleCount++;
                //for (int colJ=0; colJ < tuplesWith.begin()->getLength(); colJ++) {

                //}
            }
            if (addTupleCount == tuples.begin()->getLength())
                newTuples.insert(*iterThis);
            iterWith++;
        }
        iterThis++;
    }
    return newTuples;
}
std::set<Tuple> Relation::crossProduct(const std::set<Tuple>& tuplesWith) {
    std::set<Tuple> newTuples;
    auto iterThis = this->tuples.begin();
    auto iterWith = tuplesWith.begin();
    std::vector<std::string> theseValues;
    std::vector<std::string> withValues;
    for (unsigned int rowT=0; rowT < this->tuples.size(); rowT++) {
        iterWith = tuplesWith.begin();
        for (unsigned int rowJ = 0; rowJ < tuplesWith.size(); rowJ++) {
            std::vector<std::string> newValues = iterThis->getValues();
            for (int col = 0; col < iterWith->getLength(); col++) {
                newValues.push_back(iterWith->getValueAt(col));
            }
            Tuple tempTuple;
            tempTuple.setValues(newValues);
            newTuples.insert(tempTuple);
            iterWith++;
        }
        iterThis++;
    }
    return newTuples;
}
Tuple Relation::combineTuples(const Tuple& tupleThis, const Tuple& tupleWith) {
    std::vector<int> confirmFirst;
    std::vector<int> confirmSecond;
    Tuple combinedTuple;
    std::vector<std::string> combinedValues;
    //get a list of where the attributes match
    for (int j = 0; j < tupleWith.getLength(); j++) {
        for (int i = 0; i < tupleThis.getLength(); i++) {
            if (tupleThis.getValueAt(i) == tupleWith.getValueAt(j)) {
                confirmFirst.push_back(i);
                confirmSecond.push_back(j);
            }
        }
    }
    bool firstMatches = false;
    bool secondMatches = false;
    bool confirmsNotEqual = false;
    //compare matching indices of the headers to the tuples
    for (unsigned int i = 0; i < confirmFirst.size(); i++)
        if (tupleThis.getValueAt(confirmFirst.at(i)) != tupleWith.getValueAt(confirmSecond.at(i)))
            confirmsNotEqual = true;
    if (sendFirstHeader.size() != tupleThis.getLength(1))
        confirmsNotEqual = true;
    if (confirmsNotEqual) {
        firstMatches = true;
        secondMatches = true;
        for (unsigned int j = 0; j < sendFirstHeader.size(); j++) {
            if (tupleThis.getValueAt(sendFirstHeader.at(j)) != tupleWith.getValueAt(sendSecondHeader.at(j))) {
                firstMatches = false;
                secondMatches = false;
            }
        }
    }
    //create new vector of indices not in confirmSecond
    if (firstMatches && secondMatches) {
        std::vector<int> tupleWithIndex;
        std::vector<unsigned int> toAddIndex;
        for (int a=0; a < tupleWith.getLength(); a++) {
            tupleWithIndex.push_back(a);
        }
        for (unsigned int a = 0; a < tupleWithIndex.size(); a++) {
            bool toRemove = true;
            for (unsigned int b : sendSecondHeader) {
                if (a == b)
                    toRemove = false;
            }
            if (toRemove)
                toAddIndex.push_back(a);
        }
        //build new tuple
        for (int i = 0; i < tupleThis.getLength(); i++) {
            combinedValues.push_back(tupleThis.getValueAt(i));
        }
        for (unsigned int i : toAddIndex) {
            combinedValues.push_back(tupleWith.getValueAt(i));
        }
    }
    combinedTuple.setValues(combinedValues);
    return combinedTuple;
}
//TODO test join
Relation* Relation::join(Relation* joinWith) {
    std::set<Tuple> newTuples;
    std::vector<int> natJoinIndex;
    std::set<Tuple> tuplesWith = joinWith->getTuples();
    auto* joinedRelation = new Relation();

    //compare headers and make a new header with the correct attributes
    int decision = isJoinable(this->getHeader(), joinWith->getHeader());
    Header newHeader = combineHeaders(decision, joinWith->getHeader());
    joinedRelation->setName(this->name);
    joinedRelation->setHeader(newHeader);

    if (decision == 1) {//intersect

        newTuples = intersect(tuplesWith);
    }
    else if (decision == 2) //cross product
        newTuples = crossProduct(tuplesWith);
    else { //natural join
        auto iterThis = this->tuples.begin();
        auto iterWith = tuplesWith.begin();
        for (unsigned int i = 0; i < this->tuples.size(); i++) {
            iterWith = tuplesWith.begin();
            for (unsigned int j = 0; j < tuplesWith.size(); j++) {
                Tuple tempTuple = combineTuples(*iterThis, *iterWith);
                if (tempTuple.getLength() > 0)
                    newTuples.insert(tempTuple);
                iterWith++;
            }
            iterThis++;
        }
    }
    joinedRelation->setTuples(newTuples);
    return joinedRelation;
}
//TODO test union relations
bool Relation::unionRelations(Relation* unionWith) {
    bool relationsMatch = true;
    bool unionAdded = false;
    for (unsigned int j = 0; j < this->header.getLength(); j++)
        if (j < unionWith->getHeader().getLength() && this->header.getAttributeAt(j) != unionWith->getHeader().getAttributeAt(j))
            relationsMatch = false;
    if (relationsMatch) {
        std::vector<Tuple> vectorTuples = unionWith->getVectorOfTuples();
        for (auto & vectorTuple : vectorTuples) {
            if (this->tuples.insert(vectorTuple).second) {
                unionAdded = true;
                //print out the tuple as something new that was added
                std::cout << vectorTuple.toString(unionWith->getHeader());
            }
        }
    }
    return unionAdded;
}


std::string Relation::toString() {
    std::string output;
    for (const Tuple& t : tuples) {
        output += "  ";
        for (unsigned int i=0; i<header.getLength(); i++) {
            output += header.getAttributeAt(i) + "=" + t.getValueAt(i) + ", ";
        }
        output = output.substr(0, output.length()-2);
        output += "\n";
    }
    return output;
}