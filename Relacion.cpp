#include "Relacion.h"

void Relation::setName(std::string name) {
    this->name = name;
}
void Relation::setHeader(Header header) {
    this->header = header;
}
void Relation::addTuples(Tuple tuple) {
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


Relation* Relation::select(int index, std::string value) {
    Relation* createRelation = new Relation();
    createRelation->setName(name);
    createRelation->setHeader(header);
    for (Tuple checkThis : tuples) {
        if (checkThis.getValueAt(index) == value) {
            Tuple createTuple;
            createTuple.setValues(checkThis.getValues());
            createRelation->addTuples(createTuple);
        }
    }
    return createRelation;
    //need to delete the new relation pointers?
}
Relation* Relation::select(int index, int otherIndex) {
    Relation* createRelation = new Relation();
    createRelation->setName(name);
    createRelation->setHeader(header);
    for (Tuple checkThis : tuples) {
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
        for (unsigned int j = 0; j < yaFound.size(); j++) {
            if (yaFound[j] == header.getAttributeAt(i))
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
    Relation* createRelation = new Relation();
    createRelation->setName(name);
    Header createHeader;
    std::vector<Tuple> createTuples;
    //create a copy of tuples as a vector
    for (Tuple insertTuple : tuples) {
        createTuples.push_back(insertTuple);
    }
    //build the new header
    for (unsigned int i=0; i<indices.size(); i++) {
        createHeader.addAttribute(header.getAttributeAt(indices[i]));
    }
    //modify the vector. This is the project part
    std::set<Tuple>::iterator iterOrig = tuples.begin();
    std::vector<std::string> redoTuple;
    for (unsigned int j=0; j<createTuples.size(); j++) {
        for (unsigned int i=0; i<indices.size(); i++) {
            createTuples[j].setValueAt(i, iterOrig->getValueAt(indices[i]));
        }
        iterOrig++;
    }
    //update the header
    createRelation->setHeader(createHeader);
    //convert the vector to a set and update the tuples
    std::set<Tuple> sendNewTuples;
    for (unsigned int i=0; i<createTuples.size(); i++) {
        sendNewTuples.insert(createTuples[i]);
    }
    createRelation->setTuples(sendNewTuples);
    return createRelation;
}
Relation* Relation::rename(std::vector<std::string> attributes) {
    Relation* createRelation = new Relation();
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
    if (firstIndices.size() == secondIndices.size()) {
        return 1; //intersect
    }
    else if (firstIndices.size() == 0) {
        return 2; //cross product
    }
    else {
        for (unsigned int i=0; i < firstIndices.size(); i++) {
            sendFirstHeader.push_back(firstIndices.at(i));
        }
        for (unsigned int i=0; i < secondIndices.size(); i++) {
            sendSecondHeader.push_back(secondIndices.at(i));
        }
        return 3;
    }
}

Header Relation::combineHeaders(int decision, Header headerWith) {
    bool insertAtt = false;
    Header newHeader;
    std::vector<std::string> buildAttributes = this->header.getAttributes();
    if (decision == 2) { //cross product
        for (unsigned int i=0; i<headerWith.getLength(); i++) {
            buildAttributes.push_back(headerWith.getAttributeAt(i));
        }
    }
    else if (decision == 3){ //natural join
        for (unsigned int i=0; i<headerWith.getLength(); i++) {
            insertAtt = false;
            for (unsigned int j=0; j<sendSecondHeader.size(); j++) {
                if (i == sendSecondHeader.at(j))
                    insertAtt = true;
            }
            if (!insertAtt)
                buildAttributes.push_back(headerWith.getAttributeAt(i));
        }
    }
    newHeader.setAttributes(buildAttributes);
    return newHeader;
}

std::set<Tuple> Relation::intersect(std::set<Tuple> tuplesWith) {
    bool addTuple = false;
    std::set<Tuple> newTuples;
    auto iterThis = this->tuples.begin();
    auto iterWith = tuplesWith.begin();
    for (unsigned int rowT=0; rowT < this->tuples.size(); rowT++) {
        iterWith = tuplesWith.begin();
        for (unsigned int rowJ=0; rowJ < tuplesWith.size(); rowJ++) {
            addTuple = true;
            for (unsigned int col=0; col < this->tuples.begin()->getLength(); col++) {
                if (iterThis->getValueAt(col) != iterWith->getValueAt(col))
                    addTuple = false;
            }
            if (addTuple)
                newTuples.insert(*iterThis);
            iterWith++;
        }
        iterThis++;
    }
    return newTuples;
}

std::set<Tuple> Relation::crossProduct(std::set<Tuple> tuplesWith) {
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

Tuple Relation::combineTuples(Tuple tupleThis, Tuple tupleWith) {
    std::vector<int> confirmFirst;
    std::vector<int> confirmSecond;
    std::vector<int> barberSecond;
    Tuple combinedTuple;
    std::vector<std::string> combinedValues;
    //get a list of where the attributes match
    for (unsigned int i = 0; i < tupleThis.getLength(); i++) {
        for (unsigned int j = 0; j < tupleWith.getLength(); j++) {
            if (tupleThis.getValueAt(i) == tupleWith.getValueAt(j)) {
                confirmFirst.push_back(i);
                confirmSecond.push_back(j);
            }
        }
    }
    bool firstMatches = true;
    bool secondMatches = true;
    //compare matching indices of the headers to the tuples
    for (unsigned int i = 0; i < confirmFirst.size(); i++)
        if (i < sendFirstHeader.size() && confirmFirst.at(i) != sendFirstHeader.at(i))
            firstMatches = false;
    for (unsigned int j = 0; j < confirmSecond.size(); j++)
        if (j < sendSecondHeader.size() && confirmSecond.at(j) != sendSecondHeader.at(j))
            secondMatches = false;
    //create new vector of indices not in confirmSecond
    if (firstMatches && secondMatches) {
        for (unsigned int j = 1; j < confirmSecond.size(); j++) {
            int difference = confirmSecond.at(j) - confirmSecond.at(j - 1);
            if (difference != 1) {
                while (difference != 1) {
                    barberSecond.push_back(confirmSecond.at(j) - difference + 1);
                    difference--;
                }
            }
        }
        //build new tuple
        for (unsigned int i = 0; i < tupleThis.getLength(); i++) {
            combinedValues.push_back(tupleThis.getValueAt(i));
        }
        for (unsigned int i = 0; i < barberSecond.size(); i++) {
            combinedValues.push_back(tupleWith.getValueAt(barberSecond.at(i)));
        }
    }
    combinedTuple.setValues(combinedValues);
    return combinedTuple;
}

Relation* Relation::join(Relation* joinWith) {
    std::set<Tuple> newTuples;
    std::vector<int> natJoinIndex;
    std::set<Tuple> tuplesWith = joinWith->getTuples();
    Relation* joinedRelation = new Relation();

    //compare headers and make a new header with the correct attributes
    int decision = isJoinable(this->getHeader(), joinWith->getHeader());
    Header newHeader = combineHeaders(decision, joinWith->getHeader());
    joinedRelation->setName(this->name);
    joinedRelation->setHeader(newHeader);

    if (decision == 1) //intersect
        newTuples = intersect(tuplesWith);
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

std::string Relation::toString() {
    std::string output;
    for (Tuple t : tuples) {
        output += "  ";
        for (int i=0; i<header.getLength(); i++) {
            output += header.getAttributeAt(i) + "=" + t.getValueAt(i) + ", ";
        }
        output = output.substr(0, output.length()-2);
        output += "\n";
    }
    return output;
}
