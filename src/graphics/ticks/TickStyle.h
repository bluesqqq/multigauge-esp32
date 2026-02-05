#pragma once

#include "graphics/colors/ColorTimeline.h"
#include "graphics/TextStyle.h"

#include "utils.h"

enum TickStyle { LINE, TRIANGLE, CIRCLE };

struct TickValueStyle {
    ColorTimeline color;

    TextStyle textStyle;

    int spacing;

    bool flipValuesPosition;

    TickValueStyle(const rapidjson::Value::ConstObject& json) {
        if (json.HasMember("color") && json["color"].IsArray()) color = ColorTimeline(json["color"].GetArray());

        setObj(json, "textStyle", textStyle);
        setInt(json, "spacing", spacing);
        setBool(json, "flipValuesPosition", flipValuesPosition);
    }
};