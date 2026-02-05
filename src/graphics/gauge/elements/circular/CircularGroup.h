#pragma once

#include "graphics/gauge/Element.h"
#include "graphics/DisplayValue.h"

class CircularGroup : public Element {
    private:
        DisplayValue value = nullptr;

        float startAngle = 0;
        float endAngle = 360;

    public:
        explicit CircularGroup(YGConfigRef config);

        CircularGroup(YGConfigRef config, const rapidjson::Value::ConstObject json);

        Type getType() const override { return Type::CircularGroup; }
};
