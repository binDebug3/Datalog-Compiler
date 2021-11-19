#include "Predicate.h"

Predicate::Predicate(std::string name, std::vector<Parameter*> parameters) {
    this->name = name;
    this->parameters = parameters;
}
Predicate::~Predicate() {}

std::string Predicate::getName() {
    return name;
}
std::vector<std::string> Predicate::getParameters() const {
    std::vector<std::string> outputParameters;
    for (unsigned int i=0; i<parameters.size(); i++) {
        outputParameters.push_back(parameters[i]->toString());
    }
    return outputParameters;
}
std::vector<Parameter*> Predicate::getParameters(bool asParameters) const {
    return parameters;
}
std::string Predicate::getPofIndex(int index) {
    return parameters.at(index)->toString();
}
unsigned int Predicate::getLength() const {
    return parameters.size();
}

std::string Predicate::toString() {
    std::string output = "";
    output += name + "(";
    for (unsigned int i=0; i<parameters.size()-1; i++) {
        output += parameters[i]->toString() + ",";
    }
    output += parameters[parameters.size()-1]->toString() + ")";
    return output;
}
