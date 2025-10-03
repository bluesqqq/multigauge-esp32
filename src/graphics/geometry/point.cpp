#include "point.h"



template <typename T>
Point<T>::Point() : x(0), y(0) { }

template <typename T>
Point<T>::Point(T x, T y) : x(x), y(y) { }

template <typename T>
Point<float> Point<T>::getPointOnUnitCircle(float angle) { return Point<T>(static_cast<T>(cosf(angle)), static_cast<T>(sinf(angle))); }

template <typename T>
Point<T> Point<T>::getAligned(T x, T y, T width, T height, Alignment alignment) {
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
Point<T> Point<T>::getAligned(const Point<T>& point, T width, T height, Alignment alignment) { return Point<T>::align(point.x, point.y, width, height, alignment); }

template <typename T>
Point<T> Point<T>::getAligned(const Rectangle<T>& rectangle) { return Point<T>::align(rectangle.position.point.x, rectangle.position.point.y, rectangle.width, rectangle.height, rectangle.alignment); }

template <typename T>
bool Point<T>::operator==(const Point<T>& other) const { return x == other.x && y == other.y; }

template <typename T>
bool Point<T>::operator!=(const Point<T>& other) const { return !(*this == other); }

template <typename T>
Point<T> Point<T>::operator-() const { return Point<T>(-x, -y); }

template <typename T>
inline Point<T> &Point<T>::operator=(const Point<T> &other) { x = other.x; y = other.y; }

template <typename T>
Point<T> &Point<T>::operator+=(const Point<T> &other) {
    x += other.x;
    y += other.y;
    return *this;
}

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
Point<T> Point<T>::translated(T deltaX, T deltaY) const { return Point<T>(x + deltaX, y + deltaY); }

template <typename T>
Point<T> Point<T>::translated(const Point<T> &other) const { return *this + other; }

template <typename T>
float Point<T>::magnitude() const { return std::sqrt(x*x + y*y); }

template <typename T>
Point<float> Point<T>::normalized() const {
    float mag = magnitude();
    return Point<T>(x / mag, y / mag);
}

template <typename T>
float Point<T>::distance() const { return std::sqrt(x * x + y * y); }

template <typename T>
float Point<T>::distanceTo(const Point<T> &other) const { return std::sqrt((other.x - x) * (other.x - x) + (other.y - y) * (other.y - y)); }

template <typename T>
float Point<T>::angleTo(const Point<T>& other) const { return atan2f(other.y - y, other.x - x); }

template <typename T>
void Point<T>::rotateAroundOrigin(float angle) {
    float cosA = cosf(angle);
    float sinA = sinf(angle);

    x = x * cosA - y * sinA;
    y = x * sinA + y * cosA;
}

template <typename T>
Point<T> Point<T>::rotatedAroundOrigin(float angle) const {
    float cosA = cosf(angle);
    float sinA = sinf(angle);
    return Point<T>(x * cosA - y * sinA, x * sinA + y * cosA);
}

template <typename T>
void Point<T>::rotateAroundPoint(const Point<T> &other, float angle) {
    float cosA = cosf(angle);
    float sinA = sinf(angle);

    float dx = x - other.x;
    float dy = y - other.y;

    x = dx * cosA - dy * sinA + other.x;
    y = dx * sinA + dy * cosA + other.y;
}

template <typename T>
Point<T> Point<T>::rotatedAroundPoint(const Point<T> &other, float angle) const {
    float cosA = cosf(angle);
    float sinA = sinf(angle);

    float dx = x - other.x;
    float dy = y - other.y;

    return Point<T>(dx * cosA - dy * sinA + other.x, dx * sinA + dy * cosA + other.y);
}

template <typename T>
void Point<T>::reflectAcrossHorizontal(T horizontal) { y = 2 * horizontal - y; }

template <typename T>
Point<T> Point<T>::reflectedAcrossHorizontal(T horizontal) const { return Point<T>(x, 2 * horizontal - y); }

template <typename T>
void Point<T>::reflectAcrossVertical(T vertical) { x = 2 * vertical - x; }

template <typename T>
Point<T> Point<T>::reflectedAcrossVertical(T vertical) const { return Point<T>(x, 2 * vertical - x); }

template <typename T>
void Point<T>::reflectAcrossLine(Line<T> line) {
    float m = line.slope();
    float b = line.intercept();

    float d = (x + (y - b) * m) / (1 + m * m);
    x = 2 * d - x;
    y = 2 * d * m - y + 2 * b;
}

template <typename T>
Point<T> Point<T>::reflectedAcrossLine(Line<T> line) const {
    float m = line.slope();
    float b = line.intercept();

    float d = (x + (y - b) * m) / (1 + m * m);

    return Point<T>(2 * d - x, 2 * d * m - y + 2 * b);
}

template <typename T>
Point<int> Point<T>::toInt() const { return Point<int>(static_cast<int>(x), static_cast<int>(y)); }

template <typename T>
Point<float> Point<T>::toFloat() const { return Point<float>(static_cast<float>(x), static_cast<float>(y)); }

template <typename T>
float Point<T>::angle() const {
    float a = atan2f(y, x); 
    return a >= 0 ? a : a + 2 * PI;
}

template <typename T>
Point<T> Point<T>::midpoint(const Point<T>& other) const { return Point<T>((x + other.x) / 2, (y + other.y) / 2); }

template <typename T>
void Point<T>::interpolate(const Point<T> &other, float t) { x = x + (other.x - x) * t; y = y + (other.y - y) * t; }

template <typename T>
Point<T> Point<T>::interpolated(const Point<T> &other, float t) const { return Point<T>(x + (other.x - x) * t, y + (other.y - y) * t); }

template <typename T>
void Point<T>::scaleFromOrigin(float scale) { *this *= scale; }

template <typename T>
Point<T> Point<T>::scaledFromOrigin(float scale) const { return *this * scale; }

template <typename T>
void Point<T>::scaleFromPoint(const Point<T> &other, float scale) {
    x = (x - other.x) * scale + other.x;
    y = (y - other.y) * scale + other.y;
}

template <typename T>
Point<T> Point<T>::scaledFromPoint(const Point<T> &other, float scale) const { return Point<T>((x - other.x) * scale + other.x, (y - other.y) * scale + other.y); }

template<typename T> float Point<T>::dot(const Point<T> &other) const { return x * other.x + y * other.y; }

template <typename T>
float Point<T>::cross(const Point<T>& other) const { return x * other.y - y * other.x; }