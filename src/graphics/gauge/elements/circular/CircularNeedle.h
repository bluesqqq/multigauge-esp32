#pragma once

#include "graphics/gauge/Element.h"

class CircularNeedle : public Element {
    private:
        FillStroke color;
        float radius = 1.0f;

    public:
        explicit CircularNeedle(YGConfigRef config);

        CircularNeedle(YGConfigRef config, const rapidjson::Value::ConstObject json);

        void draw(Graphics& g) const override;
};
