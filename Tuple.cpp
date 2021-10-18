#include "Tuple.h"

Tuple::Tuple() {}

void Tuple::addValue(std::string value) {
    values.push_back(value);
}
void Tuple::setValues(std::vector<std::string> values) {
    this->values = values;
}
void Tuple::setValueAt(int index, std::string value) {
    values[index] = value;
}

std::string Tuple::getValueAt(int index) const {
    return values[index];
}
std::vector<std::string> Tuple::getValues() const {
    return values;
}

