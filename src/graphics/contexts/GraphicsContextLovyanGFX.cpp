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

void GraphicsContextLovyanGFX::drawPixel(int x, int y, uint16_t color) {
    buffer.drawPixel(x, y, color);
}

void GraphicsContextLovyanGFX::strokeLine(int x0, int y0, int x1, int y1, uint16_t color) {
    buffer.drawLine(x0, y0, x1, y1, color);
}

void GraphicsContextLovyanGFX::fillWideLine(int x0, int y0, int x1, int y1, uint16_t color, float thickness) {
    buffer.drawWideLine(x0, y0, x1, y1, thickness, color);
}

void GraphicsContextLovyanGFX::strokeTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color, float thickness) {
    buffer.drawTriangle(x0, y0, x1, y1, x2, y2, color);
}

void GraphicsContextLovyanGFX::fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color) {
    buffer.fillTriangle(x0, y0, x1, y1, x2, y2, color);
}

void GraphicsContextLovyanGFX::fillRect(int x, int y, int w, int h, uint16_t color) { buffer.fillRect(x, y, w + 1, h + 1, color); }

void GraphicsContextLovyanGFX::strokeRect(int x, int y, int w, int h, uint16_t color, float thickness) {
    buffer.drawRect(x, y, w + 1, h + 1, color);
}

void GraphicsContextLovyanGFX::fillRoundRect(int x, int y, int w, int h, float radius, uint16_t color) {
    buffer.fillRoundRect(x, y, w, h, radius, color);
}

void GraphicsContextLovyanGFX::fillRoundRect(int x, int y, int w, int h, float r1, float r2, float r3, float r4, uint16_t color) {
    // TODO: CHANGE IMPL
    buffer.fillRoundRect(x, y, w, h, r1, color);
}

void GraphicsContextLovyanGFX::strokeRoundRect(int x, int y, int w, int h, float radius, uint16_t color, float thickness) {
    buffer.drawRoundRect(x, y, w, h, radius, color);
}

void GraphicsContextLovyanGFX::strokeRoundRect(int x, int y, int w, int h, float r1, float r2, float r3, float r4, uint16_t color, float thickness) {
    // TODO: CHANGE IMPL
    buffer.drawRoundRect(x, y, w, h, r1, color);
}

void GraphicsContextLovyanGFX::fillCircle(int cx, int cy, int r, uint16_t color) { buffer.fillCircle(cx, cy, r, color); }

void GraphicsContextLovyanGFX::strokeCircle(int cx, int cy, int r, uint16_t color, float thickness) {
    buffer.drawCircle(cx, cy, r, color);
}

void GraphicsContextLovyanGFX::fillRing(int cx, int cy, int r, uint16_t color) {
}

void GraphicsContextLovyanGFX::strokeRing(int cx, int cy, int r, uint16_t color, float thickness) {
}

void GraphicsContextLovyanGFX::fillArc(int cx, int cy, int r1, int r2, float start, float end, uint16_t color) { buffer.fillArc(cx, cy, r1, r2, start, end, color); }

void GraphicsContextLovyanGFX::strokeArc(int cx, int cy, int r1, int r2, float start, float end, uint16_t color, float thickness) { buffer.drawArc(cx, cy, r1, r2, start, end, color); }

void GraphicsContextLovyanGFX::fillEllipse(int cx, int cy, int rx, int ry, uint16_t color) { buffer.fillEllipse(cx, cy, rx, ry, color); }

void GraphicsContextLovyanGFX::strokeEllipse(int cx, int cy, int rx, int ry, uint16_t color, float thickness) { buffer.drawEllipse(cx, cy, rx, ry, color);  }

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

Image GraphicsContextLovyanGFX::createNativeImage(const uint16_t* pixels, int w, int h) {
    auto* spr = new lgfx::LGFX_Sprite(&buffer);
    spr->setColorDepth(16);
    spr->setPsram(true);

    if (!spr->createSprite(w, h)) {
        delete spr;
        return Image(0, 0, nullptr, nullptr);
    }

    spr->setSwapBytes(true);
    spr->pushImage(0, 0, w, h, pixels);
    spr->setSwapBytes(false);

    return Image(w, h, spr, [](void* ptr) { delete static_cast<lgfx::LGFX_Sprite*>(ptr); });
}

void GraphicsContextLovyanGFX::drawImage(const Image &img, int x, int y, Anchor anchor) {
    if (!img.native) return;
    auto* spr = static_cast<lgfx::LGFX_Sprite*>(img.native);
    if (!spr) return;

    int drawX = x;
    int drawY = y;

    switch (anchor) {
        case Anchor::TopLeft:
            break;

        case Anchor::TopCenter:
            drawX -= img.width / 2;
            break;

        case Anchor::TopRight:
            drawX -= img.width;
            break;

        case Anchor::CenterLeft:
            drawY -= img.height / 2;
            break;

        case Anchor::Center:
            drawX -= img.width / 2;
            drawY -= img.height / 2;
            break;

        case Anchor::CenterRight:
            drawX -= img.width;
            drawY -= img.height / 2;
            break;

        case Anchor::BottomLeft:
            drawY -= img.height;
            break;

        case Anchor::BottomCenter:
            drawX -= img.width / 2;
            drawY -= img.height;
            break;

        case Anchor::BottomRight:
            drawX -= img.width;
            drawY -= img.height;
            break;
    }

    spr->pushSprite(drawX, drawY);
}

void GraphicsContextLovyanGFX::drawImage(const Image &img, int x, int y, int width, int height) {
    if (!img.native || width <= 0 || height <= 0 || img.width <= 0 || img.height <= 0) return;

    auto* spr = static_cast<lgfx::LGFX_Sprite*>(img.native);
    if (!spr) return;

    const float zx = (float)width  / (float)img.width;
    const float zy = (float)height / (float)img.height;

    spr->pushRotateZoom(x + width / 2, y + height / 2, 0.0f, zx, zy);
}

//----------[ CLIP ]----------//
void GraphicsContextLovyanGFX::setClipRect(int x, int y, int w, int h) { buffer.setClipRect(x, y, w, h); }

void GraphicsContextLovyanGFX::clearClipRect() { buffer.clearClipRect(); }
