#include "Length.h"

Length::Length(float value, LengthType type) : value(value), type(type) {}
Length Length::px(float value) { return {value, LengthType::Pixels}; }
Length Length::percent(float value) { return {value, LengthType::Percent}; }
Length Length::auto_() { return {0.0f, LengthType::Auto}; }

float Length::resolve(float px) const {
    switch(type) {
        case LengthType::Auto:
            return -1.0f;
        case LengthType::Percent:
            return (px * (value / 100.0f));
        case LengthType::Pixels:
            return value;
    }
    
    return 0;
}

bool Length::isAuto() const { return type == LengthType::Auto; }
