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

    std::string getName();
    std::vector<std::string> getParameters() const;
    std::vector<Parameter*> getParameters(bool asParameters) const;
    std::string getPofIndex(int index);

    std::string toString();
};

#endif //PREDICATE_H
