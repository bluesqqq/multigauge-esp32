#include "CircleElement.h"

CircleElement::CircleElement(YGConfigRef config) : Element(config) {}

CircleElement::CircleElement(YGConfigRef config, const rapidjson::Value::ConstObject json) : Element(config, json) {
    loadLayout(getNode(), json);

    if (!json.HasMember("props") || !json["props"].IsObject()) return;
    const rapidjson::Value::ConstObject props = json["props"].GetObject();

    if (props.HasMember("color") && props["color"].IsObject())
        color = FillStroke(props["color"].GetObject());
}

void CircleElement::draw(Graphics &g) const {
    const auto& b = getBounds();

    const float w = b.width;
    const float h = b.height;
    const float diameter = std::min(w, h);
    const float radius   = diameter * 0.5f;

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

    Element::draw(g);
}