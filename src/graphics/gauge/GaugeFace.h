#pragma once

#include <memory>
#include "graphics/gauge/Element.h"

#define NO_TITLE_TEXT "No Title"
#define NO_DESCRIPTION_TEXT "No description."

class GaugeFace : public Element {
    private:
        std::unique_ptr<Color> backgroundColor;

        const char* title;

        const char* description;

        unsigned long lastUpdateTime = 0;

    public:
        explicit GaugeFace(YGConfigRef cfg) : Element(cfg) {}

        GaugeFace(JsonObject json);
};