#include "StaticColor.h"

StaticColor::StaticColor() : color(DEFAULT_COLOR) { }

StaticColor::StaticColor(uint32_t color) : color(color) {}

StaticColor::StaticColor(JsonObject staticColorJson) : color(staticColorJson["color"].is<uint16_t>() ? staticColorJson["color"].as<uint16_t>() : 0) { }

std::unique_ptr<Color> StaticColor::blended(uint16_t color, float alpha) const { return std::make_unique<StaticColor>(Color::blend(this->color, color, alpha)); }

std::unique_ptr<Color> StaticColor::blended(const Color &other, float alpha) const { return other.blended(this->color, alpha); }

uint16_t StaticColor::getColor() const { return color; }

Color::Type StaticColor::getType() const { return Type::Static; }
