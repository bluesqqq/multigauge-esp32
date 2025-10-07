#pragma once

#include <optional>

#include "point.h"

template <typename T>
class Rectangle;

template <typename T>
struct Line {
    Point<T> p1, p2;
    
    Line(T x1, T y1, T x2, T y2);
    Line(const Point<T>& p1, const Point<T>& p2);

    static Line<float> radialLine(float angle, float startRadius, float endRadius);
    static Line<float> radialLine(float angle, float startRadiusX, float startRadiusY, float endRadiusX, float endRadiusY);

    bool operator==(const Line<T>& other) const;
    bool operator!=(const Line<T>& other) const;

    Line<T> operator-() const;

    Line<T>& operator+=(const Point<T>& translate) const;
    Line<T>& operator-=(const Point<T>& translate) const;
    Line<T>& operator*=(const Point<T>& scale) const;
    Line<T>& operator/=(const Point<T>& scale) const;

    Line<T> operator+(const Point<T>& translate) const;
    Line<T> operator-(const Point<T>& translate) const;
    Line<T> operator*(float scalar) const;
    Line<T> operator/(float scalar) const;

    Line<T> reversed() const;

    bool isHorizontal() const;
    bool isVertical() const;

    float length() const;
    float normal() const;

    float slope();
    float intercept();
    
    Point<T> midpoint() const;

    // ====== [INTERSECTIONS] ====== //

    bool intersects(const Line<T>& other);
    bool slopesIntersect(const Line<T>& other);
    std::optional<Point<T>> intersection(const Line<T>& other) const;
    std::optional<Point<T>> slopeIntersection(const Line<T>& other) const;

    std::optional<Line<T>> intersection(const Rectangle<T>& rectangle) const;
    std::vector<Line<T>>   intersection(const std::vector<Rectangle<T>>& rectangles) const;

    // ====== [TRANSLATION] ====== //

    void translate(T deltaX, T deltaY);
    void translate(const Point<T>& delta);
    Line<T> translated(T deltaX, T deltaY);
    Line<T> translated(const Point<T>& delta);

    void interpolate(const Line<T>& other, float t);
    Line<T> interpolated(const Line<T>& other, float t) const;

    // ====== [SCALE] ====== //

    void scaleFromOrigin(float scale);
    Point<T> scaledFromOrigin(float scale);

    void scaleFromPoint(const Point<T>& other, float scale);
    Point<T> scaledFromPoint(const Point<T>& other, float scale);

    // ====== [ROTATION] ====== //

    float angle() const;

    void rotateAroundOrigin(float angle);
    Point<T> rotatedAroundOrigin(float angle);

    void rotateAroundPoint(const Point<T>& pivot, float angle);
    Point<T> rotatedAroundPoint(const Point<T>& pivot, float angle);

    // ====== [REFLECTION] ====== //

    void reflectAcrossHorizontal(T horizontal);
    Point<T> reflectedAcrossHorizontal(T horizontal);

    void reflectAcrossVertical(T vertical);
    Point<T> reflectedAcrossVertical(T vertical);

    void reflectAcrossLine(Line<T> line);
    Point<T> reflectedAcrossLine(Line<T> line);

    // ====== [CONVERSION] ====== //

    Line<int> toInt() const;
    Line<float> toFloat() const;
};