#include "Element.h"

void Element::addChild(std::unique_ptr<Element> child) { 
    child->parent = this;
    children.push_back(std::move(child));
}