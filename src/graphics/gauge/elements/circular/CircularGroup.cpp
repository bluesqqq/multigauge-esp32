#include "CircularGroup.h"

CircularGroup::CircularGroup(YGConfigRef config) : Element(config) {}

CircularGroup::CircularGroup(YGConfigRef config, const rapidjson::Value::ConstObject json) : Element(config, json) {
    if (!json.HasMember("props") || !json["props"].IsObject()) return;
        const rapidjson::Value::ConstObject props = json["props"].GetObject();

    setObj(json, "value", value);
    setFloat(json, "startAngle", startAngle);
    setFloat(json, "endAngle", endAngle);
}