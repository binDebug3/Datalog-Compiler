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
