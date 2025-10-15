#include "GroupElement.h"

GroupElement::GroupElement() { }

void GroupElement::addElement(std::unique_ptr<GaugeElement> element) {
    elements.push_back(std::move(element));
}

void GroupElement::draw(Graphics &g, Rectangle<int> bounds) const {
    int numElements = elements.size();

    bounds.reduce(margin);

    g.setStroke(0xFFFF);
    g.strokeRect(bounds);

    if (numElements == 0) return;

    bounds.reduce(padding);

    int height = bounds.height / numElements;

    for (int i = 0; i < numElements; i++) {
        // Right now, this just evenly splits up the space by the number of items
        const Rectangle<int> elementBounds(bounds.getLeft(), bounds.getTop() + (height * i), bounds.width, height);
        elements[i]->draw(g, elementBounds);
    }
}
