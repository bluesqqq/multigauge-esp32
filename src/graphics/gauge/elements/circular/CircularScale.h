#pragma once

#include "graphics/gauge/Element.h"
#include "graphics/DisplayValue.h"
#include "graphics/ticks/TickList.h"

class CircularScale : public Element {
    private:
        //TickList ticks;

        float radius = 1.0f;

    public:
        explicit CircularScale(Element* parent);

        CircularScale(Element* parent, const rapidjson::Value::ConstObject json);

        void draw(Graphics& g) const override;
};
