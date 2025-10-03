#pragma once

#include <optional>

#include "point.h"

template <typename T>
struct Line {
    Point<T> p1, p2;
    
    Line(T x1, T y1, T x2, T y2);
    Line(Point<T> p1, Point<T> p2);

    static Line<T> radialLine(float angle, float startRadius, float endRadius);
    
    static Line<T> radialLine(float angle, float startRadius, float length, EdgeAlignment alignment);

    /// @brief Calculates the intersection point of this line with another line.
    /// @param other The other line to intersect with.
    /// @return A pointer to a `Point` representing the intersection point, or `nullptr` if the lines are parallel or coincident.
    std::optional<Point<T>> intersection(const Line<T>& other) const;

    std::optional<Point<T>> slopeIntersection(const Line<T>& other) const;



    bool operator==(const Line<T>& other) const;
    bool operator!=(const Line<T>& other) const;
    Line<T> operator+(const Line<T>& other) const;
    Line<T> operator-(const Line<T>& other) const;

    Line<T> operator*(float scalar) const;
    Line<T> operator/(float scalar) const;

    Line<T> reversed() const;

    bool isHorizontal() const;

    bool isVertical() const;

    float angle() const;

    Point<T> midpoint() const;

    float length() const;
};