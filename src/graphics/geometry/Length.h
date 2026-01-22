#pragma once

enum class LengthType {
    Auto, // Length accomodates to content (ignores value)
    Pixels, // Absolute pixel length
    Percent // Percentage of parent length (0% to 100%)
};

struct Length {
    float value;
    LengthType type;

    Length(float value = 0.0f, LengthType type = LengthType::Pixels);

    static Length px(float value = 1.0f);
    static Length percent(float value = 100.0f);
    static Length auto_();

    float resolve(float px) const;

    bool isAuto() const;
};