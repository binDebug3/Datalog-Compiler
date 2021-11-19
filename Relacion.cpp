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

//Tuple Relation::getTupleAt(int index) const {
//    std::set<Tuple>::iterator iter = tuples.begin();
//    //might be the wrong index?
//    for (int i=0; i<=index; i++)
//        iter++;
//    return *iter;
//}
Header Relation::getHeader() const {
    return header;
}
//int Relation::getLength() const {
//    return tuples.size();
//}


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
    //need to delete the new relation pointers?
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
    //trying to prevent duplicates but not working
    std::vector<int> yaIndex;
    std::vector<std::string> yaFound;
    bool found;
    for(int i=0; i<header.getLength(); i++) {
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
    //need to delete the new relation pointers?
}
Relation* Relation::project(std::vector<int> indices) {
    auto* createRelation = new Relation();
    createRelation->setName(name);
    Header createHeader;
    std::vector<Tuple> createTuples;
    //create a copy of tuples as a vector
//    for (Tuple insertTuple : tuples) {
//        createTuples.push_back(insertTuple);
//    }
    //build the new header
    for (int indice : indices) {
        createHeader.addAttribute(header.getAttributeAt(indice));
    }
    //modify the vector. This is the project part
    std::vector<std::string> redoTuple;
    std::vector<int> tupleIndex;
    std::vector<int> toRemoveIndex;
    //create a vector that is the inverse of the indices vector
//    for (int a=0; a<tuples.begin()->getLength(); a++) {
//        tupleIndex.push_back(a);
//    }
//    for (unsigned int a=0; a<tupleIndex.size(); a++) {
//        bool toRemove = true;
//        for (unsigned int b = 0; b < indices.size(); b++) {
//            if (a == indices[b])
//                toRemove = false;
//        }
//        if (toRemove)
//            toRemoveIndex.push_back(a);
//    }
//    for (unsigned int tupleIndex=0; tupleIndex < createTuples.size(); tupleIndex++) {
//        for (int attributeIndex=createTuples.at(tupleIndex).getLength() - 1; attributeIndex >= 0 ; attributeIndex--) {
//            for (unsigned int removeIndex=0; removeIndex < toRemoveIndex.size(); removeIndex++) {
//                if (attributeIndex == toRemoveIndex[removeIndex]) {
//                    createTuples.at(tupleIndex).removeValue(attributeIndex);
//                }
//            }
//        }
//    }
    auto iterTup = tuples.begin();
    for (unsigned int tupleIndex=0; tupleIndex < tuples.size(); tupleIndex++) {
        Tuple tempTuple;
        createTuples.push_back(tempTuple);
        for (unsigned int keepIndex=0; keepIndex < indices.size(); keepIndex++) {
            for (unsigned int attributeIndex=0; attributeIndex < iterTup->getLength(); attributeIndex++) {
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
//TODO test isjoinable
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
    if (firstIndices.size() == first.getLength()) {
        return 1; //intersect
    }
    else if (firstIndices.empty()) {
        return 2; //cross product
    }
    else {
        for (int & firstIndice : firstIndices) {
            sendFirstHeader.push_back(firstIndice);
        }
        for (int & secondIndice : secondIndices) {
            sendSecondHeader.push_back(secondIndice);
        }
        return 3;
    }
}
//TODO test combineHeaders
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
            for (int j : sendSecondHeader) {
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
//TODO test intersect
std::set<Tuple> Relation::intersect(const std::set<Tuple>& tuplesWith) {
    int addTupleCount;
    std::set<Tuple> newTuples;
    auto iterThis = this->tuples.begin();
    auto iterWith = tuplesWith.begin();
    for (unsigned int rowT=0; rowT < this->tuples.size(); rowT++) {
        iterWith = tuplesWith.begin();
        for (unsigned int rowJ=0; rowJ < tuplesWith.size(); rowJ++) {
            addTupleCount = 0;
            for (unsigned int colT=0; colT < this->tuples.begin()->getLength(); colT++) {
                for (unsigned int colJ=0; colJ < tuplesWith.begin()->getLength(); colJ++) {
                    if (iterThis->getValueAt(colT) == iterWith->getValueAt(colJ))
                        addTupleCount++;
                }
            }
            if (addTupleCount == tuples.begin()->getLength())
                newTuples.insert(*iterThis);
            iterWith++;
        }
        iterThis++;
    }
    return newTuples;
}
//TODO test crossProduct
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
            for (unsigned int col = 0; col < iterWith->getLength(); col++) {
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
//TODO test combineTuples
Tuple Relation::combineTuples(const Tuple& tupleThis, const Tuple& tupleWith) {
    std::vector<int> confirmFirst;
    std::vector<int> confirmSecond;
    Tuple combinedTuple;
    std::vector<std::string> combinedValues;
    //get a list of where the attributes match
    for (unsigned int j = 0; j < tupleWith.getLength(); j++) {
        for (unsigned int i = 0; i < tupleThis.getLength(); i++) {
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
    if (sendFirstHeader.size() != tupleThis.getLength())
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
//        for (int i: confirmFirst)
//            for (int j: sendFirstHeader)
//                if (i == j)
//                    firstMatches = true;
//        for (int i: confirmSecond)
//            for (int j: sendSecondHeader)
//                if (i == j)
//                    secondMatches = true;
    }
    //create new vector of indices not in confirmSecond
    if (firstMatches && secondMatches) {
        std::vector<int> tupleWithIndex;
        std::vector<unsigned int> toAddIndex;
        for (unsigned int a=0; a < tupleWith.getLength(); a++) {
            tupleWithIndex.push_back(a);
        }
        for (unsigned int a = 0; a < tupleWithIndex.size(); a++) {
            bool toRemove = true;
            for (int b : sendSecondHeader) {
                if (a == b)
                    toRemove = false;
            }
            if (toRemove)
                toAddIndex.push_back(a);
        }
        /*for (unsigned int i = 0; i < tupleWith.getLength(); i++) {
            bool yaAdded = false;
            for (unsigned int j = 0; j < confirmSecond.size(); j++) {
                if (i == confirmSecond.at(j))
                    yaAdded = true;
            }
            for (unsigned int k = 0; k < barberSecond.size(); k++) {
                if (barberSecond.at(k) == i)
                    yaAdded = true;
            }
            if (!yaAdded) {
               barberSecond.push_back(i);
            }
        }*/
        //build new tuple
        for (unsigned int i = 0; i < tupleThis.getLength(); i++) {
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
