#pragma once

#include "graphics/gauge/Element.h"
#include "graphics/colors/Color.h"

class TextElement : public Element {
    private:
        std::string text;
        float point;
        Color* color;

        Anchor anchor = Anchor::TopLeft;
        bool useEllipses = false;
        bool useHyphens = false;

    public:
        explicit TextElement(YGConfigRef config, std::string text, float point, Color* color) : Element(config), text(text), point(point), color(color) {}

        void draw(Graphics& g) const override {
            const auto& b = getBounds();

            g.setFill(color);
            g.drawTextArea(text, b.toInt(), anchor, useEllipses, useHyphens);

            Element::draw(g);
        }
};
