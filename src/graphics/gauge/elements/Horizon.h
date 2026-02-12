#include "graphics/gauge/Element.h"
#include "graphics/colors/StaticColor.h"

class Horizon : public Element {
    private:
        int horizonDensityVertical = 25; // Density variables can be changed to show more/less lines
        int horizonDensityHorizontal = 12;
        int horizonVAngle = 8; // Angle variables have nothing to do with degrees, just a general number to multiply by
        int horizonHAngle = 8;

        float zVelMultiplier = 0.001;
        float xVelMultiplier = 0.003;

        float zPosition = 0;
        float xPosition = 0;

        std::unique_ptr<Color> backgroundColor;
        std::unique_ptr<Color> groundColor;
        std::unique_ptr<Color> horizonColor;
        std::unique_ptr<Color> borderColor;
        
    public:
        Horizon(Element* parent, const rapidjson::Value::ConstObject json) : Element(parent, json) {
            if (!json.HasMember("props") || !json["props"].IsObject()) return;
            const rapidjson::Value::ConstObject props = json["props"].GetObject();

            if (props.HasMember("backgroundColor"))
                backgroundColor = Color::fromJson(props["backgroundColor"]);

            if (props.HasMember("groundColor"))
                groundColor = Color::fromJson(props["groundColor"]);
            
            if (props.HasMember("horizonColor"))
                horizonColor = Color::fromJson(props["horizonColor"]);

            if (props.HasMember("borderColor"))
                borderColor = Color::fromJson(props["borderColor"]);
        }

        void draw(Graphics& g) const override {
            const auto b = getBounds().toInt();

            const int left   = b.getLeft();
            const int top    = b.getTop();
            const int right  = b.getRightPixel();
            const int bottom = b.getBottomPixel();

            const int halfY = b.getCenterY();

            auto ground = Rect<int>(b);
            ground.setTop(halfY);
            ground.reduce(1);

            auto background = Rect<int>(b);
            background.setBottom(halfY);
            background.reduce(1);

            g.setFill(*groundColor);
            g.fillRect(ground);

            g.setFill(*backgroundColor);
            g.fillRect(background);

            g.setStroke(*horizonColor);

            // Horizon line (middle)
            g.strokeLine(left, halfY, right, halfY);

            // Vertical lines
            const float stepX = b.width / float(horizonDensityVertical);
            const float shift = fmodf(xPosition, 1.0f) * stepX;

            for (float i = 0; i < b.width; i += stepX) {
                float posX = left + i + shift;

                float localX = (posX - left);
                float localHalf = b.width / 2.0f;

                float endXLocal = (localX - localHalf) * horizonVAngle + localHalf;
                float endX = left + endXLocal;

                Line<float> raw(posX, (float)halfY, endX, (float)bottom);

                if (auto clipped = raw.intersection(b.toFloat())) {
                    auto li = clipped->toInt();
                    g.strokeLine(li.p1.x, li.p1.y, li.p2.x, li.p2.y);
                }
            }

            // Horizontal lines
            const float maxValue = powf((float)horizonDensityHorizontal, (float)horizonHAngle);

            for (int i = 0; i < horizonDensityHorizontal; i++) {
                float pos = (float)i + fmodf(zPosition, 1.0f);

                float localHalf = b.height / 2.0f;
                int y = (int)(halfY + powf(pos, (float)horizonHAngle) * (localHalf / maxValue));

                g.strokeLine(left, y, right, y);
            }

            g.setStroke(*borderColor);
            g.strokeRect(b);
        }

        void update(int deltaTime) override {
            zPosition += 0; //lateralAcceleration.getValueRaw() * zVelMultiplier; //zValue.getValue(DEFAULT) * zVelMultiplier;
            xPosition -= 0.01f; //speed.getValueRaw() * xVelMultiplier; //xValue.getValue(DEFAULT) * xVelMultiplier;
        }
};

REGISTER_ELEMENT_TYPE("horizon", Horizon);