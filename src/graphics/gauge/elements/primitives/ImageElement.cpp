#include "ImageElement.h"

ImageElement::ImageElement(YGConfigRef config) : Element(config), path("/placeholder.bmp") { }

ImageElement::ImageElement(YGConfigRef config, const rapidjson::Value::ConstObject json) : Element(config, json) {
    if (!json.HasMember("props") || !json["props"].IsObject()) return;
    const rapidjson::Value::ConstObject props = json["props"].GetObject();

    if (props.HasMember("path") && props["path"].IsString())
        path = props["path"].GetString();
}

bool ImageElement::init(AssetManager &assetManager) {
    return assetManager.loadImage(path, image);
}

void ImageElement::draw(Graphics &g) const {
    const auto& b = getBounds().toInt();

    g.drawImage(image, b);
}
