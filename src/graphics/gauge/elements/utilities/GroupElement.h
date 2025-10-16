#pragma once

#include "graphics/gauge/GaugeElement.h"
#include "graphics/colors/Color.h"

class GroupElement : public GaugeElement {
    public:
        GroupElement();

        GroupElement(JsonObject json);

        void draw(Graphics& g) const override;
};