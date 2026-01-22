#include "GraphicsContext.h"

float GraphicsContext::getFontScaleFactor(float point) {
    constexpr float basePx = 8.0f;

    if (point < 1.0f) point = basePx;

    float scale = point / basePx;
    if (scale < 1.0f) scale = 1.0f;

    return scale;
}

int GraphicsContext::getScreenWidth() const { return width; }

int GraphicsContext::getScreenHeight() const { return height; }

float GraphicsContext::getTextWidth(std::string_view text, float point) {
    if (text.empty()) return 0;

    std::string tmp(text);
    return getTextWidth(tmp.c_str(), point);
}
