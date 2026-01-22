#include "BoxSpacing.h"
#include "rectangle.h"

BoxSpacing::BoxSpacing(Length all) : top(all),  bottom(all), right(all), left(all) {}
BoxSpacing::BoxSpacing(Length vertical, Length horizontal) : top(vertical), bottom(vertical), left(horizontal), right(horizontal) {}
BoxSpacing::BoxSpacing(Length t, Length r, Length b, Length l) : top(t), bottom(b), left(l), right(r) {}

BoxSpacingPx BoxSpacing::resolve(float widthPx, float heightPx) { return { top.resolve(heightPx), right.resolve(widthPx), bottom.resolve(heightPx), left.resolve(widthPx) }; }

BoxSpacingPx BoxSpacing::resolve(const Rectangle<float> &rectangle) { return resolve(rectangle.width, rectangle.height); }