#include "ColorTimeline.h"
#include "StaticColor.h"
#include "utils.h"
#include <set>
#include <algorithm>

ColorKeyframe::ColorKeyframe(std::unique_ptr<Color> color, float position) : color(std::move(color)), position(position) { }

ColorKeyframe::ColorKeyframe(JsonObject colorKeyframeJson) {
    position = colorKeyframeJson["position"].is<float>() ? colorKeyframeJson["position"].as<float>() : 0.0f;
    color = colorKeyframeJson["color"].is<JsonObject>() ? Color::loadFromJson(colorKeyframeJson["color"].as<JsonObject>()) : std::make_unique<StaticColor>();
}

ColorKeyframe::ColorKeyframe(const ColorKeyframe &other) : position(other.position), color(other.color ? other.color->clone() : nullptr) {}

ColorKeyframe &ColorKeyframe::operator=(const ColorKeyframe &other) {
    if (this != &other) {
        this->position = other.position;
        this->color = other.color ? other.color->clone() : nullptr;
    }
    return *this;
}

size_t ColorTimeline::getKeyframeIndexAtPosition(float position) const {
    const size_t n = timeline.size();
    if (n <= 1) return 0;
    if (position < timeline[1].position) return 0;
    if (position >= timeline.back().position) return n - 1;

    size_t left = 1;
    size_t right = n - 1;
    while (left < right) {
        size_t middle = (left + right) / 2;
        if (timeline[middle].position < position) 
            left = middle + 1;
        else right = middle;
    }

    return left - 1;
}

ColorTimeline::ColorTimeline() {}

ColorTimeline::ColorTimeline(uint16_t color) { addKeyframe(color, 0.0f); }

ColorTimeline::ColorTimeline(JsonObject colorTimelineJson) {
    if (!colorTimelineJson["keyframes"].is<JsonArray>()) return;

    JsonArray keyframesJson = colorTimelineJson["keyframes"].as<JsonArray>();
    for (JsonObject keyframeJson : keyframesJson)
        timeline.emplace_back(ColorKeyframe(keyframeJson));
}

ColorTimeline::ColorTimeline(const ColorTimeline &other) {
    const std::vector<ColorKeyframe>& timeline = other.getTimeline();

    for (const auto& keyframe : timeline)
        this->timeline.emplace_back(keyframe);
}

ColorTimeline &ColorTimeline::operator=(const ColorTimeline &other) {
    timeline.clear();

    const std::vector<ColorKeyframe>& timeline = other.getTimeline();

    for (const auto& keyframe : timeline)
        this->timeline.emplace_back(keyframe);

    return *this;
}

ColorTimeline ColorTimeline::blended(uint16_t color, float alpha) const {
    ColorTimeline newTimeline;

    // TODO: addKeyframe is inefficient in this case. emplace_back directly into the timeline would be better
    for (const auto& keyframe : timeline)
        newTimeline.addKeyframe(ColorKeyframe(keyframe.color->blended(color, alpha), keyframe.position));

    return newTimeline;
}

ColorTimeline ColorTimeline::blended(const ColorTimeline &other, float alpha) const {
    ColorTimeline newTimeline;

    std::set<float> positions;
    for (const auto& keyframe : this->timeline) positions.insert(keyframe.position);
    for (const auto& keyframe : other.timeline) positions.insert(keyframe.position);

    // TODO: this only creates static colors, figure out a way to make it work for all types
    for (float position : positions) {
        uint16_t colorA = this->getColor(position);
        uint16_t colorB = other.getColor(position);
        uint16_t blended = Color::blend(colorA, colorB, alpha);

        newTimeline.addKeyframe(blended, position);
    }

    return newTimeline;
}

const std::vector<ColorKeyframe> &ColorTimeline::getTimeline() const { return timeline; }

void ColorTimeline::addKeyframe(uint16_t color, float position) { addKeyframe(ColorKeyframe(std::make_unique<StaticColor>(color), position)); }

void ColorTimeline::addKeyframe(std::unique_ptr<Color> color, float position) { addKeyframe(ColorKeyframe(std::move(color), position)); }

void ColorTimeline::addKeyframe(ColorKeyframe keyframe) {
    auto it = timeline.begin();
    while(it != timeline.end() && it->position < keyframe.position) ++it;
    timeline.insert(it, std::move(keyframe));
}

void ColorTimeline::clear() { timeline.clear(); }

size_t ColorTimeline::size() const { return timeline.size(); }

uint16_t ColorTimeline::getColor(float position) const {
    if (timeline.empty()) return DEFAULT_COLOR;

    size_t index = getKeyframeIndexAtPosition(position);

    if (index >= timeline.size() - 1) return timeline.back().color->getColor();

    float normalized = (position - timeline[index].position) / (timeline[index + 1].position - timeline[index].position);

    return Color::blend(timeline[index].color->getColor(), timeline[index + 1].color->getColor(), normalized);
}

float ColorTimeline::getStartPosition() const { return !timeline.empty() ? timeline.front().position : 0.0f; }

float ColorTimeline::getEndPosition() const { return !timeline.empty() ? timeline.back().position : 0.0f; }

std::vector<float> ColorTimeline::getPositions() const {
    std::vector<float> positions;

    for (const auto& keyframe : timeline) positions.push_back(keyframe.position);

    return positions;
}

std::vector<float> ColorTimeline::getPositionsMapped(float start, float end) const {
    std::vector<float> positions;

    float startPosition = getStartPosition();
    float endPosition = getEndPosition();

    for (const auto& keyframe : timeline) positions.push_back(mapf(keyframe.position, startPosition, endPosition, start, end));

    return positions;
}

std::vector<uint16_t> ColorTimeline::sample(float startPosition, float endPosition, size_t numSamples) {
    std::vector<uint16_t> result;
    if (timeline.empty() || numSamples == 0) return result;
    result.reserve(numSamples);

    float step = (endPosition - startPosition) / (numSamples - 1);
    size_t startIndex = 0;
    size_t endIndex = (timeline.size() > 1) ? 1 : 0;

    uint16_t startColor = timeline[startIndex].color->getColor();
    uint16_t endColor   = timeline[endIndex].color->getColor();

    for (size_t sample = 0; sample < numSamples; ++sample) {
        float position = startPosition + step * sample;

        while (endIndex < timeline.size() && position > timeline[endIndex].position) {
            startIndex = endIndex;
            startColor = endColor;
            endIndex++;
            endColor = timeline[std::min(endIndex, timeline.size() - 1)].color->getColor();
        }

        endIndex = std::min(endIndex, timeline.size() - 1);

        float t = 0;
        if (startIndex != endIndex) {
            float delta = timeline[endIndex].position - timeline[startIndex].position;
            t = (delta != 0) ? (position - timeline[startIndex].position) / delta : 0;
        }

        result.push_back(Color::blend(startColor, endColor, t));
    }

    return result;
}