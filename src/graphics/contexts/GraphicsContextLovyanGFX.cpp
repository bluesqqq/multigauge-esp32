#include "GraphicsContextLovyanGFX.h"

//----------[ FONT REGISTRY SYSTEM ]----------//

const lgfx::IFont *LGFXFontStroke::getFont(bool b, bool i) const {
    if (b && i) {
        if (boldItalic) return boldItalic;
    }
    if (b) return bold ? bold : normal;
    if (i) return italic ? italic : normal;
    return normal;
}

auto LGFXFontFamily::findStrokeForPt(float pt) const {
    auto it = std::upper_bound(strokes.begin(), strokes.end(), pt, [](float value, const auto& elem) { return value < elem.first; });

    if (it == strokes.begin()) return strokes.begin();
    --it;
    return it;
}

void LGFXFontFamily::addStroke(float pt, LGFXFontStroke stroke) {
    auto it = std::lower_bound(strokes.begin(), strokes.end(), pt, [](const auto& elem, float value) { return elem.first < value; });

    if (it != strokes.end() && it->first == pt) it->second = stroke;
    else strokes.insert(it, {pt, stroke});
}

LGFXFontPick LGFXFontFamily::getFont(float pt, bool wantBold, bool wantItalic) const {
    if (strokes.empty()) return { &DEFAULT_LGFX_FONT, DEFAULT_LGFX_FONT_PT };

    auto it = findStrokeForPt(pt);
    const float chosenPt = it->first;
    const LGFXFontStroke& stroke = it->second;

    // Use your stroke fallback logic (includes boldItalic handling)
    const lgfx::IFont* f = stroke.getFont(wantBold, wantItalic);

    if (!f) f = &DEFAULT_LGFX_FONT;

    return { f, chosenPt };
}






float GraphicsContextLovyanGFX::getFontScaleFactor(float point) {
    if (point < 1.0f) point = currentFontPoint;

    float scale = point / currentFontPoint;
    if (scale < 1.0f) scale = 1.0f;

    return scale;
}

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

void GraphicsContextLovyanGFX::setFontInternal(std::string family, float pt, FontWeight weight, FontSlant slant) {
    const lgfx::IFont* f = &DEFAULT_LGFX_FONT;
    float basePt = DEFAULT_LGFX_FONT_PT; // fallback

    auto it = fontFamilies.find(family);
    if (it != fontFamilies.end()) {
        auto pick = it->second.getFont(pt, weight == FontWeight::Bold, slant == FontSlant::Italic);
        f = pick.font;
        basePt = pick.strokePt;
    }

    buffer.setTextSize(1);
    buffer.setFont(f);
    currentFontPoint = buffer.fontHeight();

    buffer.setTextSize(pt / currentFontPoint);
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

    // Font registry system

    LGFXFontFamily sans;
    sans.addStroke(9,  {&lgfx::fonts::FreeSans9pt7b,  &lgfx::fonts::FreeSansBold9pt7b,  &lgfx::fonts::FreeSansOblique9pt7b,  &lgfx::fonts::FreeSansBoldOblique9pt7b});
    sans.addStroke(12, {&lgfx::fonts::FreeSans12pt7b, &lgfx::fonts::FreeSansBold12pt7b, &lgfx::fonts::FreeSansOblique12pt7b, &lgfx::fonts::FreeSansBoldOblique12pt7b});
    sans.addStroke(18, {&lgfx::fonts::FreeSans18pt7b, &lgfx::fonts::FreeSansBold18pt7b, &lgfx::fonts::FreeSansOblique18pt7b, &lgfx::fonts::FreeSansBoldOblique18pt7b});
    sans.addStroke(24, {&lgfx::fonts::FreeSans24pt7b, &lgfx::fonts::FreeSansBold24pt7b, &lgfx::fonts::FreeSansOblique24pt7b, &lgfx::fonts::FreeSansBoldOblique24pt7b});
    fontFamilies["sans"] = sans;

    LGFXFontFamily serif;
    serif.addStroke(9,  {&lgfx::fonts::FreeSerif9pt7b,  &lgfx::fonts::FreeSerifBold9pt7b,  &lgfx::fonts::FreeSerifItalic9pt7b,  &lgfx::fonts::FreeSerifBoldItalic9pt7b});
    serif.addStroke(12, {&lgfx::fonts::FreeSerif12pt7b, &lgfx::fonts::FreeSerifBold12pt7b, &lgfx::fonts::FreeSerifItalic12pt7b, &lgfx::fonts::FreeSerifBoldItalic12pt7b});
    serif.addStroke(18, {&lgfx::fonts::FreeSerif18pt7b, &lgfx::fonts::FreeSerifBold18pt7b, &lgfx::fonts::FreeSerifItalic18pt7b, &lgfx::fonts::FreeSerifBoldItalic18pt7b});
    serif.addStroke(24, {&lgfx::fonts::FreeSerif24pt7b, &lgfx::fonts::FreeSerifBold24pt7b, &lgfx::fonts::FreeSerifItalic24pt7b, &lgfx::fonts::FreeSerifBoldItalic24pt7b});
    fontFamilies["serif"] = serif;

    LGFXFontFamily mono;
    mono.addStroke(9,  {&lgfx::fonts::FreeMono9pt7b,  &lgfx::fonts::FreeMonoBold9pt7b,  &lgfx::fonts::FreeMonoOblique9pt7b,  &lgfx::fonts::FreeMonoBoldOblique9pt7b});
    mono.addStroke(12, {&lgfx::fonts::FreeMono12pt7b, &lgfx::fonts::FreeMonoBold12pt7b, &lgfx::fonts::FreeMonoOblique12pt7b, &lgfx::fonts::FreeMonoBoldOblique12pt7b});
    mono.addStroke(18, {&lgfx::fonts::FreeMono18pt7b, &lgfx::fonts::FreeMonoBold18pt7b, &lgfx::fonts::FreeMonoOblique18pt7b, &lgfx::fonts::FreeMonoBoldOblique18pt7b});
    mono.addStroke(24, {&lgfx::fonts::FreeMono24pt7b, &lgfx::fonts::FreeMonoBold24pt7b, &lgfx::fonts::FreeMonoOblique24pt7b, &lgfx::fonts::FreeMonoBoldOblique24pt7b});
    fontFamilies["mono"] = mono;

    LGFXFontFamily orbitron;
    orbitron.addStroke(24, {&lgfx::fonts::Orbitron_Light_24});
    orbitron.addStroke(32, {&lgfx::fonts::Orbitron_Light_32});
    fontFamilies["orbitron"] = orbitron;

    LGFXFontFamily dejavu;
    dejavu.addStroke(9, {&lgfx::fonts::DejaVu9});
    dejavu.addStroke(12, {&lgfx::fonts::DejaVu12});
    dejavu.addStroke(18, {&lgfx::fonts::DejaVu18});
    dejavu.addStroke(24, {&lgfx::fonts::DejaVu24});
    dejavu.addStroke(40, {&lgfx::fonts::DejaVu40});
    dejavu.addStroke(56, {&lgfx::fonts::DejaVu56});
    dejavu.addStroke(72, {&lgfx::fonts::DejaVu72});
    fontFamilies["dejavu"] = dejavu;

    LGFXFontFamily roboto;
    roboto.addStroke(24, {&lgfx::fonts::Roboto_Thin_24});
    fontFamilies["roboto"] = roboto;

    buffer.setFont(&lgfx::fonts::FreeSans12pt7b);
    currentFontPoint = 12;
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

float GraphicsContextLovyanGFX::getTextWidth(const char *text, std::string family, float pt, FontWeight weight, FontSlant slant) { 
    setFontInternal(family, pt, weight, slant);
    return buffer.textWidth(text);
}

void GraphicsContextLovyanGFX::drawText(const char* text, int x, int y, std::string family, float pt, FontWeight weight, FontSlant slant, uint16_t color, Anchor anchor) {
    setFontInternal(family, pt, weight, slant);
    buffer.setTextDatum(anchorToDatum(anchor));
    buffer.setTextColor(color);
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