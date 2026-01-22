#pragma once

#include "colors/Color.h"
#include "geometry/alignment.h"
#include "geometry/Rectangle.h"
#include "geometry/point.h"
#include "geometry/Line.h"
#include "geometry/path.h"

#include "contexts/GraphicsContext.h"

#define MIN_HYPHEN_PREFIX 3 // Prefix must be this number of chars or higher to be hyphenated in text wrap

class Graphics final {
    private:
        GraphicsContext* context;

        //----------[ COLORS ]----------//

        Color* fill;
        uint16_t fillValue;

        Color* stroke;
        uint16_t strokeValue;

        /// @brief Cache that stores a color's value for quick lookup
        std::unordered_map<Color*, uint16_t> colorCache;

        /// @brief Gets a color's value from the cache.
        /// @param color The Color object to get the color value of
        /// @return The current Color object's 16-bit color value
        uint16_t getCachedColor(Color* color);

        //----------[ TEXT ]----------//

        float textPoint = 10.0f;

    public:
        Graphics(GraphicsContext* context);

        void clearColorCache();

        Rectangle<int> getScreenBounds();
        
        void setFill(uint16_t color);
        void setFill(Color* color);

        void setStroke(uint16_t color);
        void setStroke(Color* color);



        void fillAll() const;
        void fillAll(Color* color);
        void fillAll(uint16_t color) const;

        //----------[ PRIMITIVES ]----------//

        void fillRect(int x, int y, int width, int height);
        void fillRect(const Rectangle<int>& rectangle);

        void strokeRect(int x, int y, int width, int height);
        void strokeRect(const Rectangle<int>& rectangle);

        void fillRoundedRect(int x, int y, int width, int height, int cornerSize = 0);
        void fillRoundedRect(const Rectangle<int>&, int cornerSize = 0);

        void strokeRoundedRect(int x, int y, int width, int height, int cornerSize = 0);
        void strokeRoundedRect(const Rectangle<int>&, int cornerSize = 0);

        void fillEllipse(int x, int y, int width, int height) const;
        void fillEllipse(const Rectangle<int>& area) const;

        void strokeEllipse(int x, int y, int width, int height) const;
        void strokeEllipse(const Rectangle<int>& area) const;

        void fillCircle(int cx, int cy, int radius) const;
        void fillCircle(const Point<int>& center, int radius) const;

        void strokeCircle(int cx, int cy, int radius) const;
        void strokeCircle(const Point<int>& center, int radius) const;

        void fillRing(int cx, int cy, int r1, int r2) const;
        void fillRing(const Point<int>& center, int r1, int r2) const;

        void strokeRing(int cx, int cy, int r1, int r2) const;
        void strokeRing(const Point<int>& center, int r1, int r2) const;

        void strokeLine(int x0, int y0, int x1, int y1) const;
        void strokeLine(const Point<int>& p1, const Point<int>& p2) const;
        void strokeLine(const Line<int>& line) const;

        void fillArc(int cx, int cy, int r1, int r2, float startAngle, float endAngle);
        void fillArc(Point<int> center, int r1, int r2, float startAngle, float endAngle);

        void strokeArc(int cx, int cy, int r1, int r2, float startAngle, float endAngle);
        void strokeArc(Point<int> center, int r1, int r2, float startAngle, float endAngle);

        void fillPath(const Path<int>& path);
        void strokePath(const Path<int>& path);

        //----------[ TEXT ]----------//

        void setTextPoint(float point);

        void drawText(const std::string& text, int x, int y, Anchor anchor);
        void drawText(const std::string& text, Point<int> pos, Anchor anchor);
        void drawTextVertical(const std::string& text, int x, int y, Anchor anchor);

        void drawTextArea(const std::string& text, int x, int y, int width, int height, Anchor anchor, bool useEllipses = true, bool useHyphens = false);
        void drawTextArea(const std::string& text, Rectangle<int> rectangle, Anchor anchor, bool useEllipses = true, bool useHyphens = false);

        //----------[ IMAGES ]----------//

        void drawImage(const Rectangle<int>& area);
};