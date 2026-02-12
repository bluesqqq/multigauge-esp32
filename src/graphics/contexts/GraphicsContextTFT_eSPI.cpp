#include "GraphicsContextTFT_eSPI.h"

bool GraphicsContextTFT_eSPI::init() {
    display.init();
    display.setRotation(1);
    display.fillScreen(TFT_BLACK);

    buffer.setColorDepth(8);
    buffer.createSprite(display.width(), display.height());

    width = display.width();
    height = display.height();
}

void GraphicsContextTFT_eSPI::endFrame() {
    buffer.pushSprite(0, 0);
}

void GraphicsContextTFT_eSPI::fillRect(int x, int y, int w, int h, rgba color) {
    buffer.fillRect((int)x, (int)y, (int)w, (int)h, tftColor(color));
}

void GraphicsContextTFT_eSPI::strokeRect(int x, int y, int w, int h, rgba color, float thickness) {
    int t = (int)thickness;
    for (int i = 0; i < t; ++i) {
        buffer.drawRect((int)x + i, (int)y + i, (int)w - 2*i, (int)h - 2*i, tftColor(color));
    }
}

void GraphicsContextTFT_eSPI::fillCircle(int cx, int cy, int r, rgba color) {
    buffer.fillCircle(cx, cy, r, tftColor(color));
}

void GraphicsContextTFT_eSPI::strokeCircle(int cx, int cy, int r, rgba color, float thickness) {
    int t = (int)thickness;
    for (int i = 0; i < t; ++i) {
        buffer.drawCircle(cx, cy, r - i, tftColor(color));
    }
}

void GraphicsContextTFT_eSPI::fillRing(int cx, int cy, int r, rgba color) {
    buffer.drawCircle(cx, cy, r, tftColor(color));
}

void GraphicsContextTFT_eSPI::strokeRing(int cx, int cy, int r, rgba color, float thickness) {
    strokeCircle(cx, cy, r, color, thickness);
}

void GraphicsContextTFT_eSPI::fillEllipse(int cx, int cy, int rx, int ry, rgba color) {
    buffer.fillEllipse(cx, cy, rx, ry, tftColor(color));
}

void GraphicsContextTFT_eSPI::strokeEllipse(int cx, int cy, int rx, int ry, rgba color, float thickness) {
    int t = (int)thickness;
    for (int i = 0; i < t; ++i) {
        buffer.drawEllipse(cx, cy, rx - i, ry - i, tftColor(color));
    }
}

void GraphicsContextTFT_eSPI::fillAll(rgba color) {
    buffer.fillScreen(tftColor(color));
}
