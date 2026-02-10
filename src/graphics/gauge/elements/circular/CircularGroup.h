#pragma once

#include "graphics/gauge/Element.h"
#include "graphics/DisplayValue.h"

struct CircularContext {
    DisplayValue value = nullptr;
    float startAngle = 0;
    float endAngle = 360;
};

class CircularGroup : public Element {
    private:
        CircularContext elementContext;

    public:
        explicit CircularGroup(Element* parent);

        CircularGroup(Element* parent, const rapidjson::Value::ConstObject json);

        Type getType() const override { return Type::CircularGroup; }
};
