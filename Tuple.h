#ifndef TUPLE_H
#define TUPLE_H

#include <string>
#include <vector>

class Tuple {
private:
    std::vector<std::string> values;
public:
    bool operator< (const Tuple&rhs) const {
        return values < rhs.values;
    }
    Tuple();

    void addValue(std::string value);
    void setValues(std::vector<std::string> values);
    void setValueAt(int index, std::string value);
    std::string getValueAt(int index) const;
    std::vector<std::string> getValues() const;
    int getLength() const;
};

#endif //TUPLE_H
