#include "rectangle.h"
#include "line.h"

template <typename T>
inline T Rectangle<T>::area() const { return width * height; }

template <typename T>
T Rectangle<T>::perimeter() const { width + width + height + height; }

template <typename T>
bool Rectangle<T>::isEmpty() const { return width == 0 && height == 0; }

template <typename T>
void Rectangle<T>::translate(T deltaX, T deltaY) { position.translate(deltaX, deltaY); }

template <typename T>
inline void Rectangle<T>::translate(Point<T> delta) { position += delta; }

template <typename T>
inline T Rectangle<T>::getTop() const { return Point<T>::align(*this).y; }

template <typename T>
T Rectangle<T>::getBottom() const { return Point<T>::align(*this).y + height; }

template <typename T>
T Rectangle<T>::getLeft() const { return Point<T>::align(*this).x; }

template <typename T>
T Rectangle<T>::getRight() const { return Point<T>::align(*this).x + width; }

template <typename T>
T Rectangle<T>::getCenterX() const { return getLeft() + (width / 2); }

template <typename T>
T Rectangle<T>::getCenterY() const { return getTop() + (height / 2); }

template <typename T>
Point<T> Rectangle<T>::getTopLeft() const { return Point<T>::align(position, width, height, Alignment::TopLeft); }

template <typename T>
Point<T> Rectangle<T>::getBottomLeft() const { return Point<T>::align(position, width, height, Alignment::BottomLeft); }

template <typename T>
Point<T> Rectangle<T>::getTopRight() const { return Point<T>::align(position, width, height, Alignment::TopRight); }

template <typename T>
Point<T> Rectangle<T>::getBottomRight() const { return Point<T>::align(position, width, height, Alignment::BottomRight); }

template <typename T>
Point<T> Rectangle<T>::getCenter() const { return Point<T>::align(position, width, height, Alignment::Center); }

template <typename T>
Line<T> Rectangle<T>::getTopLine() const { return Line<T>(getTopLeft(), getTopRight()); }

template <typename T>
Line<T> Rectangle<T>::getBottomLine() const { return Line<T>(getBottomRight(), getBottomLeft()); }

template <typename T>
Line<T> Rectangle<T>::getLeftLine() const { return Line<T>(getBottomLeft(), getTopLeft()); }

template <typename T>
Line<T> Rectangle<T>::getRightLine() const { return Line<T>(getTopRight(), getBottomRight()); }
