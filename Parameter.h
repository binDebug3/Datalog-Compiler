#ifndef PARAMETER_H
#define PARAMETER_H

#include <vector>
#include "Token.h"

class Parameter {
private:
    std::string unit;

public:
    Parameter(std::string unit);
    ~Parameter();

    void setUnit(std::string newUnit) {
        unit = newUnit;
    }
    std::string toString();

};

#endif //PARAMETER_H
