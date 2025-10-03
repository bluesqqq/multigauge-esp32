#pragma once

#include <optional>

#include "point.h"
#include "alignment.h"
#include "line.h"

template <typename T>
struct Rectangle {
    Point<T> position;
    T width, height;
    Alignment alignment;

    Rectangle(Point<T> position, T width, T height, Alignment alignment = Alignment::TopLeft);

    static Rectangle<T> fromPoints(Point<T> p1, Point<T> p2);

    T area() const;
    T perimeter() const;
    bool isEmpty() const;
    
    // ====== [INTERSECTIONS] ====== //

    /// @brief Checks if a point lies inside this rectangle.
    /// @param point The point to test.
    /// @return True if the point is inside the rectangle, false otherwise.
    bool contains(const Point<T>& point) const;

    /// @brief Checks if this rectangle intersects with another rectangle.
    /// @param other The other rectangle to test against.
    /// @return True if the rectangles overlap, false otherwise.
    bool intersects(const Rectangle<T>& other);

    /// @brief Computes the intersection of this rectangle with another rectangle.
    /// @param other The other rectangle to intersect with.
    /// @return An optional containing the intersecting rectangle if an intersection exists, std::nullopt otherwise.
    std::optional<Rectangle<T>> intersection(const Rectangle<T>& other);

    /// @brief Checks if a line intersects with this rectangle.
    /// @param line The line to test.
    /// @return True if the line intersects the rectangle, false otherwise.
    bool intersects(const Line<T>& line);

    /// @brief Computes the intersection of this rectangle with a line.
    /// @param line The line to intersect with.
    /// @return An optional containing the intersecting line segment if an intersection exists, std::nullopt otherwise.
    std::optional<Line<T>> intersection(const Line<T>& line);

    /// @brief Clips this rectangle so that it lies completely within another rectangle.
    /// @param other The rectangle to use as the clipping bounds.
    void clip(const Rectangle<T>& other);

    /// @brief Returns a new rectangle that is clipped by another rectangle.
    /// @param other The rectangle to use as the clipping bounds.
    /// @return A new rectangle representing this rectangle clipped by the bounds of the other rectangle.
    Rectangle<T> clipped(const Rectangle<T>& other) const;

    // ====== [POSITIONING] ====== //

    Rectangle<T> operator+(Point<T> delta);
    Rectangle<T>& operator+=(Point<T> delta);
    Rectangle<T> operator-(Point<T> delta);
    Rectangle<T>& operator-=(Point<T> delta);

    bool operator==(const Rectangle<T>& other);
    bool operator!=(const Rectangle<T>& other);

    void setTop(T top);
    void setBottom(T bottom);
    void setLeft(T left);
    void setRight(T right);
    void setCenterX(T centerX);
    void setCenterY(T centerY);

    T getTop()    const;
    T getBottom() const;
    T getLeft()   const;
    T getRight()  const;
    T getCenterX() const;
    T getCenterY() const;

    Point<T> getTopLeft() const;
    Point<T> getBottomLeft() const;
    Point<T> getTopRight() const;
    Point<T> getBottomRight() const;
    Point<T> getCenter() const;

    Line<T> getTopLine() const;
    Line<T> getBottomLine() const;
    Line<T> getLeftLine() const;
    Line<T> getRightLine() const;

    // ====== [SHAPING] ====== //

    Rectangle<T>  operator*(float scale);
    Rectangle<T>& operator*=(float scale);
    Rectangle<T>  operator/(float scale);
    Rectangle<T>& operator/=(float scale);

    void trimTop(T amount);
    void trimBottom(T amount);
    void trimLeft(T amount);
    void trimRight(T amount);
    Rectangle<T> trimmedTop(T amount);
    Rectangle<T> trimmedBottom(T amount);
    Rectangle<T> trimmedLeft(T amount);
    Rectangle<T> trimmedRight(T amount);

    Rectangle<T> removeFromTop(T amount);
    Rectangle<T> removeFromBottom(T amount);
    Rectangle<T> removeFromLeft(T amount);
    Rectangle<T> removeFromRight(T amount);

    // ====== [TRANSLATION] ====== //

    void translate(T deltaX, T deltaY);
    void translate(Point<T> delta);
    Rectangle<T> translated(T deltaX, T deltaY) const;
    Rectangle<T> translated(Point<T> delta) const;

    void interpolate(const Rectangle<T>& other, float t);
    Rectangle<T> interpolated(const Rectangle<T>& other, float t) const;

    // ====== [SCALING] ====== //

    void scaleFromOrigin(float scale);
    Point<T> scaledFromOrigin(float scale);

    void scaleFromPoint(const Point<T>& other, float scale);
    Point<T> scaledFromPoint(const Point<T>& other, float scale);

    void reduce(T deltaX, T deltaY);
    void reduce(T delta);
    Rectangle<T> reduced(T deltaX, T deltaY);
    Rectangle<T> reduced(T delta);

    void expand(T deltaX, T deltaY);
    void expand(T delta);
    Rectangle<T> expanded(T deltaX, T deltaY);
    Rectangle<T> expanded(T delta);

    // ====== [ROTATION] ====== //

    void rotateAroundOrigin(float angle);
    Rectangle<T> rotatedAroundOrigin(float angle);

    void rotateAroundPoint(const Point<T>& other, float angle);
    Rectangle<T> rotatedAroundPoint(const Point<T>& other, float angle);

    // ====== [REFLECTION] ====== //

    void reflectAcrossHorizontal(T horizontal);
    Rectangle<T> reflectedAcrossHorizontal(T horizontal);

    void reflectAcrossVertical(T vertical);
    Rectangle<T> reflectedAcrossVertical(T vertical);

    // Reflecting across a line should return a path
    //Rectangle<T> reflectedAcrossLine(Line<T> line);

    // ====== [CONVERSION] ====== //

    Rectangle<float> toFloat() const;
    Rectangle<int> toInt() const;
};