#ifndef HEADER_H
#define HEADER_H

#include <string>
#include <vector>

class Header {
private:
    std::vector<std::string> attributes;
public:
    void setAttributes(std::vector<std::string> attributes);
    void addAttribute(std::string value);
    std::vector<std::string> getAttributes() const;
    std::string getAttributeAt(int index);
    int getLength();

};

#endif //HEADER_H
