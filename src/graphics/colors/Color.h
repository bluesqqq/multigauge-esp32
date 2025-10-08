#pragma once

#include <ArduinoJson.h>
#include <memory>
#include "values/value.h"

#define DEFAULT_COLOR 0

class ColorTimeline;

class Color {
    public:
        /// @brief Creates a clone of this Color object
        /// @return A unique pointer to the created Color object
        virtual std::unique_ptr<Color> clone() const = 0;

        /// @brief Blends two 16-bit (RGB565) color values together.
        /// @param base The base color value
        /// @param blend The color value to blend with
        /// @param alpha The blend amount (0.0 = base, 1.0 = blend)
        /// @return The blended 16-bit color value
        static uint16_t blend(const uint16_t& base, const uint16_t& blend, float alpha);

        /// @brief Creates a Color object from JSON.
        /// @param colorJson The JSON object containing Color data
        /// @return A unique pointer to the created Color object (defaults to StaticColor if type is missing or invalid)
        static std::unique_ptr<Color> loadFromJson(JsonObject colorJson);

        /// @brief Gets the current color value.
        /// @return The 16-bit color value
        virtual uint16_t getColor() const = 0;

        /// @brief Gets the color timeline if this color type has one.
        /// @return Pointer to ColorTimeline, or nullptr if this color has no timeline
        virtual const ColorTimeline* getTimeline() const;

        /// @brief Color type identifiers
        enum class Type { Static, Value, Time, User };

        /// @brief Gets the type of this color.
        /// @return The Type enum value
        virtual Type getType() const = 0;

        /// @brief Blends this color with a static color value.
        /// @param color The 16-bit color value to blend with
        /// @param alpha The blend amount (0.0 = this color, 1.0 = blend color)
        /// @return A new Color object with the blended result
        virtual std::unique_ptr<Color> blended(uint16_t color, float alpha) const = 0;

        /// @brief Blends this color with another Color object.
        /// @param color The Color object to blend with
        /// @param alpha The blend amount (0.0 = this color, 1.0 = other color)
        /// @return A new Color object with the blended result
        virtual std::unique_ptr<Color> blended(const Color& color, float alpha) const = 0;
};