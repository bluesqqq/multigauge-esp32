#include <TFT_eSPI.h>
#include "GraphicsContext.h"

class GraphicsContextTFT_eSPI : public GraphicsContext {
    private:
        TFT_eSPI display;
        TFT_eSprite buffer;

        uint16_t tftColor(const rgba& c) { return display.color565(c.r, c.g, c.b); }

    public:
        GraphicsContextTFT_eSPI() : display(TFT_eSPI()), buffer(&this->display) {}

        void init() override;

        void endFrame() override;

        void fillRect(int x, int y, int w, int h, rgba color) override;
        void strokeRect(int x, int y, int w, int h, rgba color, float thickness) override;
        void fillCircle(int cx, int cy, int r, rgba color) override;
        void strokeCircle(int cx, int cy, int r, rgba color, float thickness) override;
        void fillRing(int cx, int cy, int r, rgba color) override;
        void strokeRing(int cx, int cy, int r, rgba color, float thickness) override;
        void fillEllipse(int cx, int cy, int rx, int ry, rgba color) override;
        void strokeEllipse(int cx, int cy, int rx, int ry, rgba color, float thickness) override;
        void fillAll(rgba color) override;
};