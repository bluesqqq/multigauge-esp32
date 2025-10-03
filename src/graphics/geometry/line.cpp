#include "line.h"

#include "rectangle.h"

template <typename T>
Line<T>::Line(T x1, T y1, T x2, T y2) : p1(Point<T>(x1, y1)), p2(Point<T>(x2, y2)) {}

template <typename T>
Line<T>::Line(Point<T> p1, Point<T> p2) : p1(p1), p2(p2) {}

template <typename T>
Line<float> Line<T>::radialLine(float angle, float startRadius, float endRadius) {
    Point<T> unitVector = Point<T>::getPointOnUnitCircle(angle);
    return Line<float>(unitVector * startRadius, unitVector * endRadius);
}

template <typename T>
Line<float> Line<T>::radialLine(float angle, float startRadiusX, float startRadiusY, float endRadiusX, float endRadiusY) {
    return Line<float>(Point<T>::getPointOnCircumference(startRadiusX, startRadiusY, angle), Point<T>::getPointOnCircumference(endRadiusX, endRadiusY, angle));
}

template <typename T>
bool Line<T>::operator==(const Line<T> &other) const { return p1 == other.p1 && p2 == other.p2; }

template <typename T>
bool Line<T>::operator!=(const Line<T> &other) const { return p1 != other.p1 || p2 != other.p2; }

template <typename T>
Line<T> Line<T>::operator-() const { Line<T>(-p1, -p2); }

template <typename T>
Line<T> &Line<T>::operator+=(const Point<T> &translate) const { *this->translate(translate); }

template <typename T>
Line<T> &Line<T>::operator-=(const Point<T> &translate) const { *this->translate(translate); }

template <typename T>
Line<T> &Line<T>::operator*=(const Point<T> &scale) const { p1 *= scale; p2 *= scale; }

template <typename T>
Line<T> &Line<T>::operator/=(const Point<T> &scale) const { p1 /= scale; p2 /= scale; }

template <typename T>
Line<T> Line<T>::operator+(const Point<T> &translate) const { return translated(translate); }

template <typename T>
Line<T> Line<T>::operator-(const Point<T> &translate) const { return translated(-translate); }

template <typename T>
Line<T> Line<T>::operator*(float scalar) const { return Line<T>(p1 * scalar, p2 * scalar); }

template <typename T>
Line<T> Line<T>::operator/(float scalar) const { return Line<T>(p1 / scalar, p2 / scalar); }

template <typename T>
Line<T> Line<T>::reversed() const { return Line<T>(p2, p1); }

template <typename T>
bool Line<T>::isHorizontal() const { return p1.y == p2.y; }

template <typename T>
bool Line<T>::isVertical() const { return p1.x == p2.x; }

template <typename T>
float Line<T>::length() const { return p1.distanceTo(p2); }

template <typename T>
float Line<T>::normal() const { return p1.angleTo(p2) - HALF_PI; }

template <typename T>
float Line<T>::slope() { return (p2.y - p1.y) / (p2.x - p1.x); }

template <typename T>
float Line<T>::intercept() {
    if (p2.x == p1.x) return std::numeric_limits<float>::infinity();
    return p1.y - slope() * p1.x; 
}

template <typename T>
Point<T> Line<T>::midpoint() const { return p1.midpoint(p2); }

template <typename T>
std::optional<Point<T>> Line<T>::intersection(const Line<T> &other) const {
    float otherDx = other.p2.x - other.p1.x;
    float otherDy = other.p2.y - other.p1.y;
    float thisDx  = p2.x - p1.x;
    float thisDy  = p2.y - p1.y;
    float denom = otherDy * thisDx - otherDx * thisDy;

    if (denom == 0.0f) return std::nullopt;
    float uA = (otherDx * (p1.y - other.p1.y) - otherDy * (p1.x - other.p1.x)) / denom;
    float uB = (thisDx * (p1.y - other.p1.y) - thisDy * (p1.x - other.p1.x)) / denom;

    if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) return Point<T>(p1.x + (uA * thisDx), p1.y + (uA * thisDy));
    
    return std::nullopt; // No intersection found
}

template <typename T>
std::optional<Point<T>> Line<T>::slopeIntersection(const Line<T> &other) const {
    float otherDx = other.p2.x - other.p1.x;
    float otherDy = other.p2.y - other.p1.y;
    float thisDx  = p2.x - p1.x;
    float thisDy  = p2.y - p1.y;
    float denom = otherDy * thisDx - otherDx * thisDy;

    if (denom == 0) return std::nullopt;

    float uA = (otherDx * (p1.y - other.p1.y) - otherDy * (p1.x - other.p1.x)) / denom;

    return Point<T>(p1.x + uA * (thisDx), p1.x + uA * thisDy);
}

template <typename T>
std::optional<Line<T>> Line<T>::intersection(const Rectangle<T> &rect) const {
    Point<T> clippedStart = p1;
    Point<T> clippedEnd   = p2;
    bool startInside = rect.contains(p1);
    bool endInside   = rect.contains(p2);

    if (startInside && endInside) return Line<T>(p1, p2);

    Point<T> ip;
    int count = 0;

    auto tryIntersect = [&](const Line<T>& edge) {
        auto i = this->intersection(edge);
        if (i) {
            if (!startInside && count == 0) clippedStart = *i;
            else clippedEnd = *i;
            count++;
        }
    };

    tryIntersect(rect.getTopLine());
    tryIntersect(rect.getBottomLine());
    tryIntersect(rect.getLeftLine());
    tryIntersect(rect.getRightLine());

    if (startInside) clippedStart = p1;
    if (endInside)   clippedEnd   = p2;

    if (startInside || endInside || count >= 2) { return Line<T>(clippedStart, clippedEnd); }

    return std::nullopt;
}

template <typename T>
std::vector<Line<T>> Line<T>::intersection(const std::vector<Rectangle<T>> &rectangles) const {
    std::vector<Line<T>> segments;

    for (const auto& rect : rectangles) {
        auto clipped = this->intersection(rect);
        if (clipped) segments.push_back(*clipped);
    }

    return segments;
}

template <typename T>
void Line<T>::translate(T deltaX, T deltaY) { p1.translate(deltaX, deltaY); p2.translate(deltaX, deltaY); }

template <typename T>
void Line<T>::translate(const Point<T> &delta) { p1.translate(delta); p2.translate(delta); }

template <typename T>
Line<T> Line<T>::translated(T deltaX, T deltaY) { return Line<T>(p1.translated(deltaX, deltaY), p2.translated(deltaX, deltaY)); }

template <typename T>
Line<T> Line<T>::translated(const Point<T> &delta) { return Line<T>(p1.translated(delta), p2.translated(delta)); }

template <typename T>
void Line<T>::interpolate(const Line<T> &other, float t) { p1.interpolate(other.p1, t); p2.interpolate(other.p2, t); }

template <typename T>
Line<T> Line<T>::interpolated(const Line<T> &other, float t) const { return Line<T>(p1.interpolated(other, t), p2.interpolated(other, t)); }

template <typename T>
void Line<T>::scaleFromOrigin(float scale) { p1.scaleFromOrigin(scale); p2.scaleFromOrigin(scale); }

template <typename T>
Point<T> Line<T>::scaledFromOrigin(float scale) { return Point<T>(p1.scaledFromOrigin(scale), p2.scaledFromOrigin(scale)); }

template <typename T>
void Line<T>::scaleFromPoint(const Point<T> &other, float scale) { p1.scaledFromPoint(scale); p2.scaledFromPoint(scale); }

template <typename T>
Point<T> Line<T>::scaledFromPoint(const Point<T> &other, float scale) { return Point<T>(p1.scaledFromPoint(scale), p2.scaledFromPoint(scale)); }

template <typename T>
float Line<T>::angle() const { return p1.angleTo(p2); }

template <typename T>
void Line<T>::rotateAroundOrigin(float angle) { p1.rotateAroundOrigin(angle); p2.rotateAroundOrigin(angle); }

template <typename T>
Point<T> Line<T>::rotatedAroundOrigin(float angle) { return Point<T>(p1.rotatedAroundOrigin(angle), p2.rotatedAroundOrigin(angle)); }

template <typename T>
void Line<T>::rotateAroundPoint(const Point<T> &pivot, float angle) { p1.rotateAroundPoint(pivot, angle); p2.rotateAroundPoint(pivot, angle); }

template <typename T>
Point<T> Line<T>::rotatedAroundPoint(const Point<T> &pivot, float angle) { return Point<T>(p1.rotateAroundPoint(pivot, angle), p2.rotateAroundPoint(pivot, angle)); }

template <typename T>
void Line<T>::reflectAcrossHorizontal(T horizontal) { p1.reflectAcrossHorizontal(horizontal); p2.reflectAcrossHorizontal(horizontal); }

template <typename T>
Point<T> Line<T>::reflectedAcrossHorizontal(T horizontal) { return Point<T>(p1.reflectedAcrossHorizontal(horizontal), p2.reflectedAcrossHorizontal(horizontal)); }

template <typename T>
void Line<T>::reflectAcrossVertical(T vertical) { p1.reflectAcrossVertical(vertical); p2.reflectAcrossVertical(vertical); }

template <typename T>
Point<T> Line<T>::reflectedAcrossVertical(T vertical) { return Point<T>(p1.reflectedAcrossVertical(vertical), p2.reflectedAcrossVertical(vertical)); }

template <typename T>
void Line<T>::reflectAcrossLine(Line<T> line) { p1.reflectAcrossLine(line); p2.reflectAcrossLine(line); }

template <typename T>
Point<T> Line<T>::reflectedAcrossLine(Line<T> line) { return Point<T>(p1.reflectedAcrossLine(line), p2.reflectedAcrossLine(line)); }

template <typename T>
Line<int> Line<T>::toInt() const { return Line<int>(p1.toInt(), p2.toInt()); }

template <typename T>
Line<float> Line<T>::toFloat() const { return Line<float>(p1.toFloat(), p2.toFloat()); }
