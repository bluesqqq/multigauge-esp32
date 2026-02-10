#include "CircularGroup.h"

CircularGroup::CircularGroup(Element* parent) : Element(parent) {}

CircularGroup::CircularGroup(Element* parent, const rapidjson::Value::ConstObject json) : Element(parent, json) {
    if (!json.HasMember("props") || !json["props"].IsObject()) return;
    const rapidjson::Value::ConstObject props = json["props"].GetObject();

    //setObj(json, "value", value);
    //setFloat(json, "startAngle", startAngle);
    //setFloat(json, "endAngle", endAngle);
}