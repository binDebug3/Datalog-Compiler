#include "Tuple.h"

Tuple::Tuple() = default;

void Tuple::addValue(const std::string& value) {
    values.push_back(value);
}
void Tuple::setValues(std::vector<std::string> values) {
    this->values = values;
}
void Tuple::setValueAt(int index, std::string value) {
    values[index] = value;
}

std::string Tuple::getValueAt(unsigned int index) const {
    return values[index];
}
std::vector<std::string> Tuple::getValues() const {
    return values;
}
int Tuple::getLength() const {
    return values.size();
}
unsigned int Tuple::getLength(int i) const {
    return values.size();
}
std::string Tuple::toString(Header header) const {
    std::string output;
    output += "  ";
    for (unsigned int i=0; i<header.getLength(); i++) {
        output += header.getAttributeAt(i) + "=" + values.at(i) + ", ";
    }
    output = output.substr(0, output.length()-2);
    output += "\n";
    return output;
}


