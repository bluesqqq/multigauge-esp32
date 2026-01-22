#pragma once

#include "graphics/gauge/Element.h"

class RectangleElement : public Element {
public:
    explicit RectangleElement(YGConfigRef config) : Element(config) {}

    void draw(Graphics& g) const override {
        const auto& b = getBounds();

        g.setFill(0xf800);
        g.fillRect(b.toInt());

        Element::draw(g);
    }
};
