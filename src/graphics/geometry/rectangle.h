#pragma once

#include <optional>

#include "point.h"
#include "alignment.h"
#include "Line.h"
#include "path.h"
#include "utils.h"

#include "graphics/geometry/BoxSpacing.h"
#include <vector>

template <typename T>
struct Rectangle {
    Point<T> position;
    T width, height;

    Rectangle(T x, T y, T width, T height);
    Rectangle(const Point<T>& pos, T w, T h);

    static Rectangle<T> fromPoints(Point<T> p1, Point<T> p2);

    T area() const;
    T perimeter() const;
    bool isEmpty() const;
    
    bool operator==(const Rectangle<T>& other) const;
    bool operator!=(const Rectangle<T>& other) const;
    
    // ====== [INTERSECTIONS] ====== //

    // Point

    bool contains(const Point<T>& point) const;

    // Line

    bool intersects(const Line<T>& line) const;
    std::optional<Line<T>> intersection(const Line<T>& line) const;
    bool contains(const Line<T>& line);

    // Rectangle 

    bool intersects(const Rectangle<T>& other) const;
    std::optional<Rectangle<T>> intersection(const Rectangle<T>& other) const;
    bool contains(const Rectangle<T>& other) const;

    // Circle

    bool intersects(const Circle<T>& circle) const;
    std::optional<Path<T>> intersection(const Circle<T>& circle) const;
    bool contains(const Circle<T>& circle) const;

    // ====== [POSITIONING] ====== //

    void setTop(T top);
    void setBottom(T bottom);
    void setLeft(T left);
    void setRight(T right);
    void setCenterX(T centerX);
    void setCenterY(T centerY);

    T getTop()     const;
    T getBottom()  const;
    T getLeft()    const;
    T getRight()   const;
    T getCenterX() const;
    T getCenterY() const;

    Point<T> getTopLeft()     const;
    Point<T> getBottomLeft()  const;
    Point<T> getTopRight()    const;
    Point<T> getBottomRight() const;
    Point<T> getCenter()      const;

    Line<T> getTopEdge()    const;
    Line<T> getBottomEdge() const;
    Line<T> getLeftEdge()   const;
    Line<T> getRightEdge()  const;
    std::vector<Line<T>> getEdges() const;

    // ====== [SHAPING] ====== //

    void trimTop(T amount);
    void trimBottom(T amount);
    void trimLeft(T amount);
    void trimRight(T amount);

    Rectangle<T> trimmedTop(T amount) const;
    Rectangle<T> trimmedBottom(T amount) const;
    Rectangle<T> trimmedLeft(T amount) const;
    Rectangle<T> trimmedRight(T amount) const;

    Rectangle<T> removeFromTop(T amount);
    Rectangle<T> removeFromBottom(T amount);
    Rectangle<T> removeFromLeft(T amount);
    Rectangle<T> removeFromRight(T amount);

    // ====== [TRANSLATION] ====== //

    Rectangle<T> operator+(Point<T> delta) const;
    Rectangle<T>& operator+=(Point<T> delta);
    Rectangle<T> operator-(Point<T> delta) const;
    Rectangle<T>& operator-=(Point<T> delta);

    void translate(T deltaX, T deltaY);
    void translate(Point<T> delta);
    Rectangle<T> translated(T deltaX, T deltaY) const;
    Rectangle<T> translated(Point<T> delta) const;

    void interpolate(const Rectangle<T>& other, float t);
    Rectangle<T> interpolated(const Rectangle<T>& other, float t) const;

    // ====== [SCALING] ====== //

    Rectangle<T>  operator*(float scale) const;
    Rectangle<T>& operator*=(float scale);
    Rectangle<T>  operator/(float scale) const;
    Rectangle<T>& operator/=(float scale);

    void scaleFromOrigin(float scale);
    Rectangle<T> scaledFromOrigin(float scale) const;

    void scaleFromPoint(const Point<T>& other, float scale);
    Rectangle<T> scaledFromPoint(const Point<T>& other, float scale) const;

    void reduce(T deltaX, T deltaY);
    void reduce(T delta);
    void reduce(BoxSpacing spacing, const Rectangle<float>& parentBounds);
    void reduce(BoxSpacing spacing);

    Rectangle<T> reduced(T deltaX, T deltaY) const;
    Rectangle<T> reduced(T delta) const;
    Rectangle<T> reduced(BoxSpacing spacing, const Rectangle<float>& parentBounds) const;
    Rectangle<T> reduced(BoxSpacing spacing) const;

    void expand(T deltaX, T deltaY);
    void expand(T delta);
    void expand(BoxSpacing spacing, const Rectangle<float>& parentBounds);
    void expand(BoxSpacing spacing);

    Rectangle<T> expanded(T deltaX, T deltaY) const;
    Rectangle<T> expanded(T delta) const;
    Rectangle<T> expanded(BoxSpacing spacing, const Rectangle<float>& parentBounds) const;
    Rectangle<T> expanded(BoxSpacing spacing) const;

    // ====== [ROTATION] ====== //

    void rotateAroundOrigin(float angle);
    Rectangle<T> rotatedAroundOrigin(float angle) const;

    void rotateAroundPoint(const Point<T>& other, float angle);
    Rectangle<T> rotatedAroundPoint(const Point<T>& other, float angle) const;

    // ====== [REFLECTION] ====== //

    void reflectAcrossHorizontal(T horizontal);
    Rectangle<T> reflectedAcrossHorizontal(T horizontal) const;

    void reflectAcrossVertical(T vertical);
    Rectangle<T> reflectedAcrossVertical(T vertical) const;

    // ====== [CONVERSION] ====== //

    Rectangle<float> toFloat() const;
    Rectangle<int> toInt() const;
    Path<T> asPath() const;
};

extern template struct Rectangle<int>;
extern template struct Rectangle<float>;