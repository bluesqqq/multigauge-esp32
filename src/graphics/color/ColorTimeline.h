#pragma once

#include "color.h"

struct ColorKeyframe {
    /// @brief Position in the timeline
    float position;
    /// @brief The color at this position
    std::unique_ptr<Color> color;

    ColorKeyframe(std::unique_ptr<Color> color, float position);

    /// @brief Constructs a color keyframe from JSON.
    /// @param colorKeyframeJson The JSON object containing keyframe data
    ColorKeyframe(JsonObject colorKeyframeJson);

    ColorKeyframe(const ColorKeyframe&) = delete;
    ColorKeyframe& operator=(const ColorKeyframe&) = delete;
};

class ColorTimeline {
    private:
        std::vector<ColorKeyframe> timeline;

        /// @brief Finds the index of the keyframe at or before the given position.
        /// @param position The position to search for
        /// @return The index of the keyframe
        size_t getKeyframeIndexAtPosition(float position) const;

    public:
        ColorTimeline();

        ColorTimeline(uint16_t color);

        /// @brief Constructs a timeline from JSON.
        /// @param colorTimelineJson The JSON object containing timeline data
        ColorTimeline(JsonObject colorTimelineJson);

        /// @brief Blends this timeline with a static color value.
        /// @param color The 16-bit color value to blend with
        /// @param alpha The blend amount (0.0 = this color, 1.0 = blend color)
        /// @return A new ColorTimeline object with the blended result
        ColorTimeline blended(uint16_t color, float alpha) const;

        /// @brief Blends this timeline with another ColorTImeline object.
        /// @param other The ColorTImeline object to blend with
        /// @param alpha The blend amount (0.0 = this color, 1.0 = blend color)
        /// @return A new ColorTimeline object with the blended result
        ColorTimeline blended(const ColorTimeline& other, float alpha) const;
        
        std::vector<ColorKeyframe>& getTimeline();

        /// @brief Adds a StaticColor color keyframe to the timeline
        /// @param color The 16-bit color value
        /// @param position The position in the timeline
        void addKeyframe(uint16_t color, float position);

        /// @brief Adds a color keyframe to the timeline.
        /// @param color The color object for this keyframe
        /// @param position The position in the timeline
        void addKeyframe(std::unique_ptr<Color> color, float position);

        /// @brief Adds a color keyframe to the timeline.
        /// @param keyframe The ColorKeyframe to add
        void addKeyframe(ColorKeyframe keyframe);

        /// @brief Removes all keyframes from the timeline
        void clear();

        /// @brief Gets the number of keyframes in the timeline.
        /// @return The number of keyframes
        size_t size() const;

        /// @brief Gets the interpolated color at a specific position.
        /// @param position The position in the timeline
        /// @return The 16-bit color value at that position
        uint16_t getColor(float position) const;

        /// @brief Gets the position of the first keyframe.
        /// @return The start position, or 0.0 if timeline is empty
        float getStartPosition() const;

        /// @brief Gets the position of the last keyframe.
        /// @return The start position, or 0.0 if timeline is empty
        float getEndPosition() const;

        /// @brief Gets all keyframe positions in the timeline.
        /// @return Vector of all position values
        std::vector<float> getPositions() const;

        /// @brief Gets keyframe positions remapped to a new range
        /// @param start The new start position
        /// @param end The new end position
        /// @return Vector of remapped position values
        std::vector<float> getPositionsMapped(float start = 0.0f, float end = 1.0f) const;

        /// @brief Samples the timeline at regular intervals.
        /// @param startPosition The starting position for sampling
        /// @param endPosition The ending position for sampling
        /// @param numSamples The number of samples to take
        /// @return Vector of 16-bit color values
        std::vector<uint16_t> sample(float startPosition, float endPosition, size_t numSamples);
};