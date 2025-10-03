#include "point.h"



template <typename T>
Point<T>::Point() : x(0), y(0) { }

template <typename T>
Point<T>::Point(T x, T y) : x(x), y(y) { }

template <typename T>
Point<float> Point<T>::unitVector(float angle) { return Point<T>(static_cast<T>(cosf(angle)), static_cast<T>(sinf(angle))); }

template <typename T>
Point<T> Point<T>::align(T x, T y, T width, T height, Alignment alignment) {
    switch (alignment) {
        case Alignment::TopLeft:
            return Point(x, y);
        case Alignment::TopCenter:
            return Point(x - width / 2, y);
        case Alignment::TopRight:
            return Point(x - width, y);
        case Alignment::CenterLeft:
            return Point(x, y - height / 2);
        case Alignment::Center:
            return Point(x - width / 2, y - height / 2);
        case Alignment::CenterRight:
            return Point(x - width, y - height / 2);
        case Alignment::BottomLeft:
            return Point(x, y - height);
        case Alignment::BottomCenter:
            return Point(x - width / 2, y - height);
        case Alignment::BottomRight:
            return Point(x - width, y - height);
        default:
            return Point(x, y); // Default to TOP_LEFT if alignment is unknown
    }
}

template <typename T>
Point<T> Point<T>::align(const Point<T>& point, T width, T height, Alignment alignment) { return Point<T>::align(point.x, point.y, width, height, alignment); }

template <typename T>
Point<T> Point<T>::align(const Rectangle<T>& rectangle) { return Point<T>::align(rectangle.position.point.x, rectangle.position.point.y, rectangle.width, rectangle.height, rectangle.alignment); }

template <typename T>
bool Point<T>::operator==(const Point<T>& other) const { return x == other.x && y == other.y; }

template <typename T>
bool Point<T>::operator!=(const Point<T>& other) const { return !(*this == other); }

template <typename T>
Point<T> Point<T>::operator-() const { return Point<T>(-x, -y); }

template <typename T>
Point<T>& Point<T>::operator+=(const Point<T>& other) { x += other.x; y += other.y; return *this; }

template <typename T>
Point<T>& Point<T>::operator-=(const Point<T>& other) { x -= other.x; y -= other.y; return *this; }

template <typename T>
Point<T>& Point<T>::operator*=(const Point<T>& other) { x *= other.x; y *= other.y; return *this; }

template <typename T>
Point<T>& Point<T>::operator/=(const Point<T>& other) { x /= other.x; y /= other.y; return *this; }

template <typename T>
Point<T>& Point<T>::operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }

template <typename T>
Point<T>& Point<T>::operator/=(float scalar) { x /= scalar; y /= scalar; return *this; }

template <typename T>
Point<T> Point<T>::operator-(const Point<T>& other) const { return Point<T>(x - other.x, y - other.y); }

template <typename T>
Point<T> Point<T>::operator+(const Point<T>& other) const { return Point<T>(x + other.x, y + other.y); }

template <typename T>
Point<T> Point<T>::operator*(const Point<T>& other) const { return Point<T>(x * other.x, y * other.y); }

template <typename T>
Point<T> Point<T>::operator/(const Point<T>& other) const { return Point<T>(x / other.x, y / other.y); }

template <typename T>
Point<T> Point<T>::operator*(float scalar) const { return Point<T>(x * scalar, y * scalar); }

template <typename T>
Point<T> Point<T>::operator/(float scalar) const { return Point<T>(x / scalar, y / scalar); }

template <typename T>
Point<T> Point<T>::operator^(float exponent) const { return Point<T>(powf(x, exponent), powf(y, exponent)); }

template <typename T>
void Point<T>::translate(T deltaX, T deltaY) { x += deltaX; y += deltaY; }

template <typename T>
void Point<T>::translate(const Point<T> &other) { *this += other; }

template <typename T>
float Point<T>::magnitude() const { return std::sqrt(x*x + y*y); }

template <typename T>
Point<T> Point<T>::normalize() const {
    float mag = magnitude();
    return Point<T>(x / mag, y / mag);
}

template <typename T>
float Point<T>::distance(const Point<T>& other) const {
return std::sqrt((other.x - x)*(other.x - x) + (other.y - y)*(other.y - y));
}

template <typename T>
float Point<T>::angleTo(const Point<T>& other) const { return atan2f(other.y - y, other.x - x); }

template <typename T>
float Point<T>::angle() const { 
    float a = atan2f(y, x); 
    return a >= 0 ? a : a + 2 * PI; 
}

template <typename T>
Point<T> Point<T>::midpoint(const Point<T>& other) const { return Point<T>((x + other.x)/2, (y + other.y)/2); }

template <typename T>
float Point<T>::dot(const Point<T>& other) const { return x * other.x + y * other.y; }

template <typename T>
float Point<T>::cross(const Point<T>& other) const { return x * other.y - y * other.x; }

template <typename T>
void Point<T>::print() { Serial.printf("point: (%f, %f)\n", static_cast<float>(x), static_cast<float>(y)); }