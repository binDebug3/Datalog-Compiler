#include "Parameter.h"

Parameter::Parameter(std::string unit) {
    this->unit = unit;
}
Parameter::~Parameter() {}

bool Parameter::isConstant() const {
    if (unit.substr(0,1) == "'")
        return true;
    return false;
}
std::string Parameter::toString() {
    return unit;
}

