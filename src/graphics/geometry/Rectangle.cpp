#include "rectangle.h"
#include <algorithm>
#include <cmath>

template<typename T>
Rectangle<T>::Rectangle(T x, T y, T w, T h)
    : Rectangle(Point<T>(x, y), w, h) {}

template<typename T>
Rectangle<T>::Rectangle(const Point<T>& pos, T w, T h)
    : position(pos), width(w), height(h) {}

template<typename T>
Rectangle<T> Rectangle<T>::fromPoints(Point<T> p1, Point<T> p2) {
    return Rectangle<T>(p1, p2.x - p1.x, p2.y - p1.y);
}

template<typename T>
T Rectangle<T>::area() const { return width * height; }

template<typename T>
T Rectangle<T>::perimeter() const { return width + width + height + height; }

template<typename T>
bool Rectangle<T>::isEmpty() const { return width == 0 && height == 0; }

template<typename T>
bool Rectangle<T>::operator==(const Rectangle<T>& other) const {
    return position == other.position && width == other.width && height == other.height;
}

template<typename T>
bool Rectangle<T>::operator!=(const Rectangle<T>& other) const {
    return !(*this == other);
}

// INTERSECTIONS

template<typename T>
bool Rectangle<T>::contains(const Point<T>& point) const {
    return point.x >= getLeft() && point.x <= getRight() && point.y >= getTop() && point.y <= getBottom();
}

template<typename T>
bool Rectangle<T>::intersects(const Rectangle<T>& other) const {
    return (getRight() >= other.getLeft() && getLeft() <= other.getRight() && getBottom() >= other.getTop() && getTop() <= other.getBottom());
}

template<typename T>
std::optional<Rectangle<T>> Rectangle<T>::intersection(const Rectangle<T>& other) const {
    T left = std::max(getLeft(), other.getLeft());
    T top = std::max(getTop(), other.getTop());
    T right = std::min(getRight(), other.getRight());
    T bottom = std::min(getBottom(), other.getBottom());

    if (left < right && top < bottom) {
        return Rectangle<T>(Point<T>(left, top), right - left, bottom - top);
    }
    return std::nullopt;
}

template<typename T>
bool Rectangle<T>::intersects(const Line<T>& line) const { return line.intersects(*this); }

template<typename T>
std::optional<Line<T>> Rectangle<T>::intersection(const Line<T>& line) const { return line.intersection(*this); }

template <typename T>
bool Rectangle<T>::contains(const Line<T> &line) { return contains(line.p1) && contains(line.p2); }

/*
template<typename T>
void Rectangle<T>::clip(const Rectangle<T>& other) {
    auto opt = intersection(other);
    if (opt) {
        *this = *opt;
    } else {
        // no overlap -> make empty rectangle positioned at intersection coords
        T left = std::max(getLeft(), other.getLeft());
        T top = std::max(getTop(), other.getTop());
        position = Point<T>(left, top);
        width = 0;
        height = 0;
    }
}

template<typename T>
Rectangle<T> Rectangle<T>::clipped(const Rectangle<T>& other) const {
    auto opt = intersection(other);
    if (opt) return *opt;
    return Rectangle<T>(Point<T>(0,0), 0, 0);
}
    */

// POSITIONING

template<typename T>
void Rectangle<T>::setTop(T top) {
    T bottom = getBottom();
    position.y = top;
    height = bottom - top;
}

template<typename T>
void Rectangle<T>::setBottom(T bottom) {
    position.y = bottom - height;
    height = bottom - position.y;
}

template<typename T>
void Rectangle<T>::setLeft(T left) {
    T right = getRight();
    position.x = left;
    width = right - left;
}

template<typename T>
void Rectangle<T>::setRight(T right) {
    position.x = right - width;
    width = right - position.x;
}

template<typename T>
void Rectangle<T>::setCenterX(T centerX) {
    position.x = centerX - (width / (T)2);
}

template<typename T>
void Rectangle<T>::setCenterY(T centerY) {
    position.y = centerY - (height / (T)2);
}

template<typename T>
T Rectangle<T>::getTop() const { return position.y; }
template<typename T>
T Rectangle<T>::getBottom() const { return position.y + height; }
template<typename T>
T Rectangle<T>::getLeft() const { return position.x; }
template<typename T>
T Rectangle<T>::getRight() const { return position.x + width; }
template<typename T>
T Rectangle<T>::getCenterX() const { return getLeft() + (width / (T)2); }
template<typename T>
T Rectangle<T>::getCenterY() const { return getTop() + (height / (T)2); }

template<typename T>
Point<T> Rectangle<T>::getTopLeft() const { return position; }
template<typename T>
Point<T> Rectangle<T>::getBottomLeft() const { return position.translated((T)0, height); }
template<typename T>
Point<T> Rectangle<T>::getTopRight() const { return position.translated(width, (T)0); }
template<typename T>
Point<T> Rectangle<T>::getBottomRight() const { return position.translated(width, height); }
template<typename T>
Point<T> Rectangle<T>::getCenter() const { return position.translated(width / (T)2, height / (T)2); }

template<typename T>
Line<T> Rectangle<T>::getTopEdge() const { return Line<T>(getTopLeft(), getTopRight()); }
template<typename T>
Line<T> Rectangle<T>::getBottomEdge() const { return Line<T>(getBottomRight(), getBottomLeft()); }
template<typename T>
Line<T> Rectangle<T>::getLeftEdge() const { return Line<T>(getBottomLeft(), getTopLeft()); }
template<typename T>
Line<T> Rectangle<T>::getRightEdge() const { return Line<T>(getTopRight(), getBottomRight()); }

template<typename T>
std::vector<Line<T>> Rectangle<T>::getEdges() const {
    std::vector<Line<T>> edges;
    edges.reserve(4);
    edges.push_back(getTopEdge());
    edges.push_back(getRightEdge());
    edges.push_back(getBottomEdge());
    edges.push_back(getLeftEdge());
    return edges;
}

// SHAPING

template<typename T>
void Rectangle<T>::trimTop(T amount) {
    position.y += amount;
    height -= amount;
}
template<typename T>
void Rectangle<T>::trimBottom(T amount) { height -= amount; }
template<typename T>
void Rectangle<T>::trimLeft(T amount) { position.x += amount; width -= amount; }
template<typename T>
void Rectangle<T>::trimRight(T amount) { width -= amount; }

template<typename T>
Rectangle<T> Rectangle<T>::trimmedTop(T amount) const {
    return Rectangle<T>(position.translated((T)0, amount), width, height - amount);
}
template<typename T>
Rectangle<T> Rectangle<T>::trimmedBottom(T amount) const {
    return Rectangle<T>(position, width, height - amount);
}
template<typename T>
Rectangle<T> Rectangle<T>::trimmedLeft(T amount) const {
    return Rectangle<T>(position.translated(amount, (T)0), width - amount, height);
}
template<typename T>
Rectangle<T> Rectangle<T>::trimmedRight(T amount) const {
    return Rectangle<T>(position, width - amount, height);
}

template<typename T>
Rectangle<T> Rectangle<T>::removeFromTop(T amount) {
    Rectangle<T> removed(position, width, amount);
    position.y += amount;
    height -= amount;
    return removed;
}
template<typename T>
Rectangle<T> Rectangle<T>::removeFromBottom(T amount) {
    Rectangle<T> removed(Point<T>(position.x, position.y + height - amount), width, amount);
    height -= amount;
    return removed;
}
template<typename T>
Rectangle<T> Rectangle<T>::removeFromLeft(T amount) {
    Rectangle<T> removed(position, amount, height);
    position.x += amount;
    width -= amount;
    return removed;
}
template<typename T>
Rectangle<T> Rectangle<T>::removeFromRight(T amount) {
    Rectangle<T> removed(Point<T>(position.x + width - amount, position.y), amount, height);
    width -= amount;
    return removed;
}

// TRANSLATION

template<typename T>
Rectangle<T> Rectangle<T>::operator+(Point<T> delta) const { return translated(delta); }
template<typename T>
Rectangle<T>& Rectangle<T>::operator+=(Point<T> delta) { translate(delta); return *this; }
template<typename T>
Rectangle<T> Rectangle<T>::operator-(Point<T> delta) const { return translated(-delta); }
template<typename T>
Rectangle<T>& Rectangle<T>::operator-=(Point<T> delta) { translate(-delta); return *this; }

template<typename T>
void Rectangle<T>::translate(T deltaX, T deltaY) { position.translate(deltaX, deltaY); }
template<typename T>
void Rectangle<T>::translate(Point<T> delta) { position += delta; }
template<typename T>
Rectangle<T> Rectangle<T>::translated(T deltaX, T deltaY) const { return Rectangle<T>(Point<T>(position.x + deltaX, position.y + deltaY), width, height); }
template<typename T>
Rectangle<T> Rectangle<T>::translated(Point<T> delta) const { return Rectangle<T>(position + delta, width, height); }

template<typename T>
void Rectangle<T>::interpolate(const Rectangle<T>& other, float t) {
    position.interpolate(other.position, t);
    width = lerp(width, other.width, t);
    height = lerp(height, other.height, t);
}
template<typename T>
Rectangle<T> Rectangle<T>::interpolated(const Rectangle<T>& other, float t) const {
    return Rectangle<T>(position.interpolated(other.position, t), lerp(width, other.width, t), lerp(height, other.height, t));
}

// SCALING

template<typename T>
Rectangle<T> Rectangle<T>::operator*(float scale) const { return Rectangle<T>(position * scale, width * scale, height * scale); }
template<typename T>
Rectangle<T>& Rectangle<T>::operator*=(float scale) { position *= scale; width *= scale; height *= scale; return *this; }
template<typename T>
Rectangle<T> Rectangle<T>::operator/(float scale) const { return Rectangle<T>(position / scale, width / scale, height / scale); }
template<typename T>
Rectangle<T>& Rectangle<T>::operator/=(float scale) { position /= scale; width /= scale; height /= scale; return *this; }

template<typename T>
void Rectangle<T>::scaleFromOrigin(float scale) { position.scaleFromOrigin(scale); width *= scale; height *= scale; }
template<typename T>
Rectangle<T> Rectangle<T>::scaledFromOrigin(float scale) const { return Rectangle<T>(position.scaledFromOrigin(scale), width * scale, height * scale); }

template<typename T>
void Rectangle<T>::scaleFromPoint(const Point<T>& other, float scale) { position.scaleFromPoint(other, scale); width *= scale; height *= scale; }
template<typename T>
Rectangle<T> Rectangle<T>::scaledFromPoint(const Point<T>& other, float scale) const { return Rectangle<T>(position.scaledFromPoint(other, scale), width * scale, height * scale); }

template<typename T>
void Rectangle<T>::reduce(T deltaX, T deltaY) { position.translate(deltaX, deltaY);  width -= (T)(2 * deltaX); height -= (T)(2 * deltaY); }
template<typename T>
void Rectangle<T>::reduce(T delta) { reduce(delta, delta); }
template<typename T>
void Rectangle<T>::reduce(BoxSpacing spacing, const Rectangle<float>& parentBounds) {
    BoxSpacingPx spacingPx = spacing.resolve(parentBounds);
    position.translate((T)spacingPx.left, (T)spacingPx.top);
    width -= (T)(spacingPx.left + spacingPx.right);
    height -= (T)(spacingPx.top + spacingPx.bottom);
}
template<typename T>
void Rectangle<T>::reduce(BoxSpacing spacing) { reduce(spacing, toFloat()); }

template<typename T>
Rectangle<T> Rectangle<T>::reduced(T deltaX, T deltaY) const { return Rectangle<T>(position.translated(deltaX, deltaY), width - (T)(2 * deltaX), height - (T)(2 * deltaY)); }
template<typename T>
Rectangle<T> Rectangle<T>::reduced(T delta) const { return reduced(delta, delta); }
template<typename T>
Rectangle<T> Rectangle<T>::reduced(BoxSpacing spacing, const Rectangle<float>& parentBounds) const {
    BoxSpacingPx spacingPx = spacing.resolve(parentBounds);
    return Rectangle<T>(position.translated((T)spacingPx.left, (T)spacingPx.top), width - (T)(spacingPx.left + spacingPx.right), height - (T)(spacingPx.top + spacingPx.bottom));
}
template<typename T>
Rectangle<T> Rectangle<T>::reduced(BoxSpacing spacing) const { return reduced(spacing, toFloat()); }

template<typename T>
void Rectangle<T>::expand(T deltaX, T deltaY) { position.translate(-deltaX, -deltaY);  width += (T)(2 * deltaX); height += (T)(2 * deltaY); }
template<typename T>
void Rectangle<T>::expand(T delta) { expand(delta, delta); }
template<typename T>
void Rectangle<T>::expand(BoxSpacing spacing, const Rectangle<float>& parentBounds) {
    BoxSpacingPx spacingPx = spacing.resolve(parentBounds);
    position.translate((T)-spacingPx.left, (T)-spacingPx.top);
    width += (T)(spacingPx.left + spacingPx.right);
    height += (T)(spacingPx.top + spacingPx.bottom);
}
template<typename T>
void Rectangle<T>::expand(BoxSpacing spacing) { expand(spacing, toFloat()); }

template<typename T>
Rectangle<T> Rectangle<T>::expanded(T deltaX, T deltaY) const { return Rectangle<T>(position.translated((T)-deltaX, (T)-deltaY), width + (T)(2 * deltaX), height + (T)(2 * deltaY)); }
template<typename T>
Rectangle<T> Rectangle<T>::expanded(T delta) const { return expanded(delta, delta); }
template<typename T>
Rectangle<T> Rectangle<T>::expanded(BoxSpacing spacing, const Rectangle<float>& parentBounds) const {
    BoxSpacingPx spacingPx = spacing.resolve(parentBounds);
    return Rectangle<T>(position.translated((T)-spacingPx.left, (T)-spacingPx.top), width + (T)(spacingPx.left + spacingPx.right), height + (T)(spacingPx.top + spacingPx.bottom));
}
template<typename T>
Rectangle<T> Rectangle<T>::expanded(BoxSpacing spacing) const { return expanded(spacing, toFloat()); }

// ROTATION helpers: compute bounding box of rotated corners
template<typename T>
static Rectangle<T> rectangleFromRotatedCorners(const std::array<Point<T>,4>& corners) {
    T minx = corners[0].x, maxx = corners[0].x;
    T miny = corners[0].y, maxy = corners[0].y;
    for (size_t i=1;i<4;++i) {
        minx = std::min(minx, corners[i].x);
        maxx = std::max(maxx, corners[i].x);
        miny = std::min(miny, corners[i].y);
        maxy = std::max(maxy, corners[i].y);
    }
    return Rectangle<T>(Point<T>(minx, miny), maxx - minx, maxy - miny);
}

template<typename T>
void Rectangle<T>::rotateAroundOrigin(float angle) {
    *this = rotatedAroundOrigin(angle);
}
template<typename T>
Rectangle<T> Rectangle<T>::rotatedAroundOrigin(float angle) const {
    float s = std::sin(angle);
    float c = std::cos(angle);
    auto tl = getTopLeft();
    auto tr = getTopRight();
    auto bl = getBottomLeft();
    auto br = getBottomRight();

    std::array<Point<T>,4> pts = { tl, tr, bl, br };
    std::array<Point<T>,4> rpts;
    for (int i=0;i<4;++i) {
        float x = static_cast<float>(pts[i].x);
        float y = static_cast<float>(pts[i].y);
        float rx = x * c - y * s;
        float ry = x * s + y * c;
        rpts[i] = Point<T>((T)rx, (T)ry);
    }
    return rectangleFromRotatedCorners(rpts);
}

template<typename T>
void Rectangle<T>::rotateAroundPoint(const Point<T>& other, float angle) {
    *this = rotatedAroundPoint(other, angle);
}
template<typename T>
Rectangle<T> Rectangle<T>::rotatedAroundPoint(const Point<T>& other, float angle) const {
    float s = std::sin(angle);
    float c = std::cos(angle);
    auto tl = getTopLeft();
    auto tr = getTopRight();
    auto bl = getBottomLeft();
    auto br = getBottomRight();

    std::array<Point<T>,4> pts = { tl, tr, bl, br };
    std::array<Point<T>,4> rpts;
    for (int i=0;i<4;++i) {
        float x = static_cast<float>(pts[i].x - other.x);
        float y = static_cast<float>(pts[i].y - other.y);
        float rx = x * c - y * s + other.x;
        float ry = x * s + y * c + other.y;
        rpts[i] = Point<T>((T)rx, (T)ry);
    }
    return rectangleFromRotatedCorners(rpts);
}

// REFLECTION

template<typename T>
void Rectangle<T>::reflectAcrossHorizontal(T horizontal) {
    *this = reflectedAcrossHorizontal(horizontal);
}
template<typename T>
Rectangle<T> Rectangle<T>::reflectedAcrossHorizontal(T horizontal) const {
    auto tl = getTopLeft();
    auto tr = getTopRight();
    auto bl = getBottomLeft();
    auto br = getBottomRight();
    std::array<Point<T>,4> pts = { tl, tr, bl, br };
    for (auto &p : pts) {
        p.y = (T)(2 * horizontal - p.y);
    }
    return rectangleFromRotatedCorners(pts);
}

template<typename T>
void Rectangle<T>::reflectAcrossVertical(T vertical) {
    *this = reflectedAcrossVertical(vertical);
}
template<typename T>
Rectangle<T> Rectangle<T>::reflectedAcrossVertical(T vertical) const {
    auto tl = getTopLeft();
    auto tr = getTopRight();
    auto bl = getBottomLeft();
    auto br = getBottomRight();
    std::array<Point<T>,4> pts = { tl, tr, bl, br };
    for (auto &p : pts) {
        p.x = (T)(2 * vertical - p.x);
    }
    return rectangleFromRotatedCorners(pts);
}

// CONVERSION

template<typename T>
Rectangle<float> Rectangle<T>::toFloat() const { return Rectangle<float>(position.toFloat(), (float)width, (float)height); }
template<typename T>
Rectangle<int> Rectangle<T>::toInt() const { return Rectangle<int>(position.toInt(), (int)width, (int)height); }

template <typename T>
Path<T> Rectangle<T>::asPath() const { return Path<T>(*this); }

template struct Rectangle<int>;
template struct Rectangle<float>;