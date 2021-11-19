#ifndef TUPLE_H
#define TUPLE_H

#include <string>
#include <vector>
#include "Header.h"

class Tuple {
private:
    std::vector<std::string> values;
public:
    bool operator< (const Tuple&rhs) const {
        return values < rhs.values;
    }
    Tuple();

    void addValue(const std::string& value);
    void setValues(std::vector<std::string> values);
    void setValueAt(int index, std::string value);
    std::string getValueAt(unsigned int index) const;
    std::vector<std::string> getValues() const;
    unsigned int getLength() const;
    std::string toString(Header header) const;
};

#endif //TUPLE_H
