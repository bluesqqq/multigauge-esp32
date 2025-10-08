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

void Graphics::clearColorCache()
{
    colorCache.clear();
    if (fill != nullptr) fillValue = getCachedColor(fill);
    if (stroke != nullptr) fillValue = getCachedColor(fill);
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

void Graphics::fillAll() const { fillAll(fill); }

void Graphics::fillAll(Color *color) const { if (color) context->fillAll(color->getColor()); }

void Graphics::fillRect(int x, int y, int width, int height) { if (fill) context->fillRectangle(x, y, width, height, fill->getColor()); }

void Graphics::fillRect(const Rectangle<int> &rectangle) { 
    Point<int> pos = rectangle.getTopLeft();
    fillRect(pos.x, pos.y, rectangle.width, rectangle.height);
}

void Graphics::strokeRect(int x, int y, int width, int height) { if (fill) context->fillRectangle(x, y, width, height, fill->getColor()); }

void Graphics::strokeRect(const Rectangle<int> &rectangle) {
    Point<int> pos = rectangle.getTopLeft();
    strokeRect(pos.x, pos.y, rectangle.width, rectangle.height);
}

void Graphics::fillCircle(int cx, int cy, int radius) const {
    if (fill) context->fillCircle(cx, cy, radius, fill->getColor());
}
