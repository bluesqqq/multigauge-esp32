#pragma once

#include "graphics/gauge/Element.h"
#include "graphics/colors/Color.h"

class TextElement : public Element {
    private:
        std::string text;
        float point;
        std::unique_ptr<Color> color;

        Anchor anchor = Anchor::TopLeft;
        bool useEllipses = false;
        bool useHyphens = false;

    public:
        explicit TextElement(YGConfigRef config, std::string text, float point, std::unique_ptr<Color> color);

        TextElement(YGConfigRef config, const rapidjson::Value::ConstObject json);
        
        void draw(Graphics& g) const override;
};
