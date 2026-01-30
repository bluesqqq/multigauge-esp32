#pragma once

#include "graphics/gauge/Element.h"

class CircleElement : public Element {
    private:
        FillStroke color;

    public:
        explicit CircleElement(YGConfigRef config);

        CircleElement(YGConfigRef config, const rapidjson::Value::ConstObject json);

        void draw(Graphics& g) const override;
};
