#pragma once

#include "graphics/colors/Color.h"

class GraphicsContext {
    public:
        /// @brief Called once during initialization
        virtual void init() {}

        /// @brief Called before drawing a new frame
        virtual void beginFrame() {};

        /// @brief Called after all draw calls for the current frame are finished
        virtual void endFrame() {};

        virtual void fillRectangle(float x, float y, float w, float h, uint16_t color) = 0;
        virtual void strokeRectangle(float x, float y, float w, float h, uint16_t color, float thickness) = 0;
        virtual void fillCircle(int cx, int cy, int r, uint16_t color) = 0;
        virtual void strokeCircle(int cx, int cy, int r, uint16_t color, float thickness) = 0;
        virtual void fillRing(int cx, int cy, int r, uint16_t color) = 0;
        virtual void strokeRing(int cx, int cy, int r, uint16_t color, float thickness) = 0;
        virtual void fillEllipse(int cx, int cy, int rx, int ry, uint16_t color) = 0;
        virtual void strokeEllipse(int cx, int cy, int rx, int ry, uint16_t color, float thickness) = 0;
        virtual void fillAll(uint16_t color) = 0;
};