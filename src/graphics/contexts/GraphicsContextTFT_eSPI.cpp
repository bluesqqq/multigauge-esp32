#include "GraphicsContextTFT_eSPI.h"

void GraphicsContextTFT_eSPI::init() {
    display.init();
    display.setRotation(1);
    display.fillScreen(TFT_BLACK);

    buffer.setColorDepth(8);
    buffer.createSprite(240, 240);
}

void GraphicsContextTFT_eSPI::endFrame() {
    buffer.pushSprite(0, 0);
}

void GraphicsContextTFT_eSPI::fillRectangle(float x, float y, float w, float h, uint16_t color) {
    buffer.fillRect((int)x, (int)y, (int)w, (int)h, color);
}

void GraphicsContextTFT_eSPI::strokeRectangle(float x, float y, float w, float h, uint16_t color, float thickness) {
    int t = (int)thickness;
    for (int i = 0; i < t; ++i) {
        buffer.drawRect((int)x + i, (int)y + i, (int)w - 2*i, (int)h - 2*i, color);
    }
}

void GraphicsContextTFT_eSPI::fillCircle(int cx, int cy, int r, uint16_t color) {
    buffer.fillCircle(cx, cy, r, color);
}

void GraphicsContextTFT_eSPI::strokeCircle(int cx, int cy, int r, uint16_t color, float thickness) {
    int t = (int)thickness;
    for (int i = 0; i < t; ++i) {
        buffer.drawCircle(cx, cy, r - i, color);
    }
}

void GraphicsContextTFT_eSPI::fillRing(int cx, int cy, int r, uint16_t color) {
    buffer.drawCircle(cx, cy, r, color);
}

void GraphicsContextTFT_eSPI::strokeRing(int cx, int cy, int r, uint16_t color, float thickness) {
    strokeCircle(cx, cy, r, color, thickness);
}

void GraphicsContextTFT_eSPI::fillEllipse(int cx, int cy, int rx, int ry, uint16_t color) {
    buffer.fillEllipse(cx, cy, rx, ry, color);
}

void GraphicsContextTFT_eSPI::strokeEllipse(int cx, int cy, int rx, int ry, uint16_t color, float thickness) {
    int t = (int)thickness;
    for (int i = 0; i < t; ++i) {
        buffer.drawEllipse(cx, cy, rx - i, ry - i, color);
    }
}

void GraphicsContextTFT_eSPI::fillAll(uint16_t color) {
    buffer.fillScreen(color);
}
