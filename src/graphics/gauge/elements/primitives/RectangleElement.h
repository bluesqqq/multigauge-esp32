#pragma once

#include "graphics/gauge/Element.h"
#include "graphics/colors/Color.h"

class RectangleElement : public Element {
    private:
        FillStroke color;

        float radius = 0.0f;

    public:
        explicit RectangleElement(YGConfigRef config);

        RectangleElement(YGConfigRef config, const rapidjson::Value::ConstObject json);

        void draw(Graphics& g) const override;
};
