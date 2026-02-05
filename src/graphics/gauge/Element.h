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
        //----------[ TREE ]----------//

        /// @brief Parent element in the hierarchy, or nullptr if root
        Element* parent = nullptr;
        /// @brief Owned child elements
        std::vector<std::unique_ptr<Element>> children;

        //----------[ LAYOUT ]----------//

        /// @brief Absolute bounds computer from Yoga
        Rect<float> bounds = Rect<float>(0.0f, 0.0f, 0.0f, 0.0f);
        /// @brief Yoga node for this element
        YGNodeRef node = nullptr;
        /// @brief Shared Yoga config
        YGConfigRef config = nullptr;
        /// @brief True when the layout needs to be recalculated at the root
        bool layoutDirty = true;
        
        void clearLayoutDirtyRecursive();
        void initializeNode(YGConfigRef config);
        
    protected:
        void markLayoutDirty();
        bool needsLayout() const { return layoutDirty; }

        //----------[ HOOKS ]----------//

        virtual bool init(AssetManager& assetManager);
        virtual void draw(Graphics& g) const;
        virtual void update(int deltaTime);

    public:
        enum Type { Base, CircularGroup };

        explicit Element(YGConfigRef config);
        explicit Element(YGConfigRef config, const rapidjson::Value::ConstObject json);
        virtual ~Element();
        static std::unique_ptr<Element> fromJson(YGConfigRef config, const rapidjson::Value::ConstObject json);

        virtual Type getType() const { return Type::Base; }
        
        //----------[ TREE ]----------//

        Element* getParent() const { return parent; }
        YGNodeRef getNode() const { return node; }
        const Rect<float>& getBounds() const { return bounds; }

        void addChild(std::unique_ptr<Element> child);
        void addChild(const rapidjson::Value::ConstObject json);
        bool removeChild(Element* child);

        bool isRoot() const { return parent == nullptr; }

        //----------[ TRAVERSAL ]----------//

        bool initRecursive(AssetManager& assetManager);
        void drawRecursive(Graphics& g) const;
        void updateRecursive(int deltaTime);

        //----------[ LAYOUT ]----------//

        void layoutRecursive(float width, float height, YGDirection direction = YGDirectionLTR);
};
