#pragma once

#include "graphics/colors/Color.h"

class GraphicsContext {
    public:
        virtual void fillRectangle(float x, float y, float w, float h, uint16_t color) = 0;
        virtual void strokeRectangle(float x, float y, float w, float h, uint16_t color) = 0;
        virtual void fillCircle(int cx, int cy, int r, uint16_t color) = 0;
        virtual void strokeCircle(int cx, int cy, int r, uint16_t color) = 0;
        virtual void fillRing(int cx, int cy, int r, uint16_t color) = 0;
        virtual void strokeRing(int cx, int cy, int r, uint16_t color) = 0;
        virtual void fillEllipse(int cx, int cy, int rx, int ry, uint16_t color) = 0;
        virtual void strokeEllipse(int cx, int cy, int rx, int ry, uint16_t color) = 0;
        virtual void fillAll(uint16_t color) = 0;
};

#include <LovyanGFX.h>

class GraphicsContextLovyanGFX : public GraphicsContext {
    private:
        LGFX_Device* display;

    public:
        GraphicsContextLovyanGFX(LGFX_Device* display) : display(display) {}

        void fillRectangle(float x, float y, float w, float h, uint16_t color) override {
            display->fillRect(x, y, w, h, color);
        }

        void fillCircle(int cx, int cy, int r, uint16_t color) override {
            display->fillCircle(cx, cy, r, color);
        }


};