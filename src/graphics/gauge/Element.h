#pragma once

#include "graphics/colors/Color.h"
#include "graphics/geometry/alignment.h"
#include "graphics/geometry/rectangle.h"

enum class LengthType {
    Pixels,
    Percent
};

struct Length {
    float value;
    LengthType type;

    constexpr Length(float value = 0.0f, LengthType type = LengthType::Pixels) : value(value), type(type) {}

    static constexpr Length px(float value = 1.0f) { return { value, LengthType::Pixels}; }
    static constexpr Length percent(float value = 100.0f) { return { value, LengthType::Percent}; }

    int resolve(int parent) const {
        if (type == LengthType::Percent) return (int)(parent * (value / 100.0f));
        return (int)value;
    }
};

struct BoxSpacing {
    Length top;
    Length right;
    Length bottom;
    Length left;

    constexpr BoxSpacing() = default;
    constexpr BoxSpacing(Length all) : top(all),  bottom(all), right(all), left(all) {}
    constexpr BoxSpacing(Length vertical, Length horizontal) : top(vertical), bottom(vertical), left(horizontal), right(horizontal) {}
    constexpr BoxSpacing(Length t, Length r, Length b, Length l) : top(t), bottom(b), left(l), right(r) {}
    
    BoxSpacing resolve(float parentWidth, float parentHeight) {
        return {
            Length::px(top.resolve(parentHeight)),
            Length::px(bottom.resolve(parentHeight)),
            Length::px(left.resolve(parentWidth)),
            Length::px(right.resolve(parentWidth))
        };
    }

    BoxSpacing resolve(const Rectangle<int>& rectangle) { return resolve(rectangle.width, rectangle.height); }
};

enum class PositionType { Auto, Absolute, Relative };

class Element {
    private:
        // ====== [HIERARCHY] ====== //

        /// @brief Parent element in the hierarchy, or nullptr if root
        Element* parent = nullptr;
        /// @brief Owned child elements
        std::vector<std::unique_ptr<Element>> children;
        
        // ====== [LAYOUT] ====== //

        PositionType positionType = PositionType::Auto;
        Length width = Length::percent();
        Length height = Length::percent();

        Length minimumWidth = Length::px(0);
        Length minimumHeight = Length::px(0);

        Length maximumWidth = Length::px(10000);
        Length maximumHeight = Length::px(10000);

        float grow = 0;
        float shrink = 0;

        /// @brief Spacing outside the element between its bounds and border
        BoxSpacing margin;
        /// @brief Spacing inside the element between its content and border
        BoxSpacing padding;

        /// @brief Child layout direction
        Orientation orientation = Orientation::Vertical;
        /// @brief Main-axis alignment
        Justify justify = Justify::Start;
        /// @brief Cross-axis alignment
        Align align = Align::Center;

        Length gap = Length::px(10.0f);

        // ====== [STYLE] ====== //

        std::unique_ptr<Color> backgroundColor = nullptr;
        std::unique_ptr<Color> strokeColor = nullptr;
        int strokeThickness = 0;
        bool visible = true;
        Length cornerRadius = Length::px(0.0f);

    protected:
        Rectangle<int> bounds;

    public:
        void addChild(std::unique_ptr<Element> child);

        void addChild(JsonObject json);

        void layout(Rectangle<int> parentBounds) {
            BoxSpacing p = padding.resolve(parentBounds);

            auto contentBounds = bounds.reduced(p);

            int totalContentWidthPx = gap.resolve(parentBounds.width) * (children.size() - 1);
            int totalContentHeightPx = 0;

            std::vector<std::pair<int, int>> desiredBoundsPx;
            std::vector<std::pair<int, int>> minMaxWidths;
            std::vector<std::pair<int, int>> minMaxHeights;

            for (auto& child : children) {
                int widthPx    = width.resolve(bounds.width);
                int minWidthPx = minimumWidth.resolve(bounds.width);
                int maxWidthPx = maximumWidth.resolve(bounds.width);

                minMaxWidths.emplace_back(minWidthPx, maxWidthPx);

                int heightPx    = height.resolve(bounds.height);
                int minHeightPx = minimumHeight.resolve(bounds.height);
                int maxHeightPx = maximumHeight.resolve(bounds.height);

                minMaxHeights.emplace_back(minHeightPx, maxHeightPx);

                int desiredWidthPx = std::clamp(widthPx, minWidthPx, maxWidthPx);
                int desiredHeightPx = std::clamp(heightPx, minHeightPx, maxHeightPx);

                desiredBoundsPx.emplace_back(desiredWidthPx, desiredHeightPx);

                totalContentWidthPx += desiredWidthPx;
                totalContentHeightPx += desiredHeightPx;
            }

            int deltaPx = parentBounds.width - totalContentWidthPx;

            if (deltaPx < 0) { // Shrink
                float totalShrink = 0;
                for (auto& child : children)
                    totalShrink += child->shrink;

                for (int i = 0; i < children.size(); i++) {
                    auto& child = children[i];
                    int reductionPixels = (child->shrink / totalShrink) * std::abs(deltaPx);
                    desiredBoundsPx[i].first -= reductionPixels;
                }

            } else if (deltaPx > 0) { // Grow
                float totalGrow = 0;
                for (auto& child : children)
                    totalGrow += child->grow;

                for (int i = 0; i < children.size(); i++) {
                    auto& child = children[i];
                    int expandPixels = (child->grow / totalGrow) * deltaPx;
                    desiredBoundsPx[i].first += expandPixels;
                }
            }
        }
};
