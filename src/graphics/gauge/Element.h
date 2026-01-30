#pragma once

#include "graphics/colors/Color.h"
#include "graphics/geometry/alignment.h"
#include "graphics/geometry/Rect.h"

#include "graphics/Graphics.h"
#include <yoga/Yoga.h>
#include "layout.h"

#include <rapidjson/document.h>

#include "AssetManager.h"

class Element {
    private:
        /// @brief Parent element in the hierarchy, or nullptr if root
        Element* parent = nullptr;
        /// @brief Owned child elements
        std::vector<std::unique_ptr<Element>> children;

        Rect<float> bounds = Rect<float>(0.0f, 0.0f, 0.0f, 0.0f);

        YGNodeRef node = nullptr;
        YGConfigRef config = nullptr;        

        bool needsLayout_ = true;

        void syncLayoutRecursive(float parentAbsX, float parentAbsY) {
            const float left = YGNodeLayoutGetLeft(node);
            const float top = YGNodeLayoutGetTop(node);
            const float width = YGNodeLayoutGetWidth(node);
            const float height = YGNodeLayoutGetHeight(node);

            const float absX = parentAbsX + left;
            const float absY = parentAbsY + top;

            bounds = Rect<float>(absX, absY, width, height);

            for (auto& child : children) {
                child->syncLayoutRecursive(absX, absY);
            }
        }

        void clearLayoutDirtyRecursive() {
            needsLayout_ = false;
            for (auto& child : children) child->clearLayoutDirtyRecursive();
        }

        void initializeNode(YGConfigRef config) {
            node = YGNodeNewWithConfig(config);

            YGNodeSetContext(node, this);
        }
        
    protected:
        void markLayoutDirty() {
            Element* n = this;
            while (n->parent) n = n->parent;
            n->needsLayout_ = true;
        }

        bool needsLayout() const { return needsLayout_; }

    public:
        explicit Element(YGConfigRef config);

        explicit Element(YGConfigRef config, const rapidjson::Value::ConstObject json);

        virtual ~Element();

        static std::unique_ptr<Element> fromJson(YGConfigRef config, const rapidjson::Value::ConstObject json);

        // Tree

        YGNodeRef getNode() const { return node; }

        void addChild(std::unique_ptr<Element> child);

        void addChild(const rapidjson::Value::ConstObject json);

        bool removeChild(Element* child) {
            if (!child) return false;

            for (size_t i = 0; i < children.size(); ++i) {
                if (children[i].get() == child) {
                    YGNodeRemoveChild(node, child->node);
                    child->parent = nullptr;
                    children.erase(children.begin() + i);
                    markLayoutDirty();
                    return true;
                }
            }
            return false;
        }

        // Layout + bounds

        void calculateLayout(float width, float height, YGDirection direction = YGDirectionLTR) {
            YGNodeCalculateLayout(node, width, height, direction);
            syncLayoutRecursive(0, 0);
            clearLayoutDirtyRecursive();
        }

        const Rect<float>& getBounds() const { return bounds; }

        // Update + draw

        virtual bool init(AssetManager& assetManager);

        virtual void draw(Graphics& g) const;

        virtual void update(int deltaTime);
};
