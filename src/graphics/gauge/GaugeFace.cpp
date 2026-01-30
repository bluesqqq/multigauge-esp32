#include "GaugeFace.h"

GaugeFace::GaugeFace(YGConfigRef config) : Element(config) {}

GaugeFace::GaugeFace(YGConfigRef config, const rapidjson::Document& json) : Element(config, json.GetObject()) {
    loadLayout(getNode(), json.GetObject());

    if (!json.HasMember("props") || !json["props"].IsObject()) return;
    const rapidjson::Value::ConstObject props = json["props"].GetObject();

    if (props.HasMember("backgroundColor") && props["backgroundColor"].IsObject())
        backgroundColor = Color::fromJson(props["backgroundColor"].GetObject());

    if (props.HasMember("title") && props["title"].IsString())
        title = props["title"].GetString();

    if (props.HasMember("description") && props["description"].IsString())
        description = props["description"].GetString();
}