#include "UserColor.h"
#include "StaticColor.h"

UserColor::UserColor(Slot slot) : slot(slot) { }

UserColor::UserColor(JsonObject userColorJson) {
    if (userColorJson["slot"].is<const char*>()) {
        std::string slotStr(userColorJson["slot"].as<const char*>());

        if (slotStr == "primary")       slot = Slot::Primary;
        else if (slotStr == "secondary") slot = Slot::Secondary;
        else                             slot = Slot::Background;
    } else {
        slot = Slot::Background;
    }
}

uint16_t UserColor::getColor() const { return userColors[static_cast<size_t>(slot)]; }

Color::Type UserColor::getType() const { return Type::User; }

std::unique_ptr<Color> UserColor::blended(uint16_t color, float alpha) const { return std::make_unique<StaticColor>(Color::blend(getColor(), color, alpha)); }

std::unique_ptr<Color> UserColor::blended(const Color &other, float alpha) const { return other.blended(getColor(), alpha); }