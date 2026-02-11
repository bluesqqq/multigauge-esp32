#include "CircularNeedle.h"

CircularNeedle::CircularNeedle(Element* parent) : CircularElement(parent) {}

CircularNeedle::CircularNeedle(Element* parent, const rapidjson::Value::ConstObject json) : CircularElement(parent, json) {
    if (!json.HasMember("props") || !json["props"].IsObject()) return;
    const rapidjson::Value::ConstObject props = json["props"].GetObject();

    setFloat(props, "radius", radius);
    setObj(props, "color", color);
}

void CircularNeedle::draw(Graphics &g) const {
    const auto& b = getBounds();

    const float w = b.width;
    const float h = b.height;
    const float diameter = std::min(w, h);
    const float radius   = diameter * 0.5f * this->radius;

    const float cx = b.position.x + w * 0.5f;
    const float cy = b.position.y + h * 0.5f;

    const DisplayValue& value = resolvedDisplayValue();
    float startAngle = resolvedStartAngle() * DEG_TO_RAD;
    float endAngle = resolvedEndAngle() * DEG_TO_RAD;

    float needleAngle = mapf(value.getInterpolationValue(), 0.0f, 1.0f, startAngle, endAngle);

    Line<float> needle(cx, cy, cx + cos(needleAngle) * radius, cy + sin(needleAngle) * radius);
    g.setFillStroke(color);
    g.fillWideLine(needle.toInt(), 10);

    g.drawText(value.getValueString(true), {cx, cy}, Anchor::Center);
    /*
    if (color.fill) {
        g.setFill(*color.fill);
        g.fillCircle(cx, cy, radius);
    }

    if (color.stroke) {
        g.setStroke(*color.stroke);
        g.setStrokeThickness(color.thickness);
        g.strokeCircle(cx, cy, radius);
    }
    */
}
