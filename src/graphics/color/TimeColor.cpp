#include "TimeColor.h"

float TimeColor::getTime() const {
    float duration = timeline.getEndPosition();

    if (timeline.size() == 0 || duration <= 0.0f) return 0.0f;

    float t = static_cast<float>(millis());

    switch (loopType) {
    case LoopType::Forward:
        t = std::fmod(t, duration);
        break;
    
    case LoopType::Reverse:
        t = duration - std::fmod(t, duration);
        break;

    case LoopType::PingPong:
        t = std::fmod(t, 2.0f * duration);
        if (t > duration) t = 2.0f * duration - t;
        break;
    }

    return t;
}

const ColorTimeline *TimeColor::getTimeline() const { return &timeline; }

TimeColor::TimeColor() : timeline(ColorTimeline()), loopType(LoopType::Forward) { }

TimeColor::TimeColor(const ColorTimeline &timeline, LoopType loopType) : timeline(timeline), loopType(loopType) {}

TimeColor::TimeColor(JsonObject timeColorJson) {
    timeline = timeColorJson["timeline"].is<JsonObject>() ? ColorTimeline(timeColorJson["timeline"].as<JsonObject>()) : ColorTimeline();
    if (timeColorJson["loopType"].is<const char*>()) {
        const char* loopStr = timeColorJson["loopType"].as<const char*>();
        if (strcmp(loopStr, "forward") == 0) loopType = LoopType::Forward;
        else if (strcmp(loopStr, "reverse") == 0) loopType = LoopType::Reverse;
        else if (strcmp(loopStr, "pingpong") == 0) loopType = LoopType::PingPong;
        else loopType = LoopType::Forward;
    } else {
        loopType = LoopType::Forward;
    }
}

std::unique_ptr<Color> TimeColor::blended(uint16_t color, float alpha) const { return std::make_unique<TimeColor>(timeline.blended(color, alpha)); }

std::unique_ptr<Color> TimeColor::blended(const Color &other, float alpha) const {
    switch(other.getType()) {
        case (Type::Static):
            return blended(other.getColor(), alpha);

        case (Type::Time):
        case (Type::Value): {
            const ColorTimeline* timeline = other.getTimeline();
            if (timeline != nullptr) return std::make_unique<TimeColor>(this->timeline.blended(*timeline, alpha), this->loopType);
        }
    }

    return std::make_unique<TimeColor>(this->timeline, this->loopType);
}

uint16_t TimeColor::getColor() const { return timeline.getColor(getTime()); }

Color::Type TimeColor::getType() const { return Type::Time; }
