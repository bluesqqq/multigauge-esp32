#include "LGFX_definition.h"
#include "GraphicsContext.h"

#include <algorithm>

//----------[ FONT REGISTRY SYSTEM ]----------//

#define DEFAULT_LGFX_FONT lgfx::fonts::Font2
#define DEFAULT_LGFX_FONT_PT 16

struct LGFXFontPick {
    const lgfx::IFont* font = nullptr;
    float strokePt = 0.0f; // the base point size that was selected
};

struct LGFXFontStroke {
    const lgfx::IFont* normal = nullptr;
    const lgfx::IFont* bold   = nullptr;
    const lgfx::IFont* italic = nullptr;
    const lgfx::IFont* boldItalic = nullptr;

    const lgfx::IFont* getFont(bool b, bool i) const;
};

class LGFXFontFamily {
    private:
        std::vector<std::pair<float, LGFXFontStroke>> strokes;

        auto findStrokeForPt(float pt) const;

    public:
        void addStroke(float pt, LGFXFontStroke stroke);

        LGFXFontPick getFont(float pt, bool wantBold, bool wantItalic) const;
};


class GraphicsContextLovyanGFX : public GraphicsContext {
    private:
        LGFX display;
        LGFX_Sprite buffer;

        float currentFontPoint;
        std::unordered_map<std::string, LGFXFontFamily> fontFamilies;

        float getFontScaleFactor(float point);

        lgfx::v1::textdatum::textdatum_t anchorToDatum(Anchor anchor);

        void setFontInternal(std::string family, float pt, FontWeight weight, FontSlant slant);

    public:
        GraphicsContextLovyanGFX() : display(LGFX()), buffer(&this->display) {}

        void init() override;

        void endFrame() override;

        //----------[ FILL ]----------//
        void fillAll(uint16_t color) override;

        //----------[ PIXEL ]----------//
        void drawPixel(int x, int y, uint16_t color) override;

        //----------[ LINE ]----------//
        void strokeLine(int x0, int y0, int x1, int y1, uint16_t color) override;
        void fillWideLine(int x0, int y0, int x1, int y1, uint16_t color, float thickness) override;

        //----------[ RECTANGLE ]----------//
        void fillRect(int x, int y, int w, int h, uint16_t color) override;
        void strokeRect(int x, int y, int w, int h, uint16_t color, float thickness) override;

        void fillRoundRect(int x, int y, int w, int h, float radius, uint16_t color) override;
        void fillRoundRect(int x, int y, int w, int h, float r1, float r2, float r3, float r4, uint16_t color) override;

        void strokeRoundRect(int x, int y, int w, int h, float radius, uint16_t color, float thickness) override;
        void strokeRoundRect(int x, int y, int w, int h, float r1, float r2, float r3, float r4, uint16_t color, float thickness) override;
        
        //----------[ CIRCLE ]----------//
        void fillCircle(int cx, int cy, int r, uint16_t color) override;
        void strokeCircle(int cx, int cy, int r, uint16_t color, float thickness) override;

        //----------[ ELLIPSE ]----------//
        void fillEllipse(int cx, int cy, int rx, int ry, uint16_t color) override;
        void strokeEllipse(int cx, int cy, int rx, int ry, uint16_t color, float thickness) override;

        //----------[ RING ]----------//
        void fillRing(int cx, int cy, int r, uint16_t color) override;
        void strokeRing(int cx, int cy, int r, uint16_t color, float thickness) override;

        //----------[ ARC ]----------//
        void fillArc(int cx, int cy, int r1, int r2, float start, float end, uint16_t color) override;
        void strokeArc(int cx, int cy, int r1, int r2, float start, float end, uint16_t color, float thickness) override;

        //----------[ TRIANGLE ]----------//
        void strokeTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color, float thickness) override;
        void fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color) override;

        //----------[ TEXT ]----------//
        float getTextWidth(const char* text, std::string family, float pt, FontWeight weight, FontSlant slant) override;
        void drawText(const char* text, int x, int y, std::string family, float pt, FontWeight weight, FontSlant slant, uint16_t color, Anchor anchor = Anchor::TopLeft) override;

        //----------[ IMAGE ]----------//
        Image createNativeImage(const uint16_t* pixels, int w, int h) override;

        void drawImage(const Image& img, int x, int y, Anchor anchor = Anchor::TopLeft) override;
        void drawImage(const Image& img, int x, int y, int width, int height) override;

        //----------[ CLIP ]----------//
        void setClipRect(int x, int y, int width, int height) override;
        void clearClipRect() override;
};