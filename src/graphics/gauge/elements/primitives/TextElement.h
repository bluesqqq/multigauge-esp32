#pragma once

#include "graphics/gauge/Element.h"
#include "graphics/colors/Color.h"
#include "values/ValueEmbed.h"

class TextElement : public Element {
    private:
        std::string text;
        TextStyle textStyle;

        Anchor anchor = Anchor::TopLeft;
        bool useEllipses = false;
        bool useHyphens = false;

    public:
        explicit TextElement(YGConfigRef config, std::string text);

        TextElement(YGConfigRef config, const rapidjson::Value::ConstObject json);
        
        void draw(Graphics& g) const override;
};
