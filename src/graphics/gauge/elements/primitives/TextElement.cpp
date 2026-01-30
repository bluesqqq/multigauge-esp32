#include "TextElement.h"

TextElement::TextElement(YGConfigRef config, std::string text, float point, std::unique_ptr<Color> color) : Element(config), text(text), point(point), color(std::move(color)) {}

TextElement::TextElement(YGConfigRef config, const rapidjson::Value::ConstObject json) : Element(config, json) {
    loadLayout(getNode(), json);

    if (!json.HasMember("props") || !json["props"].IsObject()) return;
    const rapidjson::Value::ConstObject props = json["props"].GetObject();

    if (props.HasMember("text") && props["text"].IsString())
        text = props["text"].GetString();

    if (props.HasMember("point") && props["point"].IsNumber())
        point = props["point"].GetFloat();

    if (props.HasMember("color") && props["color"].IsObject())
        color = Color::fromJson(props["color"].GetObject());

    // TODO: Add anchor parsing

    if (props.HasMember("ellipses") && props["ellipses"].IsBool())
        useEllipses = props["ellipses"].GetBool();

    if (props.HasMember("hyphens") && props["hyphens"].IsBool())
        useHyphens = props["hyphens"].GetBool();
}

void TextElement::draw(Graphics &g) const {
    const auto& b = getBounds();

    if (color) { 
        g.setFill(*color);
        g.drawTextArea(text, b.toInt(), anchor, useEllipses, useHyphens);
    }

    Element::draw(g);
}