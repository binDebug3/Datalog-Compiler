#include "Parameter.h"

Parameter::Parameter(std::string unit) {
    this->unit = unit;
}
Parameter::~Parameter() {}

std::string Parameter::toString() {
    return unit;
}

