#pragma once

#include "GaugeElement.h"
#include <memory>

#define NO_TITLE_TEXT "No Title"
#define NO_DESCRIPTION_TEXT "No description."

class GaugeFace {
    private:
        std::vector<std::unique_ptr<GaugeElement>> elements;

        std::unique_ptr<Color> backgroundColor;

        const char* title;

        const char* description;

        unsigned long lastUpdateTime = 0;

    public:
        GaugeFace();

        GaugeFace(const char* title, const char* description);

        GaugeFace(JsonObject json);

        void addGaugeElement(std::unique_ptr<GaugeElement> element);

        void init();

        void draw(Graphics& g) const;

        void update();

        JsonObject serialize();
};