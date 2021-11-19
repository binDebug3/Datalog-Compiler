#ifndef HEADER_H
#define HEADER_H

#include <string>
#include <vector>

class Header {
private:
    std::vector<std::string> attributes;
public:
    void setAttributes(std::vector<std::string> attributes);
    void addAttribute(const std::string& value);
    std::vector<std::string> getAttributes() const;
    std::string getAttributeAt(unsigned int index);
    unsigned int getLength();

};

#endif //HEADER_H
