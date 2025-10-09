#include "GraphicsContextLovyanGFX.h"

void GraphicsContextLovyanGFX::init() {
    display.init();
    display.setBrightness(255);
    display.setRotation(1);
    display.fillScreen(TFT_RED);

    buffer.setColorDepth(16);
    buffer.setPsram(false);
    buffer.createSprite(display.width(), display.height());
}

void GraphicsContextLovyanGFX::endFrame() {
    buffer.pushSprite(0, 0);
}

void GraphicsContextLovyanGFX::fillRectangle(float x, float y, float w, float h, uint16_t color) { buffer.fillRect(x, y, w, h, color); }

void GraphicsContextLovyanGFX::strokeRectangle(float x, float y, float w, float h, uint16_t color, float thickness) {

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
