#pragma once

#include "graphics/gauge/Element.h"
#include "CircularGroup.h"

class CircularNeedle : public Element {
    private:
        CircularContext elementContext;
        bool inherit = true;

        FillStroke color;
        float radius = 1.0f;

    public:
        explicit CircularNeedle(Element* parent);

        CircularNeedle(Element* parent, const rapidjson::Value::ConstObject json);

        void draw(Graphics& g) const override;
};
