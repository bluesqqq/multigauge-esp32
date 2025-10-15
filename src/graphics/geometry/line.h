#pragma once

#include <optional>
#include <vector>
#include <limits>
#include <cmath>
#include "point.h"

template <typename T>
class Rectangle;

template <typename T>
struct Line {
    Point<T> p1, p2;

    Line(T x1, T y1, T x2, T y2) : p1(x1, y1), p2(x2, y2) {}
    Line(const Point<T>& p1, const Point<T>& p2) : p1(p1), p2(p2) {}

    static Line<float> radialLine(float angle, float startRadius, float endRadius) {
        Point<float> unitVector = Point<float>::getPointOnUnitCircle(angle);
        return Line<float>(unitVector * startRadius, unitVector * endRadius);
    }

    static Line<float> radialLine(float angle, float startRadiusX, float startRadiusY, float endRadiusX, float endRadiusY) {
        return Line<float>(Point<float>::getPointOnCircumference(startRadiusX, startRadiusY, angle),
                           Point<float>::getPointOnCircumference(endRadiusX, endRadiusY, angle));
    }

    bool operator==(const Line<T>& other) const { return p1 == other.p1 && p2 == other.p2; }
    bool operator!=(const Line<T>& other) const { return !(*this == other); }
    Line<T> operator-() const { return Line<T>(-p1, -p2); }

    Line<T> reversed() const { return Line<T>(p2, p1); }

    bool isHorizontal() const { return p1.y == p2.y; }
    bool isVertical() const { return p1.x == p2.x; }

    float length() const { return p1.distanceTo(p2); }
    float normal() const { return p1.angleTo(p2) - 3.14159265f/2; }

    float slope() const { return (p2.x == p1.x) ? std::numeric_limits<float>::infinity() : (p2.y - p1.y) / (p2.x - p1.x); }
    float intercept() const { return (p2.x == p1.x) ? std::numeric_limits<float>::infinity() : p1.y - slope() * p1.x; }

    Point<T> midpoint() const { return p1.midpoint(p2); }

    float angle() const { return p1.angleTo(p2); }

    // ====== [INTERSECTIONS] ====== //

    bool intersects(const Line<T>& other) const {
        float denom = (other.p2.y - other.p1.y)*(p2.x - p1.x) - (other.p2.x - other.p1.x)*(p2.y - p1.y);
        if (denom == 0.0f) return false;
        float uA = ((other.p2.x - other.p1.x)*(p1.y - other.p1.y) - (other.p2.y - other.p1.y)*(p1.x - other.p1.x)) / denom;
        float uB = ((p2.x - p1.x)*(p1.y - other.p1.y) - (p2.y - p1.y)*(p1.x - other.p1.x)) / denom;
        return (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1);
    }

    std::optional<Point<T>> intersection(const Line<T>& other) const {
        float denom = (other.p2.y - other.p1.y)*(p2.x - p1.x) - (other.p2.x - other.p1.x)*(p2.y - p1.y);
        if (denom == 0.0f) return std::nullopt;
        float uA = ((other.p2.x - other.p1.x)*(p1.y - other.p1.y) - (other.p2.y - other.p1.y)*(p1.x - other.p1.x)) / denom;
        float uB = ((p2.x - p1.x)*(p1.y - other.p1.y) - (p2.y - p1.y)*(p1.x - other.p1.x)) / denom;
        if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) return Point<T>(p1.x + uA*(p2.x - p1.x), p1.y + uA*(p2.y - p1.y));
        return std::nullopt;
    }

    bool slopesIntersect(const Line<T>& other) const { return slope() != other.slope(); }

    std::optional<Point<T>> slopeIntersection(const Line<T>& other) const {
        float denom = (other.p2.y - other.p1.y)*(p2.x - p1.x) - (other.p2.x - other.p1.x)*(p2.y - p1.y);
        if (denom == 0.0f) return std::nullopt;
        float uA = ((other.p2.x - other.p1.x)*(p1.y - other.p1.y) - (other.p2.y - other.p1.y)*(p1.x - other.p1.x)) / denom;
        return Point<T>(p1.x + uA*(p2.x - p1.x), p1.y + uA*(p2.y - p1.y));
    }

    bool intersects(const Rectangle<T>& rect) const {
        const auto& edges = rect.getEdges();

        int count = 0;

        if (rect.contains(p1)) count++;
        if (rect.contains(p2)) count++;
        if (count == 2) return true;

        for (const auto& edge : edges) if (intersects(edge) && (++count == 2)) return true;

        return false;
    }

    std::optional<Line<T>> intersection(const Rectangle<T>& rect) const {
        const auto& edges = rect.getEdges();

        std::vector<Point<T>> points;

        if (rect.contains(p1)) points.push_back(p1);
        if (rect.contains(p2)) points.push_back(p2);
        if (points.size() == 2) return Line<T>(points[0], points[1]); // Fully contained line

        for (const auto& edge : edges) {
            auto point = intersection(edge);
            if (point) {
                points.push_back(point);
                if (points.size() == 2) return Line<T>(points[0], points[1]);
            }
        }

        return std::nullopt;
    };

    bool intersects(const std::vector<Rectangle<T>>& rectangles) const { for (const auto& rectangle : rectangles) if (intersects(rectangle)) return true; return false; }

    std::vector<Line<T>> intersection(const std::vector<Rectangle<T>>& rectangles) const {
        std::vector<Line<T>> lines;

        for (const auto& rectangle : rectangles) {
            auto line = intersection(rectangle);
            if (line) lines.push_back(line);
        }

        return lines;
    };

    // ====== [TRANSLATION] ====== //

    Line<T> operator+(const Point<T>& translate) const { return Line<T>(p1 + translate, p2 + translate); }
    Line<T>& operator+=(const Point<T>& translate) { p1 += translate; p2 += translate; return *this; }
    Line<T> operator-(const Point<T>& translate) const { return Line<T>(p1 - translate, p2 - translate); }
    Line<T>& operator-=(const Point<T>& translate) { p1 -= translate; p2 -= translate; return *this; }

    void translate(T deltaX, T deltaY) { p1.translate(deltaX, deltaY); p2.translate(deltaX, deltaY); }
    void translate(const Point<T>& delta) { p1.translate(delta); p2.translate(delta); }
    Line<T> translated(T deltaX, T deltaY) const { return Line<T>(p1.translated(deltaX, deltaY), p2.translated(deltaX, deltaY)); }
    Line<T> translated(const Point<T>& delta) const { return Line<T>(p1.translated(delta), p2.translated(delta)); }

    void interpolate(const Line<T>& other, float t) { p1.interpolate(other.p1, t); p2.interpolate(other.p2, t); }
    Line<T> interpolated(const Line<T>& other, float t) const { return Line<T>(p1.interpolated(other.p1, t), p2.interpolated(other.p2, t)); }

    // ====== [SCALING] ====== //

    Line<T> operator*(float scalar) const { return Line<T>(p1 * scalar, p2 * scalar); }
    Line<T>& operator*=(const Point<T>& scale) { p1 *= scale; p2 *= scale; return *this; }
    Line<T> operator/(float scalar) const { return Line<T>(p1 / scalar, p2 / scalar); }
    Line<T>& operator/=(const Point<T>& scale) { p1 /= scale; p2 /= scale; return *this; }

    void scaleFromOrigin(float scale) { p1.scaleFromOrigin(scale); p2.scaleFromOrigin(scale); }
    Point<T> scaledFromOrigin(float scale) { return Point<T>(p1.scaledFromOrigin(scale), p2.scaledFromOrigin(scale)); }

    void scaleFromPoint(const Point<T>& other, float scale) { p1.scaleFromPoint(other, scale); p2.scaleFromPoint(other, scale); }
    Point<T> scaledFromPoint(const Point<T>& other, float scale) { return Point<T>(p1.scaledFromPoint(other, scale), p2.scaledFromPoint(other, scale)); }

    // ====== [ROTATION] ====== //

    void rotateAroundOrigin(float angle) { p1.rotateAroundOrigin(angle); p2.rotateAroundOrigin(angle); }
    Point<T> rotatedAroundOrigin(float angle) { return Point<T>(p1.rotatedAroundOrigin(angle), p2.rotatedAroundOrigin(angle)); }

    void rotateAroundPoint(const Point<T>& pivot, float angle) { p1.rotateAroundPoint(pivot, angle); p2.rotateAroundPoint(pivot, angle); }
    Point<T> rotatedAroundPoint(const Point<T>& pivot, float angle) { return Point<T>(p1.rotatedAroundPoint(pivot, angle), p2.rotatedAroundPoint(pivot, angle)); }

    // ====== [REFLECTION] ====== //

    void reflectAcrossHorizontal(T horizontal) { p1.reflectAcrossHorizontal(horizontal); p2.reflectAcrossHorizontal(horizontal); }
    Point<T> reflectedAcrossHorizontal(T horizontal) { return Point<T>(p1.reflectedAcrossHorizontal(horizontal), p2.reflectedAcrossHorizontal(horizontal)); }

    void reflectAcrossVertical(T vertical) { p1.reflectAcrossVertical(vertical); p2.reflectAcrossVertical(vertical); }
    Point<T> reflectedAcrossVertical(T vertical) { return Point<T>(p1.reflectedAcrossVertical(vertical), p2.reflectedAcrossVertical(vertical)); }

    void reflectAcrossLine(Line<T> line) { p1.reflectAcrossLine(line); p2.reflectAcrossLine(line); }
    Point<T> reflectedAcrossLine(Line<T> line) { return Point<T>(p1.reflectedAcrossLine(line), p2.reflectedAcrossLine(line)); }

    // ====== [CONVERSION] ====== //

    Line<int> toInt() const { return Line<int>(p1.toInt(), p2.toInt()); }
    Line<float> toFloat() const { return Line<float>(p1.toFloat(), p2.toFloat()); }
};
