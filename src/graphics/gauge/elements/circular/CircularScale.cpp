#include "CircularScale.h"

CircularScale::CircularScale(Element* parent, const rapidjson::Value::ConstObject json) : Element(parent, json) {
    if (!json.HasMember("props") || !json["props"].IsObject()) return;
    const rapidjson::Value::ConstObject props = json["props"].GetObject();

    //setFloat(json, "radius", radius);
    //setObj(json, "ticks", ticks);
    //setObj(json, "value", value);
}