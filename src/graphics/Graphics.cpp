#include "Graphics.h"

uint16_t Graphics::getCachedColor(Color *color) {
    auto it = colorCache.find(color);
    if (it != colorCache.end())
        return it->second;

    uint16_t c = color->getColor();
    colorCache[color] = c;
    return c;
}

Graphics::Graphics(GraphicsContext *context) : context(context) { }

void Graphics::clearColorCache() {
    colorCache.clear();
    if (fill != nullptr) fillValue = getCachedColor(fill);
    if (stroke != nullptr) fillValue = getCachedColor(fill);
}

Rectangle<int> Graphics::getScreenBounds() {
    return Rectangle<int>(0, 0, context->getScreenWidth(), context->getScreenHeight());
}

void Graphics::setFill(uint16_t color) {
    fill = nullptr;
    fillValue = color;
}

void Graphics::setFill(Color *color) {
    fill = color;
    fillValue = getCachedColor(color);
}

void Graphics::setStroke(uint16_t color) {
    stroke = nullptr;
    strokeValue = color;
}

void Graphics::setStroke(Color *color) {
    stroke = color; 
    strokeValue = getCachedColor(color);
}

void Graphics::fillAll() const { fillAll(fillValue); }

void Graphics::fillAll(Color *color) { context->fillAll(getCachedColor(color)); }

void Graphics::fillAll(uint16_t color) const { context->fillAll(color); }

void Graphics::fillRect(int x, int y, int width, int height) { context->fillRectangle(x, y, width, height, fillValue); }

void Graphics::fillRect(const Rectangle<int> &rectangle) { fillRect(rectangle.position.x, rectangle.position.y, rectangle.width, rectangle.height); }

void Graphics::strokeRect(int x, int y, int width, int height) { context->strokeRectangle(x, y, width, height, strokeValue, 1); }

void Graphics::strokeRect(const Rectangle<int> &rectangle) { strokeRect(rectangle.position.x, rectangle.position.y, rectangle.width, rectangle.height); }

void Graphics::fillCircle(int cx, int cy, int radius) const { context->fillCircle(cx, cy, radius, fillValue); }

void Graphics::strokeCircle(int cx, int cy, int radius) const { context->strokeCircle(cx, cy, radius, strokeValue, 1); }
