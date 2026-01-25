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
    if (stroke != nullptr) strokeValue = getCachedColor(stroke);
}

Rect<int> Graphics::getScreenBounds() {
    return Rect<int>(0, 0, context->getScreenWidth(), context->getScreenHeight());
}

void Graphics::setFill(uint16_t color) {
    fill = nullptr;
    fillValue = color;
}

void Graphics::setFill(Color *color) {
    if (!color) return;
    fill = color;
    fillValue = getCachedColor(color);
}

void Graphics::setStroke(uint16_t color) {
    stroke = nullptr;
    strokeValue = color;
}

void Graphics::setStroke(Color *color) {
    if (!color) return;
    stroke = color; 
    strokeValue = getCachedColor(color);
}

void Graphics::fillAll() const { fillAll(fillValue); }

void Graphics::fillAll(Color *color) { context->fillAll(getCachedColor(color)); }

void Graphics::fillAll(uint16_t color) const { context->fillAll(color); }

void Graphics::fillRect(int x, int y, int width, int height) { context->fillRect(x, y, width, height, fillValue); }

void Graphics::fillRect(const Rect<int> &rectangle) { fillRect(rectangle.position.x, rectangle.position.y, rectangle.width, rectangle.height); }

void Graphics::strokeRect(int x, int y, int width, int height) { context->strokeRect(x, y, width, height, strokeValue, 1); }

void Graphics::strokeRect(const Rect<int> &rectangle) { strokeRect(rectangle.position.x, rectangle.position.y, rectangle.width, rectangle.height); }

void Graphics::fillCircle(int cx, int cy, int radius) const { context->fillCircle(cx, cy, radius, fillValue); }

void Graphics::strokeCircle(int cx, int cy, int radius) const { context->strokeCircle(cx, cy, radius, strokeValue, 1); }

void Graphics::strokeLine(int x0, int y0, int x1, int y1) const { context->strokeLine(x0, y0, x1, y1, strokeValue); }

void Graphics::strokeLine(const Point<int> &p1, const Point<int> &p2) const { strokeLine(p1.x, p1.y, p2.x, p2.y); }

void Graphics::strokeLine(const Line<int> &line) const { strokeLine(line.p1.x, line.p1.y, line.p2.x, line.p2.y); }

void Graphics::fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2) { context->fillTriangle(x0, y0, x1, y1, x2, y2, fillValue); }

void Graphics::strokeTriangle(int x0, int y0, int x1, int y1, int x2, int y2) { context->strokeTriangle(x0, y0, x1, y1, x2, y2, strokeValue); }

void Graphics::setTextPoint(float point) { textPoint = point; }

void Graphics::drawText(const std::string &text, int x, int y, Anchor anchor) { context->drawText(text.c_str(), x, y, fillValue, textPoint, anchor); }

void Graphics::drawText(const std::string &text, Point<int> pos, Anchor anchor) { drawText(text, pos.x, pos.y, anchor); }

void Graphics::drawTextVertical(const std::string& text, int x, int y, Anchor anchor) {
    if (text.empty()) return;

    const int step = textPoint;
    if (step <= 0) return;

    int glyphCount = 0;
    for (char c : text) if (c != '\n') glyphCount++;
    if (glyphCount <= 0) return;

    const int blockH = glyphCount * step;

    Rect<int> box(x, y, 0, blockH);
    Point<int> start = Point<int>::getAnchored(box, anchor);

    int drawX = start.x;
    int drawY = start.y;

    Point<int> topP    = Point<int>::getAnchored(box, Anchor::TopLeft);
    Point<int> midP    = Point<int>::getAnchored(box, Anchor::CenterLeft);
    Point<int> bottomP = Point<int>::getAnchored(box, Anchor::BottomLeft);

    int dir = (start.y == bottomP.y) ? -1 : 1;

    if (start.y == midP.y) {
        drawY -= (blockH / 2);
        dir = 1;
    } else if (start.y == bottomP.y) {
        drawY -= step;
        dir = -1;
    }

    char buf[2] = {0, 0};

    for (char c : text) {
        if (c == '\n') continue;

        buf[0] = c;
        context->drawText(buf, drawX, drawY, fillValue, textPoint, Anchor::TopLeft);

        drawY += dir * step;
    }
}

void Graphics::drawTextArea(const std::string& text, int x, int y, int width, int height, Anchor anchor, bool useEllipses, bool useHyphens) {
    if (width <= 0 || height <= 0 || text.empty()) return;

    Rect<int> rect(x, y, width, height);

    const int lineHeight = textPoint;
    if (lineHeight <= 0) return;

    const int maxLines = rect.height / lineHeight;
    if (maxLines <= 0) return;

    uint16_t textColor = fill ? getCachedColor(fill) : fillValue;

    std::string scratch;
    scratch.reserve(128);

    auto measureString = [&](const std::string& s) -> int { return s.empty() ? 0 : context->getTextWidth(s.c_str(), textPoint); };

    auto measureRange = [&](int start, int len) -> int {
        if (len <= 0) return 0;
        scratch.assign(text.data() + start, (size_t)len);
        return context->getTextWidth(scratch.c_str(), textPoint);
    };

    const int n = (int)text.size();
    const int ellW = context->getTextWidth("...", textPoint);
    const int hyW  = context->getTextWidth("-", textPoint);

    std::vector<std::string> lines;
    lines.reserve(maxLines);

    int cursor = 0;

    auto skipSpaces = [&]() { while (cursor < n && text[cursor] == ' ') cursor++; };

    while (cursor < n && (int)lines.size() < maxLines) {
        if (text[cursor] == '\n') {
            lines.emplace_back("");
            cursor++;
            continue;
        }

        skipSpaces();
        if (cursor >= n) break;

        std::string line;

        while (cursor < n && text[cursor] != '\n') {
            int wordStart = cursor;
            while (cursor < n && text[cursor] != '\n' && text[cursor] != ' ') cursor++;
            int wordEnd = cursor;
            int wordLen = wordEnd - wordStart;

            while (cursor < n && text[cursor] == ' ') cursor++;

            std::string candidate = line;
            if (!candidate.empty()) candidate.push_back(' ');
            candidate.append(text.data() + wordStart, (size_t)wordLen);

            if (measureString(candidate) <= rect.width) {
                line.swap(candidate);
                continue;
            }

            if (!useHyphens) {
                cursor = wordStart;
                break;
            }

            std::string base = line;
            if (!base.empty()) base.push_back(' ');
            int baseW = measureString(base);

            if (baseW > rect.width) {
                cursor = wordStart;
                break;
            }

            int available = rect.width - baseW - hyW;
            if (available <= 0) {
                cursor = wordStart;
                break;
            }

            // Binary search largest prefix that fits
            int lo = 1;
            int hi = wordLen;
            int best = 0;

            while (lo <= hi) {
                int mid = (lo + hi) / 2;
                int w = measureRange(wordStart, mid);
                if (w <= available) {
                    best = mid;
                    lo = mid + 1;
                } else {
                    hi = mid - 1;
                }
            }

            if (best <= 0) {
                cursor = wordStart;
                break;
            }

            if (best < MIN_HYPHEN_PREFIX) {
                cursor = wordStart;
                break;
            }

            line = base;
            line.append(text.data() + wordStart, (size_t)best);
            if (best < wordLen) line.push_back('-');

            cursor = wordStart + best;
            break;
        }

        while (!line.empty() && line.back() == ' ') line.pop_back();
        lines.push_back(line);

        if (cursor < n && text[cursor] == '\n') cursor++;
    }

    const bool truncatedVertically = (cursor < n);

    if (truncatedVertically && useEllipses && !lines.empty()) {
        std::string& last = lines.back();

        if ((int)last.size() >= 3) {
            last.resize(last.size() - 3);
            last += "...";
        } else {
            last = "...";
        }

        while (measureString(last) > rect.width) {
            if ((int)last.size() <= 3) break;
            last.erase(last.size() - 4, 1);
        }

        if (measureString(last) > rect.width) last.clear();
    }

    enum class HAlign { Left, Center, Right };
    enum class VAlign { Top, Middle, Bottom };

    auto anchorToH = [&](Anchor a) -> HAlign {
        switch (a) {
            case Anchor::TopLeft:
            case Anchor::CenterLeft:
            case Anchor::BottomLeft:
                return HAlign::Left;

            case Anchor::TopCenter:
            case Anchor::Center:
            case Anchor::BottomCenter:
                return HAlign::Center;

            case Anchor::TopRight:
            case Anchor::CenterRight:
            case Anchor::BottomRight:
                return HAlign::Right;
        }
        return HAlign::Left;
    };

    auto anchorToV = [&](Anchor a) -> VAlign {
        switch (a) {
            case Anchor::TopLeft:
            case Anchor::TopCenter:
            case Anchor::TopRight:
                return VAlign::Top;

            case Anchor::CenterLeft:
            case Anchor::Center:
            case Anchor::CenterRight:
                return VAlign::Middle;

            case Anchor::BottomLeft:
            case Anchor::BottomCenter:
            case Anchor::BottomRight:
                return VAlign::Bottom;
        }
        return VAlign::Top;
    };

    HAlign hAlign = anchorToH(anchor);
    VAlign vAlign = anchorToV(anchor);

    const int visibleLines = (int)lines.size();
    if (visibleLines <= 0) return;

    const int blockHeight = visibleLines * lineHeight;

    int blockTop = rect.position.y;
    if (vAlign == VAlign::Middle) blockTop = rect.position.y + (rect.height - blockHeight) / 2;
    if (vAlign == VAlign::Bottom) blockTop = rect.position.y + (rect.height - blockHeight);

    for (int i = 0; i < visibleLines; ++i) {
        int drawY = blockTop + i * lineHeight;
        if (drawY + lineHeight > rect.position.y + rect.height) break;

        const std::string& line = lines[i];
        if (line.empty()) continue;

        int lineW = context->getTextWidth(line.c_str(), textPoint);

        int drawX = rect.position.x;
        if (hAlign == HAlign::Center) drawX = rect.position.x + (rect.width - lineW) / 2;
        if (hAlign == HAlign::Right)  drawX = rect.position.x + rect.width - lineW;

        context->drawText(line.c_str(), drawX, drawY, textColor, textPoint);
    }
}


void Graphics::drawTextArea(const std::string &text, Rect<int> rectangle, Anchor anchor, bool useEllipses, bool useHyphens) { drawTextArea(text, rectangle.position.x, rectangle.position.y, rectangle.width, rectangle.height, anchor, useEllipses, useHyphens); }

void Graphics::fillEllipse(int x, int y, int width, int height) const {
    fillEllipse(Rect<int>(x, y, width, height));
}

void Graphics::fillEllipse(const Rect<int> &area) const {
    const auto center = area.getCenter();
    context->fillEllipse(center.x, center.y, area.width / 2, area.height / 2, fillValue);
}

void Graphics::strokeEllipse(int x, int y, int width, int height) const {
    strokeEllipse(Rect<int>(x, y, width, height));
}

void Graphics::strokeEllipse(const Rect<int> &area) const {
    const auto center = area.getCenter();
    context->strokeEllipse(center.x, center.y, area.width / 2, area.height / 2, strokeValue, 1);
}
