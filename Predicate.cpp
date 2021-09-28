#include "Predicate.h"

Predicate::Predicate(std::string name, std::vector<Parameter*> parameters) {
    this->name = name;
    this->parameters = parameters;
}
Predicate::~Predicate() {}

std::string Predicate::toString() {
    std::string output = "";
    output += name + "(";
    for (unsigned int i=0; i<parameters.size()-1; i++) {
        output += parameters[i]->toString() + ",";
    }
    output += parameters[parameters.size()-1]->toString() + ")";
    return output;
}
