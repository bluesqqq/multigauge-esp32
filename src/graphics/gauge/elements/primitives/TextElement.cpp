#include "TextElement.h"

TextElement::TextElement(YGConfigRef config, std::string text) : Element(config), text(text) {
    
}

TextElement::TextElement(YGConfigRef config, const rapidjson::Value::ConstObject json) : Element(config, json) {
    loadLayout(getNode(), json);

    if (!json.HasMember("props") || !json["props"].IsObject()) return;
    const rapidjson::Value::ConstObject props = json["props"].GetObject();

    if (props.HasMember("text") && props["text"].IsString())
        text = props["text"].GetString();

    if (props.HasMember("textStyle") && props["textStyle"].IsObject())
        textStyle =TextStyle(props["textStyle"].GetObject());

    // TODO: Add anchor parsing

    if (props.HasMember("ellipses") && props["ellipses"].IsBool())
        useEllipses = props["ellipses"].GetBool();

    if (props.HasMember("hyphens") && props["hyphens"].IsBool())
        useHyphens = props["hyphens"].GetBool();
}

void TextElement::draw(Graphics &g) const {
    const auto& b = getBounds();

    if (textStyle.color) { 
        g.setTextStyle(textStyle);

        if (text.find('{') != std::string::npos) {
            const std::string expanded = embed_render::replaceEmbeds(text);
            g.drawTextArea(expanded, b.toInt(), anchor, useEllipses, useHyphens);
        } else {
            g.drawTextArea(text, b.toInt(), anchor, useEllipses, useHyphens);
        }
    }

    Element::draw(g);
}
