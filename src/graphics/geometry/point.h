#pragma once

#include <cmath>
#include "Arduino.h"
#include "alignment.h"

template <typename T>
struct Rectangle;

template <typename T>
struct Line;

template <typename T>
struct Point {
    T x, y;

    Point() : x(0), y(0) {}
    
    Point(T x, T y) : x(x), y(y) {}

    /// @brief Returns a unit vector pointing in the specified angle.
    /// @param angle Angle in radians.
    /// @return A Point representing a unit vector with the given direction
    static Point<float> getPointOnUnitCircle(float angle) {
        return Point<float>(cosf(angle), sinf(angle));
    }

    static Point<float> getPointOnCircumference(float radius, float angle) {
        return Point<float>(radius * cosf(angle), radius * sinf(angle));
    }

    static Point<float> getPointOnCircumference(float radiusX, float radiusY, float angle) {
        return Point<float>(radiusX * cosf(angle), radiusY * sinf(angle));
    }

    static Point<T> getAnchored(T x, T y, T width, T height, Anchor anchor) {
        switch (anchor) {
            case Anchor::TopLeft: return Point(x, y);
            case Anchor::TopCenter: return Point(x - width / 2, y);
            case Anchor::TopRight: return Point(x - width, y);
            case Anchor::CenterLeft: return Point(x, y - height / 2);
            case Anchor::Center: return Point(x - width / 2, y - height / 2);
            case Anchor::CenterRight: return Point(x - width, y - height / 2);
            case Anchor::BottomLeft: return Point(x, y - height);
            case Anchor::BottomCenter: return Point(x - width / 2, y - height);
            case Anchor::BottomRight: return Point(x - width, y - height);
            default: return Point(x, y);
        }
    }

    static Point<T> getAnchored(const Point<T>& point, T width, T height, Anchor anchor) {
        return Point<T>::getAnchored(point.x, point.y, width, height, anchor);
    }

    static Point<T> getAnchored(const Rectangle<T>& rectangle) {
        return Point<T>::getAnchored(rectangle.position.x, rectangle.position.y, rectangle.width, rectangle.height, rectangle.anchor);
    }

    inline bool operator==(const Point<T>& other) const { return x == other.x && y == other.y; }
    inline bool operator!=(const Point<T>& other) const { return !(*this == other); }

    inline Point<T> operator-() const { return Point<T>(-x, -y); }

    inline Point<T>& operator=(const Point<T>& other) { x = other.x; y = other.y; return *this; }
    inline Point<T>& operator+=(const Point<T>& other) { x += other.x; y += other.y; return *this; }
    inline Point<T>& operator-=(const Point<T>& other) { x -= other.x; y -= other.y; return *this; }
    inline Point<T>& operator*=(const Point<T>& other) { x *= other.x; y *= other.y; return *this; }
    inline Point<T>& operator/=(const Point<T>& other) { x /= other.x; y /= other.y; return *this; }

    inline Point<T>& operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }
    inline Point<T>& operator/=(float scalar) { x /= scalar; y /= scalar; return *this; }

    inline Point<T> operator-(const Point<T>& other) const { return Point<T>(x - other.x, y - other.y); }
    inline Point<T> operator+(const Point<T>& other) const { return Point<T>(x + other.x, y + other.y); }
    inline Point<T> operator*(const Point<T>& other) const { return Point<T>(x * other.x, y * other.y); }
    inline Point<T> operator/(const Point<T>& other) const { return Point<T>(x / other.x, y / other.y); }

    inline Point<T> operator*(float scalar) const { return Point<T>(x * scalar, y * scalar); }
    inline Point<T> operator/(float scalar) const { return Point<T>(x / scalar, y / scalar); }

    inline Point<T> operator^(float exponent) const { return Point<T>(powf(x, exponent), powf(y, exponent)); }

    float magnitude() const { return std::sqrt(x * x + y * y); }

    Point<float> normalized() const {
        float mag = magnitude();
        return Point<float>(x / mag, y / mag);
    }

    float distance() const { return std::sqrt(x * x + y * y); }
    float distanceTo(const Point<T>& other) const { return std::sqrt((other.x - x) * (other.x - x) + (other.y - y) * (other.y - y)); }
    float dot(const Point<T>& other) const { return x * other.x + y * other.y; }
    float cross(const Point<T>& other) const { return x * other.y - y * other.x; }

    // ====== [TRANSLATION] ====== //
    void translate(T deltaX, T deltaY) { x += deltaX; y += deltaY; }
    void translate(const Point<T>& other) { *this += other; }
    Point<T> translated(T deltaX, T deltaY) const { return Point<T>(x + deltaX, y + deltaY); }
    Point<T> translated(const Point<T>& other) const { return *this + other; }

    Point<T> midpoint(const Point<T>& other) const { return Point<T>((x + other.x) / 2, (y + other.y) / 2); }

    void interpolate(const Point<T>& other, float t) { x = x + (other.x - x) * t; y = y + (other.y - y) * t; }
    Point<T> interpolated(const Point<T>& other, float t) const { return Point<T>(x + (other.x - x) * t, y + (other.y - y) * t); }

    // ====== [SCALE] ====== //
    void scaleFromOrigin(float scale) { *this *= scale; }
    Point<T> scaledFromOrigin(float scale) const { return *this * scale; }

    void scaleFromPoint(const Point<T>& other, float scale) {
        x = (x - other.x) * scale + other.x;
        y = (y - other.y) * scale + other.y;
    }
    Point<T> scaledFromPoint(const Point<T>& other, float scale) const {
        return Point<T>((x - other.x) * scale + other.x, (y - other.y) * scale + other.y);
    }

    // ====== [ROTATION] ====== //
    float angle() const {
        float a = atan2f(y, x);
        return a >= 0 ? a : a + 2 * PI;
    }
    float angleTo(const Point<T>& other) const { return atan2f(other.y - y, other.x - x); }

    void rotateAroundOrigin(float angle) {
        float cosA = cosf(angle);
        float sinA = sinf(angle);
        T nx = x * cosA - y * sinA;
        T ny = x * sinA + y * cosA;
        x = nx; y = ny;
    }

    Point<T> rotatedAroundOrigin(float angle) const {
        float cosA = cosf(angle);
        float sinA = sinf(angle);
        return Point<T>(x * cosA - y * sinA, x * sinA + y * cosA);
    }

    void rotateAroundPoint(const Point<T>& other, float angle) {
        float cosA = cosf(angle);
        float sinA = sinf(angle);
        float dx = x - other.x;
        float dy = y - other.y;
        x = dx * cosA - dy * sinA + other.x;
        y = dx * sinA + dy * cosA + other.y;
    }

    Point<T> rotatedAroundPoint(const Point<T>& other, float angle) const {
        float cosA = cosf(angle);
        float sinA = sinf(angle);
        float dx = x - other.x;
        float dy = y - other.y;
        return Point<T>(dx * cosA - dy * sinA + other.x, dx * sinA + dy * cosA + other.y);
    }

    // ====== [REFLECTION] ====== //
    void reflectAcrossHorizontal(T horizontal) { y = 2 * horizontal - y; }
    Point<T> reflectedAcrossHorizontal(T horizontal) const { return Point<T>(x, 2 * horizontal - y); }

    void reflectAcrossVertical(T vertical) { x = 2 * vertical - x; }
    Point<T> reflectedAcrossVertical(T vertical) const { return Point<T>(2 * vertical - x, y); }

    void reflectAcrossLine(Line<T> line) {
        float m = line.slope();
        float b = line.intercept();
        float d = (x + (y - b) * m) / (1 + m * m);
        x = 2 * d - x;
        y = 2 * d * m - y + 2 * b;
    }

    Point<T> reflectedAcrossLine(Line<T> line) const {
        float m = line.slope();
        float b = line.intercept();
        float d = (x + (y - b) * m) / (1 + m * m);
        return Point<T>(2 * d - x, 2 * d * m - y + 2 * b);
    }

    // ====== [CONVERSION] ====== //
    Point<int> toInt() const { return Point<int>(static_cast<int>(x), static_cast<int>(y)); }
    Point<float> toFloat() const { return Point<float>(static_cast<float>(x), static_cast<float>(y)); }
};
