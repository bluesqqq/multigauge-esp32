#pragma once

#include "graphics/colors/Color.h"
#include "graphics/geometry/alignment.h"

class GraphicsContext {
    protected:
        int width;
        int height;

        float getFontScaleFactor(float point);

    public:
        /// @brief Called once during initialization
        virtual void init() {}

        /// @brief Called before drawing a new frame
        virtual void beginFrame() {};

        /// @brief Called after all draw calls for the current frame are finished
        virtual void endFrame() {};

        int getScreenWidth() const;
        int getScreenHeight() const;
        
        virtual void strokeLine(int x0, int y0, int x1, int y1, uint16_t color) = 0;
        virtual void fillWideLine(int x0, int y0, int x1, int y1, uint16_t color, float thickness = 1) = 0;

        virtual void strokeTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color) = 0;
        virtual void fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color) = 0;

        virtual void fillRectangle(int x, int y, int w, int h, uint16_t color) = 0;
        virtual void strokeRectangle(int x, int y, int w, int h, uint16_t color, float thickness) = 0;

        virtual void fillCircle(int cx, int cy, int r, uint16_t color) = 0;
        virtual void strokeCircle(int cx, int cy, int r, uint16_t color, float thickness) = 0;

        virtual void fillRing(int cx, int cy, int r, uint16_t color) = 0;
        virtual void strokeRing(int cx, int cy, int r, uint16_t color, float thickness) = 0;

        virtual void fillEllipse(int cx, int cy, int rx, int ry, uint16_t color) = 0;
        virtual void strokeEllipse(int cx, int cy, int rx, int ry, uint16_t color, float thickness) = 0;
        virtual void fillAll(uint16_t color) = 0;

        // Text

        virtual float getTextWidth(const char* text, float point) = 0;
        float getTextWidth(std::string_view text, float point);

        virtual void drawText(const char* text, int x, int y, uint16_t color, float point, Anchor anchor = Anchor::TopLeft) = 0;
};