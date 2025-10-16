#pragma once

#include <optional>

#include "point.h"
#include "alignment.h"
#include "line.h"
#include "utils.h"
#include "graphics/gauge/Element.h"

template <typename T>
struct Rectangle {
    Point<T> position;
    T width, height;

    Rectangle(T x, T y, T width, T height) : Rectangle(Point<T>(x, y), width, height) {}

    Rectangle(const Point<T>& pos, T w, T h) : position(pos), width(w), height(h) {}

    static Rectangle<T> fromPoints(Point<T> p1, Point<T> p2) { return Rectangle<T>(p1, p2.x - p1.x, p2.y - p1.y); }

    T area() const { return width * height; }
    T perimeter() const { width + width + height + height; }
    bool isEmpty() const { return width == 0 && height == 0; }
    
    bool operator==(const Rectangle<T>& other) { return position == other.position && width == width && height == height; }
    bool operator!=(const Rectangle<T>& other) { return position != other.position || width != width || height != height; }
    
    // ====== [INTERSECTIONS] ====== //

    /// @brief Checks if a point lies inside this rectangle.
    /// @param point The point to test.
    /// @return True if the point is inside the rectangle, false otherwise.
    bool contains(const Point<T>& point) const { return point.x >= getLeft() && point.x <= getRight() && point.y >= getTop() && point.y <= getBottom(); }

    /// @brief Checks if this rectangle intersects with another rectangle.
    /// @param other The other rectangle to test against.
    /// @return True if the rectangles overlap, false otherwise.
    bool intersects(const Rectangle<T>& other) {
        return (getRight() >= other.getLeft() && getLeft() <= other.getRight() && getBottom() >= other.getTop() && getTop() <= other.getBottom());
    };

    /// @brief Computes the intersection of this rectangle with another rectangle.
    /// @param other The other rectangle to intersect with.
    /// @return An optional containing the intersecting rectangle if an intersection exists, std::nullopt otherwise.
    std::optional<Rectangle<T>> intersection(const Rectangle<T>& other);

    /// @brief Checks if a line intersects with this rectangle.
    /// @param line The line to test.
    /// @return True if the line intersects the rectangle, false otherwise.
    bool intersects(const Line<T>& line) { return line.intersects(*this); };

    /// @brief Computes the intersection of this rectangle with a line.
    /// @param line The line to intersect with.
    /// @return An optional containing the intersecting line segment if an intersection exists, std::nullopt otherwise.
    std::optional<Line<T>> intersection(const Line<T>& line) { return line.intersection(*this); }

    /// @brief Clips this rectangle so that it lies completely within another rectangle.
    /// @param other The rectangle to use as the clipping bounds.
    void clip(const Rectangle<T>& other);

    /// @brief Returns a new rectangle that is clipped by another rectangle.
    /// @param other The rectangle to use as the clipping bounds.
    /// @return A new rectangle representing this rectangle clipped by the bounds of the other rectangle.
    Rectangle<T> clipped(const Rectangle<T>& other) const;

    // ====== [POSITIONING] ====== //

    void setTop(T top);
    void setBottom(T bottom);
    void setLeft(T left);
    void setRight(T right);
    void setCenterX(T centerX);
    void setCenterY(T centerY);

    T getTop()     const { return position.y; }
    T getBottom()  const { return position.y + height; }
    T getLeft()    const { return position.x; }
    T getRight()   const { return position.x + width; }
    T getCenterX() const { return getLeft() + (width / 2); }
    T getCenterY() const { return getTop() + (height / 2); }

    Point<T> getTopLeft()     const { return position; }
    Point<T> getBottomLeft()  const { return position.translated(0.0f, height); }
    Point<T> getTopRight()    const { return position.translated(width, 0.0f); }
    Point<T> getBottomRight() const { return position.translated(width, height); }
    Point<T> getCenter()      const { return position.translated(width / 2.0f, height / 2.0f); }

    Line<T> getTopEdge()    const { return Line<T>(getTopLeft(), getTopRight()); }
    Line<T> getBottomEdge() const { return Line<T>(getBottomRight(), getBottomLeft()); }
    Line<T> getLeftEdge()   const { return Line<T>(getBottomLeft(), getTopLeft()); }
    Line<T> getRightEdge()  const { return Line<T>(getTopRight(), getBottomRight()); }
    std::vector<Line<T>> getEdges() const { 
        std::vector<Line<T>> edges;
        edges.reserve(4);
        edges.push_back(getTopEdge());
        edges.push_back(getRightEdge());
        edges.push_back(getBottomEdge());
        edges.push_back(getLeft());
    }

    // ====== [SHAPING] ====== //

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

    Rectangle<T> operator+(Point<T> delta) { return translated(delta); }
    Rectangle<T>& operator+=(Point<T> delta) { translate(delta); }
    Rectangle<T> operator-(Point<T> delta) { return translated(-delta); }
    Rectangle<T>& operator-=(Point<T> delta) { translate(-delta); }

    void translate(T deltaX, T deltaY) { position.translate(deltaX, deltaY); }
    void translate(Point<T> delta) { position += delta; }
    Rectangle<T> translated(T deltaX, T deltaY) const { return Rectangle<T>(Point<T>(position.x + deltaX, position.y + deltaY), width, height); }
    Rectangle<T> translated(Point<T> delta) const { return Rectangle<T>(position + delta, width, height); };

    void interpolate(const Rectangle<T>& other, float t) { position.interpolate(other, t); width = lerp(width, other.width, t); height = lerp(height, other.height, t); }
    Rectangle<T> interpolated(const Rectangle<T>& other, float t) const { return Rectangle<T>(position.interpolated(other, t),  lerp(width, other.width, t), lerp(height, other.height, t)); }

    // ====== [SCALING] ====== //

    Rectangle<T>  operator*(float scale) { return Rectangle<T>(position * scale, width * scale, height * scale); }
    Rectangle<T>& operator*=(float scale) { position *= scale; width *= scale; height *= scale; };
    Rectangle<T>  operator/(float scale) { return Rectangle<T>(position / scale, width / scale, height / scale); }
    Rectangle<T>& operator/=(float scale) { position /= scale; width /= scale; height /= scale; };

    void scaleFromOrigin(float scale) { position.scaleFromOrigin(scale); width *= scale; height *= scale; }
    Point<T> scaledFromOrigin(float scale) { return Rectangle<T>(position.scaledFromOrigin(scale), width * scale, height * scale); }

    void scaleFromPoint(const Point<T>& other, float scale) { position.scaleFromPoint(other, scale); width *= scale; height *= scale; }
    Point<T> scaledFromPoint(const Point<T>& other, float scale) { return Rectangle<T>(position.scaledFromPoint(other, scale), width * scale, height * scale); }

    void reduce(T deltaX, T deltaY) { position.translate(deltaX, deltaY);  width -= 2 * deltaX; height -= 2 * deltaY; }
    void reduce(T delta) { reduce(delta, delta); }
    void reduce(BoxSpacing spacing, const Rectangle<T>& parentBounds) {
        BoxSpacing spacingPx = spacing.resolve(parentBounds.width, parentBounds.height);
        position.translate(spacingPx.left, spacingPx.top);
        width -= (spacingPx.left + spacingPx.right);
        height -= (spacingPx.top - spacingPx.bottom);
    }
    void reduce(BoxSpacing spacing) { reduce(spacing, *this); }
    Rectangle<T> reduced(T deltaX, T deltaY) const{ return Rectangle<T>(position.translated(deltaX, deltaY), width - (2 * deltaX), height - (2 * deltaY)); }
    Rectangle<T> reduced(T delta) const { return reduced(delta, delta); }
    Rectangle<T> reduced(BoxSpacing spacing, const Rectangle<T>& parentBounds) const {
        BoxSpacing spacingPx = spacing.resolve(parentBounds.width, parentBounds.height);
        return Rectangle<T>(position.translated(spacingPx.left, spacingPx.top), width - spacingPx.left - spacingPx.right, height - spacingPx.top - spacingPx.bottom);
    }
    Rectangle<T> reduced(BoxSpacing spacing) const { return reduced(spacing, *this); }

    

    void expand(T deltaX, T deltaY) { position.translate(-deltaX, -deltaY);  width += 2 * deltaX; height += 2 * deltaY; }
    void expand(T delta) { expand(delta, delta); };
    void expand(BoxSpacing spacing, const Rectangle<T>& parentBounds) {
        BoxSpacing spacingPx = spacing.resolve(parentBounds.width, parentBounds.height);
        position.translate(-spacingPx.left, -spacingPx.top);
        width += (spacingPx.left + spacingPx.right);
        height += (spacingPx.top - spacingPx.bottom);
    }
    void expand(BoxSpacing spacing) { expand(spacing, *this); }
    Rectangle<T> expanded(T deltaX, T deltaY) const { return Rectangle<T>(position.translated(-deltaX, -deltaY), width + (2 * deltaX), height + (2 * deltaY)); }
    Rectangle<T> expanded(T delta) const { return expanded(delta, delta); }
    Rectangle<T> expanded(BoxSpacing spacing, const Rectangle<T>& parentBounds) const {
        BoxSpacing spacingPx = spacing.resolve(parentBounds.width, parentBounds.height);
        return Rectangle<T>(position.translated(-spacingPx.left, -spacingPx.top), width + spacingPx.left + spacingPx.right, height + spacingPx.top + spacingPx.bottom);
    }
    Rectangle<T> expanded(BoxSpacing spacing) const { return expanded(spacing, *this); }

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

    Rectangle<float> toFloat() const { return Rectangle<float>(position.toFloat(), (float)width, (float)height); };
    Rectangle<int> toInt() const { return Rectangle<int>(position.toInt(), (int)width, (int)height); };
};
