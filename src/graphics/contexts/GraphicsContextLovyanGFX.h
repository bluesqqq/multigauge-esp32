#include "LGFX_definition.h"
#include "GraphicsContext.h"

class GraphicsContextLovyanGFX : public GraphicsContext {
    private:
        LGFX display;
        LGFX_Sprite buffer;

        lgfx::v1::textdatum::textdatum_t anchorToDatum(Anchor anchor);

    public:
        GraphicsContextLovyanGFX() : display(LGFX()), buffer(&this->display) {}

        void init() override;

        void endFrame() override;

        void strokeLine(int x0, int y0, int x1, int y1, uint16_t color) override;
        void fillWideLine(int x0, int y0, int x1, int y1, uint16_t color, float thickness = 1) override;

        void strokeTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color) override;
        void fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color) override;

        void fillRect(int x, int y, int w, int h, uint16_t color) override;
        void strokeRect(int x, int y, int w, int h, uint16_t color, float thickness) override;
        void fillCircle(int cx, int cy, int r, uint16_t color) override;
        void strokeCircle(int cx, int cy, int r, uint16_t color, float thickness) override;
        void fillRing(int cx, int cy, int r, uint16_t color) override;
        void strokeRing(int cx, int cy, int r, uint16_t color, float thickness) override;
        void fillEllipse(int cx, int cy, int rx, int ry, uint16_t color) override;
        void strokeEllipse(int cx, int cy, int rx, int ry, uint16_t color, float thickness) override;
        void fillAll(uint16_t color) override;

        float getTextWidth(const char* text, float point) override;
        void drawText(const char* text, int x, int y, uint16_t color, float point, Anchor anchor = Anchor::TopLeft) override;
};