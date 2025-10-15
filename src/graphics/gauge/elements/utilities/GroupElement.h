#pragma once

#include "graphics/gauge/GaugeElement.h"
#include "graphics/colors/Color.h"
#include "graphics/geometry/alignment.h"

struct Element {
    bool freeform;
    
    int margin;
    int padding;

    std::vector<std::unique_ptr<GaugeElement>> children;   
};

class GroupElement : public GaugeElement {
    private:
        std::vector<std::unique_ptr<GaugeElement>> elements;

        int margin = 0;
        int padding = 0;
        float gap = 0; // Gap between items

        std::unique_ptr<Color> backgroundColor = nullptr;

        Orientation orientation = Orientation::Vertical;
        Justify justify = Justify::Start;
        Align   align   = Align::Center;

    public:
        GroupElement();

        GroupElement(JsonObject json);

        void addElement(std::unique_ptr<GaugeElement> element);

        void addElement(JsonObject json);

        void draw(Graphics& g, Rectangle<int> bounds) const override;
};