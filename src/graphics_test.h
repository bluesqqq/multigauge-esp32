// graphics_test_primitives.h
#pragma once

#include <multigauge/graphics/Graphics.h>
#include <multigauge/AssetManager.h>

#include <multigauge/geometry/Rect.h>
#include <cmath>

// Simple animated value 0..1
static inline float pingpong01(float t) {
    float x = fmodf(t, 1.0f);
    return x < 0.5f ? x * 2.0f : 2.0f - x * 2.0f;
}

static inline float clamp01(float v) {
    return v < 0.0f ? 0.0f : (v > 1.0f ? 1.0f : v);
}

static inline void drawBottomCenterLabel(Graphics& g, const Rect<int>& screen, const char* fnName) {
    const int W = screen.width;
    const int H = screen.height;

    const int pillW = 360;
    const int pillH = 30;
    const int x = (W - pillW) / 2;
    const int y = H - pillH - 8;

    g.setPaint(rgba{ 0, 0, 0, 150 }, rgba{ 255, 255, 255, 70 }, 1.0f);
    g.drawRoundedRect(x, y, pillW, pillH, 12.0f);

    g.setTextPaint(rgba{ 255, 255, 255, 255 }, "sans", FontWeight::Bold, FontSlant::Normal, 14.0f);
    g.drawText(fnName, W / 2, y + pillH / 2, Anchor::Center);
}

static inline void drawGraphicsPrimitiveTest(Graphics& g, GraphicsContext& ctx, AssetManager& assets, uint32_t nowUs) {
    (void)ctx;
    (void)assets;

    const Rect<int> screen = g.getScreenBounds();
    const int W = screen.width;
    const int H = screen.height;

    const float t = (float)nowUs / 1000000.0f; // seconds

    constexpr float SECS_PER_PAGE = 3.0f;

    // Size animation (slow and obvious)
    const float s = pingpong01(t / 2.8f);

    // Pulsing stroke thickness (1..10 px)
    const float pulse01 = 0.5f + 0.5f * sinf(t * 2.0f); // 0..1
    const float strokeT = 1.0f + 9.0f * clamp01(pulse01);

    // Colors: one style (blue fill, white stroke)
    const rgba fillBlue   {  40, 120, 255, 220 };
    const rgba strokeWhite{ 255, 255, 255, 230 };

    g.clearColorCache();
    g.fillAll(rgba{ 12, 12, 16, 255 });

    const int dstW = (int)(W * 0.65f);
    const int dstH = (int)(H * 0.55f);

    Rect<int> dst{
        W / 2 - dstW / 2,
        H / 2 - dstH / 2,
        dstW,
        dstH
    };

    enum class Page : int {
        DrawPixel = 0,
        DrawLine,
        DrawRect,
        DrawRoundedRect,
        DrawEllipse,
        DrawCircle,
        DrawRing,
        DrawArc,
        DrawTri,
        Count
    };

    const int pageCount = (int)Page::Count;
    const int page = ((int)std::floor(t / SECS_PER_PAGE)) % pageCount;
    const Page p = (Page)page;

    // Common paint for primitives
    g.setFill(fillBlue);
    g.setStroke(strokeWhite, strokeT);

    // Geometry helpers
    const int cx = dst.x + dst.width / 2;
    const int cy = dst.y + dst.height / 2;

    switch (p) {
        case Page::DrawPixel: {
            // A small pulsing pixel “spark” plus a short diagonal so you can see pixel placement
            g.setStroke(nullptr, 1.0f); // pixel test uses fill for pixel (your drawPixel likely uses fill)
            g.setFill(fillBlue);

            // A tiny diamond of pixels around center
            const int r = (int)(6 + 14 * clamp01(pulse01));
            for (int dy = -r; dy <= r; ++dy) {
                const int span = r - (int)std::abs(dy);
                for (int dx = -span; dx <= span; ++dx) {
                    // light density so it doesn't become a filled blob
                    if (((dx + dy) & 1) == 0) g.drawPixel(cx + dx, cy + dy);
                }
            }

            // Restore stroke so the dst outline is visible
            g.setFill(fillBlue);
            g.setStroke(strokeWhite, strokeT);

            drawBottomCenterLabel(g, screen, "drawPixel");
        } break;

        case Page::DrawLine: {
            // Thick line across dst, angle slowly changes
            const float ang = t * 0.7f; // radians-ish motion
            const float dx = cosf(ang);
            const float dy = sinf(ang);

            const int x0 = (int)(cx - dx * (dst.width  * 0.45f));
            const int y0 = (int)(cy - dy * (dst.height * 0.45f));
            const int x1 = (int)(cx + dx * (dst.width  * 0.45f));
            const int y1 = (int)(cy + dy * (dst.height * 0.45f));

            // Use the pulsing thickness param on the call too (your drawLine has its own thickness arg)
            g.drawLine(x0, y0, x1, y1, strokeT);

            drawBottomCenterLabel(g, screen, "drawLine (thickness pulsing)");
        } break;

        case Page::DrawRect: {
            g.drawRect(dst);
            drawBottomCenterLabel(g, screen, "drawRect");
        } break;

        case Page::DrawRoundedRect: {
            // Radius pulses a bit too
            const float rr = 6.0f + 24.0f * clamp01(pulse01);
            g.drawRoundedRect(dst, rr);
            drawBottomCenterLabel(g, screen, "drawRoundedRect");
        } break;

        case Page::DrawEllipse: {
            // Ellipse inscribed in dst
            g.drawEllipseInRect(dst);
            drawBottomCenterLabel(g, screen, "drawEllipseInRect");
        } break;

        case Page::DrawCircle: {
            // Circle centered in dst, radius based on min dimension
            const int r = (int)(0.45f * (float)std::min(dst.width, dst.height));
            g.drawCircle(cx, cy, r);
            drawBottomCenterLabel(g, screen, "drawCircle");
        } break;

        case Page::DrawRing: {
            // Ring centered in dst
            const int base = (int)(0.40f * (float)std::min(dst.width, dst.height));
            const int r2 = base;
            const int r1 = std::max(1, base - (int)(12 + 18 * clamp01(pulse01))); // ring thickness changes
            g.drawRing(cx, cy, r1, r2);
            drawBottomCenterLabel(g, screen, "drawRing");
        } break;

        case Page::DrawArc: {
            // Arc in a ring band
            const int base = (int)(0.40f * (float)std::min(dst.width, dst.height));
            const int r2 = base;
            const int r1 = std::max(1, base - 24);

            // sweep angle pulses between ~45 and ~320 degrees
            const float sweep = 45.0f + 275.0f * clamp01(pulse01);
            const float start = fmodf(t * 60.0f, 360.0f);
            const float end   = start + sweep;

            g.drawArc(cx, cy, r1, r2, start, end);
            drawBottomCenterLabel(g, screen, "drawArc");
        } break;

        case Page::DrawTri: {
            // Triangle points orbiting inside dst
            const float a0 = t * 0.9f;
            const float a1 = a0 + 2.0943951f; // +120 deg
            const float a2 = a0 + 4.1887902f; // +240 deg

            const float rx = dst.width  * 0.42f;
            const float ry = dst.height * 0.42f;

            const int x0 = (int)(cx + cosf(a0) * rx);
            const int y0 = (int)(cy + sinf(a0) * ry);
            const int x1 = (int)(cx + cosf(a1) * rx);
            const int y1 = (int)(cy + sinf(a1) * ry);
            const int x2 = (int)(cx + cosf(a2) * rx);
            const int y2 = (int)(cy + sinf(a2) * ry);

            g.drawTri(x0, y0, x1, y1, x2, y2);
            drawBottomCenterLabel(g, screen, "drawTri");
        } break;

        default: break;
    }
}