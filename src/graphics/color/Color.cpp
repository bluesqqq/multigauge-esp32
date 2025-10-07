#include "color.h"

#include "StaticColor.h"
#include "ValueColor.h"
#include "TimeColor.h"

const ColorTimeline * Color::getTimeline() const { return nullptr; }

uint16_t Color::blend(const uint16_t &base, const uint16_t &blend, float alpha) {
    uint8_t r1 = (base >> 11) & 0x1F;
    uint8_t g1 = (base >> 5)  & 0x3F;
    uint8_t b1 = base & 0x1F;

    uint8_t r2 = (blend >> 11) & 0x1F;
    uint8_t g2 = (blend >> 5)  & 0x3F;
    uint8_t b2 = blend & 0x1F;

    uint8_t r = r1 + (r2 - r1) * alpha;
    uint8_t g = g1 + (g2 - g1) * alpha;
    uint8_t b = b1 + (b2 - b1) * alpha;

    return (r << 11) | (g << 5) | b;
}

std::unique_ptr<Color> Color::loadFromJson(JsonObject colorJson) {
    if (!colorJson["type"].is<const char*>()) return std::make_unique<StaticColor>();
    
    std::string type = std::string(colorJson["type"].as<const char*>());

    if (type == "static")     return std::make_unique<StaticColor>(colorJson);
    else if (type == "value") return std::make_unique<ValueColor>(colorJson);
    else if (type == "time")  return std::make_unique<TimeColor>(colorJson);
    else if (type == "user")  return std::make_unique<StaticColor>(colorJson);

    return std::make_unique<StaticColor>();
}