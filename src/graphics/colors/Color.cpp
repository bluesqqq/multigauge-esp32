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

std::unique_ptr<Color> Color::fromJson(const rapidjson::Value::ConstObject json) {
    if (!json.HasMember("type") || !json["type"].IsString())
        return std::make_unique<StaticColor>();

    const char* type = json["type"].GetString();

    if (strcmp(type, "static") == 0)     return std::make_unique<StaticColor>(json);
    if (strcmp(type, "value") == 0)      return std::make_unique<ValueColor>(json);
    if (strcmp(type, "time") == 0)       return std::make_unique<TimeColor>(json);
    if (strcmp(type, "user") == 0)       return std::make_unique<StaticColor>(json);

    return std::make_unique<StaticColor>();
}

//----------[ FILL STROKE ]----------//

FillStroke::FillStroke() : fill(nullptr), stroke(nullptr) {}

FillStroke::FillStroke(std::unique_ptr<Color> fill, std::unique_ptr<Color> stroke, float thickness) : fill(std::move(fill)), stroke(std::move(stroke)), thickness(thickness) {}

FillStroke::FillStroke(const rapidjson::Value::ConstObject json)
    : fill((json.HasMember("fill") && json["fill"].IsObject() ? Color::fromJson(json["fill"].GetObject()) : nullptr)),
      stroke((json.HasMember("stroke") && json["stroke"].IsObject() ? Color::fromJson(json["stroke"].GetObject()) : nullptr)),
      thickness((json.HasMember("thickness") && json["thickness"].IsNumber()) ? json["thickness"].GetFloat() :  1.0f) {}

FillStroke FillStroke::blended(uint16_t c, float alpha) const { return FillStroke((fill) ? fill->blended(c, alpha) : nullptr, (stroke) ? stroke->blended(c, alpha) : nullptr, thickness); }

FillStroke FillStroke::blended(const Color &c, float alpha) const { return FillStroke((fill) ? fill->blended(c, alpha) : nullptr, (stroke) ? stroke->blended(c, alpha) : nullptr, thickness); }