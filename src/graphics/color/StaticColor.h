#pragma once

#include "color.h"

class StaticColor : public Color {
    private:
        // Static 16-bit (RGB565) color value
        uint16_t color;
    
    public:
        /// @brief Constructs a StaticColor with default color
        StaticColor();

        /// @brief Constructs a StaticColor with a specified color value.
        /// @param color The 16-bit color value
        StaticColor(uint32_t color);

        /// @brief Constructs a StaticColor from JSON.
        /// @param staticColorJson The JSON object containing color data
        StaticColor(JsonObject staticColorJson);

        /// @brief Gets the static color value.
        /// @return The 16-bit color value
        uint16_t getColor() const override;

        /// @brief Gets the type of this color.
        /// @return Type::Static
        Type getType() const override;

        /// @brief Blends this color with a static color value.
        /// @param color The 16-bit color value to blend with
        /// @param alpha The blend amount (0.0 = this color, 1.0 = blend color)
        /// @return A new StaticColor object with the blended result
        std::unique_ptr<Color> blended(uint16_t color, float alpha) const override;

        /// @brief Blends this color with another Color object.
        /// @param color The Color object to blend with
        /// @param alpha The blend amount (0.0 = this color, 1.0 = other color)
        /// @return A new Color object of the same derived type as the input (e.g., blending with a TimeColor returns a TimeColor)
        std::unique_ptr<Color> blended(const Color& other, float alpha) const override;

};