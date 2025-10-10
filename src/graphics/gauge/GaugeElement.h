#pragma once

#include "graphics/Graphics.h"

class GaugeElement {
    public:
        static std::unique_ptr<GaugeElement> loadFromJson(JsonObject json);
        
        virtual ~GaugeElement() = default;

        virtual bool init() { return true; };

        virtual void draw(Graphics& g) const = 0;

        virtual void update(float deltaTime) = 0;

        virtual JsonObject serialize() = 0;
};