#pragma once

enum class Direction {
    Right,
    Down,
    Left,
    Up
};

enum class Alignment {
    TopLeft,
    TopCenter,
    TopRight,
    CenterLeft,
    Center,
    CenterRight,
    BottomLeft,
    BottomCenter,
    BottomRight
};

enum class EdgeAlignment {
    Outer = -1,
    Center = 0,
    Inner = 1
};

enum class Orientation {
    Horizontal,
    Vertical
};