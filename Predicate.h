#ifndef PREDICATE_H
#define PREDICATE_H

#include <vector>
#include "Token.h"
#include "Parameter.h"

class Predicate {
private:
    std::string name;
    std::vector<Parameter*> parameters;

public:
    Predicate(std::string name, std::vector<Parameter*> parameters);
    ~Predicate();

    std::string toString();
};

#endif //PREDICATE_H
