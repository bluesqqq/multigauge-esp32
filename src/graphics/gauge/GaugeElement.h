#pragma once

#include "graphics/Graphics.h"

class GaugeElement {
    public:
        static std::unique_ptr<GaugeElement> loadFromJson(JsonObject json);

        virtual bool init();

        virtual void draw(Graphics& g, Rectangle<int> bounds) const = 0;

        virtual void update(float deltaTime) {};
};