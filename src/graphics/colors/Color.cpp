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

std::unique_ptr<Color> Color::fromJson(const rapidjson::Value& json) {
    if (json.IsUint()) { return std::make_unique<StaticColor>((uint16_t)json.GetUint()); }

    if (json.IsInt()) {
        int v = json.GetInt();
        if (v < 0) v = 0;
        if (v > 65535) v = 65535;
        return std::make_unique<StaticColor>((uint16_t)v);
    }

    // ---- Must be an object for typed colors ----
    if (!json.IsObject()) return std::make_unique<StaticColor>();

    const auto obj = json.GetObject();

    // If no "type", treat as StaticColor object shorthand:
    // { "color": 6470 } or { "value": 6470 }
    if (!obj.HasMember("type") || !obj["type"].IsString()) return std::make_unique<StaticColor>();

    const char* type = obj["type"].GetString();

    if (strcmp(type, "value")  == 0) return std::make_unique<ValueColor>(obj);
    if (strcmp(type, "time")   == 0) return std::make_unique<TimeColor>(obj);
    if (strcmp(type, "user")   == 0) return std::make_unique<StaticColor>();

    return std::make_unique<StaticColor>();
}

//----------[ FILL STROKE ]----------//

FillStroke::FillStroke() : fill(nullptr), stroke(nullptr) {}

FillStroke::FillStroke(std::unique_ptr<Color> fill, std::unique_ptr<Color> stroke, float thickness) : fill(std::move(fill)), stroke(std::move(stroke)), thickness(thickness) {}

FillStroke::FillStroke(const rapidjson::Value::ConstObject json)
    : fill((json.HasMember("fill") ? Color::fromJson(json["fill"]) : nullptr)),
      stroke((json.HasMember("stroke") ? Color::fromJson(json["stroke"]) : nullptr)),
      thickness((json.HasMember("thickness") && json["thickness"].IsNumber()) ? json["thickness"].GetFloat() :  1.0f) {}

FillStroke FillStroke::blended(uint16_t c, float alpha) const { return FillStroke((fill) ? fill->blended(c, alpha) : nullptr, (stroke) ? stroke->blended(c, alpha) : nullptr, thickness); }

FillStroke FillStroke::blended(const Color &c, float alpha) const { return FillStroke((fill) ? fill->blended(c, alpha) : nullptr, (stroke) ? stroke->blended(c, alpha) : nullptr, thickness); }