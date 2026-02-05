#include "RectangleElement.h"

RectangleElement::RectangleElement(YGConfigRef config) : Element(config) {}

RectangleElement::RectangleElement(YGConfigRef config, const rapidjson::Value::ConstObject json) : Element(config, json) {
    if (!json.HasMember("props") || !json["props"].IsObject()) return;
    const rapidjson::Value::ConstObject props = json["props"].GetObject();

    if (props.HasMember("color") && props["color"].IsObject())
        color = FillStroke(props["color"].GetObject());

    if (props.HasMember("radius") && props["radius"].IsNumber())
        radius = props["radius"].GetFloat();
}

void RectangleElement::draw(Graphics &g) const {
    const auto& b = getBounds().toInt();

    if (color.fill) {
        g.setFill(*color.fill);
        if (radius > 0.0f) g.fillRoundedRect(b, radius);
        else g.fillRect(b);
    }

    if (color.stroke) {
        g.setStroke(*color.stroke);
        g.setStrokeThickness(color.thickness);
        if (radius > 0.0f) g.strokeRoundedRect(b, radius);
        else g.strokeRect(b);
    }
}