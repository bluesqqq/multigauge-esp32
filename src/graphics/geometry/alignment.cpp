#include "alignment.h"

std::pair<float, float> alignLength(float start, float length, LengthAlignment alignment) {
    switch (alignment) {
        case LENGTH_OUTER:
            return std::pair<float, float>(start, start - length);

        case LENGTH_CENTER: {
            const float halfLength = length / 2.0;
            return std::pair<float, float>(start - halfLength, start + halfLength);
        }

        case LENGTH_INNER:
            return std::pair<float, float>(start, start + length);
        
        default:
            return std::pair<float, float>(start, start);
    }
}