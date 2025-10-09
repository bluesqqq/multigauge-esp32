#include "GraphicsContextTFT_eSPI.h"

void GraphicsContextTFT_eSPI::init() {
    display.init();
    display.setRotation(1);
    display.fillScreen(TFT_BLACK);

    buffer.setColorDepth(16);
    buffer.createSprite(display.width(), display.height());
}

void GraphicsContextTFT_eSPI::fillRectangle(float x, float y, float w, float h, uint16_t color) {
}
