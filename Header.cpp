#include "Header.h"

void Header::setAttributes(std::vector<std::string> attributes) {
    this->attributes = attributes;
}
void Header::addAttribute(const std::string& value) {
    attributes.push_back(value);
}

unsigned int Header::getLength() {
    return attributes.size();
}
std::vector<std::string> Header::getAttributes() const {
    return attributes;
}
std::string Header::getAttributeAt(unsigned int index) {
    return attributes[index];
}



