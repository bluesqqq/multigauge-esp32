#pragma once

#include "graphics/gauge/Element.h"

class CircleElement : public Element {
public:
    explicit CircleElement(YGConfigRef config) : Element(config) {}

    void draw(Graphics& g) const override {
        const auto& b = getBounds();

        const float w = b.width;
        const float h = b.height;
        const float diameter = std::min(w, h);
        const float radius   = diameter * 0.5f;

        const float cx = b.position.x + w * 0.5f;
        const float cy = b.position.y + h * 0.5f;

        g.setFill(0x00FF00);
        g.fillCircle(cx, cy, radius);

        Element::draw(g);
    }
};
