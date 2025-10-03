#pragma once

#include <cmath>
#include "Arduino.h"
#include "alignment.h"

template <typename T>
struct Rectangle;

template <typename T>
struct Point {
    T x, y;

    Point();
    
    Point(T x, T y);

    /// @brief Returns a unit vector pointing in the specified angle.
    /// @param angle Angle in radians.
    /// @return A Point representing a unit vector with the given direction
    static Point<float> getPointOnUnitCircle(float angle);

    static Point<float> getPointOnCircumference(float radius, float angle);

    static Point<float> getPointOnCircumference(float radiusX, float radiusY, float angle);

    static Point<T> getAligned(T x, T y, T width, T height, Alignment alignment);
    static Point<T> getAligned(const Point<T>& point, T width, T height, Alignment alignment);
    static Point<T> getAligned(const Rectangle<T>& rectangle);

    inline bool operator==(const Point<T>& other) const;
    inline bool operator!=(const Point<T>& other) const;

    inline Point<T> operator-() const;

    inline Point<T>& operator+=(const Point<T>& other);
    inline Point<T>& operator-=(const Point<T>& other);
    inline Point<T>& operator*=(const Point<T>& other);
    inline Point<T>& operator/=(const Point<T>& other);

    inline Point<T>& operator*=(float scalar);
    inline Point<T>& operator/=(float scalar);

    inline Point<T> operator-(const Point<T>& other) const;
    inline Point<T> operator+(const Point<T>& other) const;
    inline Point<T> operator*(const Point<T>& other) const;
    inline Point<T> operator/(const Point<T>& other) const;

    inline Point<T> operator*(float scalar) const;
    inline Point<T> operator/(float scalar) const;

    inline Point<T> operator^(float exponent) const;

    float magnitude() const;

    Point<float> normalized() const;

    float distance() const;
    float distanceTo(const Point<T>& other) const;
    float dot(const Point<T>& other) const;
    float cross(const Point<T>& other) const;

    // ====== [TRANSLATION] ====== //

    void translate(T deltaX, T deltaY);
    void translate(const Point<T>& other);
    Point<T> translated(T deltaX, T deltaY);
    Point<T> translated(const Point<T>& other);

    Point<T> midpoint(const Point<T>& other) const;

    void interpolate(const Point<T>& other, float t);
    Point<T> interpolated(const Point<T>& other, float t) const;

    void scaleFromOrigin(float scale);
    Point<T> scaledFromOrigin(float scale);

    void scaledFromPoint(const Point<T>& other, float scale);
    Point<T> scaledFromPoint(const Point<T>& other, float scale);

    // ====== [ROTATION] ====== //

    float angle() const;
    float angleTo(const Point<T>& other) const;

    void rotateAroundOrigin(float angle);
    Point<T> rotatedAroundOrigin(float angle);

    void rotateAroundPoint(const Point<T>& other, float angle);
    Point<T> rotatedAroundPoint(const Point<T>& other, float angle);

    // ====== [CONVERSION] ====== //

    Point<int> toInt();
    Point<float> toFloat();
};