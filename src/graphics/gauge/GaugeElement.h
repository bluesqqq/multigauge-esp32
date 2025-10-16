#pragma once

#include "graphics/Graphics.h"
#include "graphics/gauge/Element.h"

class GaugeElement : public Element {
    public:
        static std::unique_ptr<GaugeElement> loadFromJson(JsonObject json);

        virtual bool init();

        virtual void draw(Graphics& g) const = 0;

        virtual void update(float deltaTime) {};
};