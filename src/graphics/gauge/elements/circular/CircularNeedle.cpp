#include "CircularNeedle.h"
#include "CircularGroup.h"

CircularNeedle::CircularNeedle(YGConfigRef config) : Element(config) {}

CircularNeedle::CircularNeedle(YGConfigRef config, const rapidjson::Value::ConstObject json) : Element(config, json) {
    if (!json.HasMember("props") || !json["props"].IsObject()) return;
    const rapidjson::Value::ConstObject props = json["props"].GetObject();

    setFloat(json, "radius", radius);

    setObj(json, "color", color);
}

void CircularNeedle::draw(Graphics &g) const {
    const auto& b = getBounds();

    const float w = b.width;
    const float h = b.height;
    const float diameter = std::min(w, h);
    const float radius   = diameter * 0.5f * this->radius;

    const float cx = b.position.x + w * 0.5f;
    const float cy = b.position.y + h * 0.5f;

    if (color.fill) {
        g.setFill(*color.fill);
        g.fillCircle(cx, cy, radius);
    }

    if (color.stroke) {
        g.setStroke(*color.stroke);
        g.setStrokeThickness(color.thickness);
        g.strokeCircle(cx, cy, radius);
    }
}
