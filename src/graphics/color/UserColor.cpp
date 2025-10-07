#include "UserColor.h"
#include "StaticColor.h"

UserColor::UserColor(Slot slot) : slot(slot) { }

UserColor::UserColor(JsonObject userColorJson) {
    std::string slot = std::string(userColorJson["slot"].as<const char*>());

    if (slot == "primary")     this->slot = Slot::Primary;
    else if (slot == "secondary") this->slot = Slot::Secondary;
    else this->slot = Slot::Background;
}

uint16_t UserColor::getColor() const { return userColors[static_cast<size_t>(slot)]; }

Color::Type UserColor::getType() const { return Type::User; }

std::unique_ptr<Color> UserColor::blended(uint16_t color, float alpha) const { return std::make_unique<StaticColor>(Color::blend(getColor(), color, alpha)); }

std::unique_ptr<Color> UserColor::blended(const Color &other, float alpha) const { return other.blended(getColor(), alpha); }