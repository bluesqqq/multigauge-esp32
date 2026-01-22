#pragma once

#include "Length.h"

template <typename T>
struct Rectangle;

struct BoxSpacingPx {
    float top, right, bottom, left;

    float horizontal() const { return left + right; }
    float vertical() const { return top + bottom; }
};

struct BoxSpacing {
    Length top;
    Length right;
    Length bottom;
    Length left;

    BoxSpacing() = default;
    BoxSpacing(Length all);
    BoxSpacing(Length vertical, Length horizontal);
    BoxSpacing(Length t, Length r, Length b, Length l);
    
    BoxSpacingPx resolve(float widthPx, float heightPx);

    BoxSpacingPx resolve(const Rectangle<float>& rectangle);
};