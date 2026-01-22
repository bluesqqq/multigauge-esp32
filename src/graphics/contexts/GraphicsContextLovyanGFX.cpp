#include "GraphicsContextLovyanGFX.h"

lgfx::v1::textdatum::textdatum_t GraphicsContextLovyanGFX::anchorToDatum(Anchor anchor) {
    using lgfx::v1::textdatum_t;

    switch (anchor)
    {
        case Anchor::TopLeft:      return textdatum_t::top_left;
        case Anchor::TopCenter:    return textdatum_t::top_center;
        case Anchor::TopRight:     return textdatum_t::top_right;

        case Anchor::CenterLeft:   return textdatum_t::middle_left;
        case Anchor::Center: return textdatum_t::middle_center;
        case Anchor::CenterRight:  return textdatum_t::middle_right;

        case Anchor::BottomLeft:   return textdatum_t::bottom_left;
        case Anchor::BottomCenter: return textdatum_t::bottom_center;
        case Anchor::BottomRight:  return textdatum_t::bottom_right;
    }

    // Fallback if Anchor gets extended without updating this function.
    return textdatum_t::top_left;
}

void GraphicsContextLovyanGFX::init() {
    display.init();
    display.setBrightness(255);
    display.setRotation(0);
    display.fillScreen(TFT_RED);

    buffer.setColorDepth(16);
    buffer.setPsram(false);
    buffer.createSprite(display.width(), display.height());

    width = display.width();
    height = display.height();
}

void GraphicsContextLovyanGFX::endFrame() {
    buffer.pushSprite(0, 0);
}

void GraphicsContextLovyanGFX::strokeLine(int x0, int y0, int x1, int y1, uint16_t color) {
    buffer.drawLine(x0, y0, x1, y1, color);
}

void GraphicsContextLovyanGFX::fillWideLine(int x0, int y0, int x1, int y1, uint16_t color, float thickness) {
    buffer.drawWideLine(x0, y0, x1, y1, thickness, color);
}

void GraphicsContextLovyanGFX::strokeTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color) {
    buffer.drawTriangle(x0, y0, x1, y1, x2, y2, color);
}

void GraphicsContextLovyanGFX::fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color) {
    buffer.fillTriangle(x0, y0, x1, y1, x2, y2, color);
}

void GraphicsContextLovyanGFX::fillRectangle(int x, int y, int w, int h, uint16_t color) { buffer.fillRect(x, y, w, h, color); }

void GraphicsContextLovyanGFX::strokeRectangle(int x, int y, int w, int h, uint16_t color, float thickness) {
    buffer.drawRect(x, y, w, h, color);
}

void GraphicsContextLovyanGFX::fillCircle(int cx, int cy, int r, uint16_t color) { buffer.fillCircle(cx, cy, r, color); }

void GraphicsContextLovyanGFX::strokeCircle(int cx, int cy, int r, uint16_t color, float thickness) {
    buffer.drawCircle(cx, cy, r, color);
}

void GraphicsContextLovyanGFX::fillRing(int cx, int cy, int r, uint16_t color) {
}

void GraphicsContextLovyanGFX::strokeRing(int cx, int cy, int r, uint16_t color, float thickness) {
}

void GraphicsContextLovyanGFX::fillEllipse(int cx, int cy, int rx, int ry, uint16_t color) { buffer.fillEllipse(cx, cy, rx, ry, color); }

void GraphicsContextLovyanGFX::strokeEllipse(int cx, int cy, int rx, int ry, uint16_t color, float thickness) {
}

void GraphicsContextLovyanGFX::fillAll(uint16_t color) { buffer.fillScreen(color); }

float GraphicsContextLovyanGFX::getTextWidth(const char *text, float point) { 
    buffer.setTextSize(getFontScaleFactor(point));
    return buffer.textWidth(text);
}

void GraphicsContextLovyanGFX::drawText(const char *text, int x, int y, uint16_t color, float point, Anchor anchor) {
    buffer.setTextDatum(anchorToDatum(anchor));
    buffer.setTextColor(color);
    buffer.setTextSize(getFontScaleFactor(point));
    buffer.drawString(text, x, y);
}
