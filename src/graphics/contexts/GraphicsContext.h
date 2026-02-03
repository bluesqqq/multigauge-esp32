#pragma once

#include "graphics/colors/Color.h"
#include "graphics/geometry/alignment.h"

#include "images/Image.h"

#include "graphics/TextStyle.h"

class GraphicsContext {
    protected:
        int width;
        int height;

    public:
        /// @brief Called once during initialization
        virtual void init() {}

        /// @brief Called before drawing a new frame
        virtual void beginFrame() {};

        /// @brief Called after all draw calls for the current frame are finished
        virtual void endFrame() {};

        int getScreenWidth() const;
        int getScreenHeight() const;
        
        //----------[ FILL ]----------//
        virtual void fillAll(uint16_t color) = 0;

        //----------[ PIXEL ]----------//
        virtual void drawPixel(int x, int y, uint16_t color) = 0;

        //----------[ LINE ]----------//
        virtual void strokeLine(int x0, int y0, int x1, int y1, uint16_t color) = 0;
        virtual void fillWideLine(int x0, int y0, int x1, int y1, uint16_t color, float thickness) = 0;

        //----------[ RECTANGLE ]----------//
        virtual void fillRect(int x, int y, int w, int h, uint16_t color) = 0;
        virtual void strokeRect(int x, int y, int w, int h, uint16_t color, float thickness) = 0;

        virtual void fillRoundRect(int x, int y, int w, int h, float radius, uint16_t color) = 0;
        virtual void fillRoundRect(int x, int y, int w, int h, float r1, float r2, float r3, float r4, uint16_t color) = 0;

        virtual void strokeRoundRect(int x, int y, int w, int h, float radius, uint16_t color, float thickness) = 0;
        virtual void strokeRoundRect(int x, int y, int w, int h, float r1, float r2, float r3, float r4, uint16_t color, float thickness) = 0;

        //----------[ CIRCLE ]----------//
        virtual void fillCircle(int cx, int cy, int r, uint16_t color) = 0;
        virtual void strokeCircle(int cx, int cy, int r, uint16_t color, float thickness) = 0;

        //----------[ ELLIPSE ]----------//
        virtual void fillEllipse(int cx, int cy, int rx, int ry, uint16_t color) = 0;
        virtual void strokeEllipse(int cx, int cy, int rx, int ry, uint16_t color, float thickness) = 0;

        //----------[ RING ]----------//
        virtual void fillRing(int cx, int cy, int r, uint16_t color) = 0;
        virtual void strokeRing(int cx, int cy, int r, uint16_t color, float thickness) = 0;

        //----------[ ARC ]----------//
        virtual void fillArc(int cx, int cy, int r1, int r2, float start, float end, uint16_t color) = 0;
        virtual void strokeArc(int cx, int cy, int r1, int r2, float start, float end, uint16_t color, float thickness) = 0;

        //----------[ TRIANGLE ]----------//
        virtual void strokeTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color, float thickness) = 0;
        virtual void fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color) = 0;

        //----------[ TEXT ]----------//
        virtual float getTextWidth(const char* text, std::string family, float pt, FontWeight weight, FontSlant slant) = 0;
        float getTextWidth(std::string_view text, std::string family, float pt, FontWeight weight, FontSlant slant);

        virtual void drawText(const char* text, int x, int y, std::string family, float pt, FontWeight weight, FontSlant slant, uint16_t color, Anchor anchor = Anchor::TopLeft) = 0;

        //----------[ IMAGE ]----------//
        virtual Image createNativeImage(const uint16_t* pixels, int w, int h) = 0;

        virtual void drawImage(const Image& img, int x, int y, Anchor anchor = Anchor::TopLeft) = 0;
        virtual void drawImage(const Image& img, int x, int y, int width, int height) = 0;

        //----------[ CLIP ]----------//
        virtual void setClipRect(int x, int y, int width, int height) = 0;
        virtual void clearClipRect() = 0;
};