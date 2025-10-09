#include <TFT_eSPI.h>
#include "GraphicsContext.h"

class GraphicsContextTFT_eSPI : public GraphicsContext {
    private:
        TFT_eSPI display;
        TFT_eSprite buffer;

    public:
        GraphicsContextTFT_eSPI() : display(TFT_eSPI()), buffer(&this->display) {}

        void init() override;

        void fillRectangle(float x, float y, float w, float h, uint16_t color) override;
        void strokeRectangle(float x, float y, float w, float h, uint16_t color, float thickness) override;
        void fillCircle(int cx, int cy, int r, uint16_t color) override;
        void strokeCircle(int cx, int cy, int r, uint16_t color, float thickness) override;
        void fillRing(int cx, int cy, int r, uint16_t color) override;
        void strokeRing(int cx, int cy, int r, uint16_t color, float thickness) override;
        void fillEllipse(int cx, int cy, int rx, int ry, uint16_t color) override;
        void strokeEllipse(int cx, int cy, int rx, int ry, uint16_t color, float thickness) override;
        void fillAll(uint16_t color) override;
};