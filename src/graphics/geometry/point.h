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
    static Point<float> unitVector(float angle);

    static Point<T> align(T x, T y, T width, T height, Alignment alignment);
    static Point<T> align(const Point<T>& point, T width, T height, Alignment alignment);
    static Point<T> align(const Rectangle<T>& rectangle);

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

    void translate(T deltaX, T deltaY);
    void translate(const Point<T>& other);

    float magnitude() const;

    Point<T> normalize() const;

    float distance(const Point<T>& other) const;

    float angleTo(const Point<T>& other) const;

    float angle() const;

    Point<T> midpoint(const Point<T>& other) const;

    float dot(const Point<T>& other) const;

    float cross(const Point<T>& other) const;

    void print();
};