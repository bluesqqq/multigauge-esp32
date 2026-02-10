#include "Element.h"

#include "layout.h"

void Element::clearLayoutDirtyRecursive() {
    layoutDirty = false;
    for (auto& child : children) child->clearLayoutDirtyRecursive();
}

YGConfigRef Element::createConfig() {
    YGConfigRef config = YGConfigNew();
    YGConfigSetUseWebDefaults(config, false);
    return config;
}

void Element::markLayoutDirty() {
    Element* n = this;
    while (n->parent) n = n->parent;
    n->layoutDirty = true;
}

Element::Element(Element* p, bool inherit) : parent(p), inherited(inherit) {
    config = parent ? parent->getConfig() : createConfig();

    if (!parent) inherited = false; // root must own layout

    if (!inherited) {
        node = YGNodeNewWithConfig(config);
        YGNodeSetContext(node, this);
    }

    refreshInheritanceCacheRecursive();
}

Element::Element(Element* p, const rapidjson::Value::ConstObject json) : Element(p, styleIsInherit(json)) {
    if (!inherited) loadLayout(getNode(), json);

    if (!json.HasMember("children") || !json["children"].IsArray()) return;
    for (const auto& child : json["children"].GetArray())
        addChild(child.GetObject());
}

Element::~Element() {
    children.clear();
    
    if (node) {
        YGNodeFree(node);
        node = nullptr;
    }
}

#include "elements/primitives/TextElement.h"
#include "elements/primitives/RectangleElement.h"
#include "elements/primitives/CircleElement.h"
#include "elements/Horizon.h"
#include "elements/primitives/ImageElement.h"
#include "elements/Graph.h"

std::unique_ptr<Element> Element::fromJson(Element* parent, const rapidjson::Value::ConstObject json) {
    if (!json.HasMember("type") || !json["type"].IsString()) return std::make_unique<Element>(parent, json);

    const char* type = json["type"].GetString();

    if (strcmp(type, "text") == 0)      return std::make_unique<TextElement>(parent, json);
    if (strcmp(type, "rectangle") == 0) return std::make_unique<RectangleElement>(parent, json);
    if (strcmp(type, "circle") == 0)    return std::make_unique<CircleElement>(parent, json);
    if (strcmp(type, "image") == 0)     return std::make_unique<ImageElement>(parent, json);
    if (strcmp(type, "horizon") == 0)   return std::make_unique<Horizon>(parent, json);
    if (strcmp(type, "graph") == 0)     return std::make_unique<Graph>(parent, json);

    return std::make_unique<Element>(parent, json);
}

void Element::addChild(const rapidjson::Value::ConstObject json) {
    std::unique_ptr<Element> child = fromJson(this, json);
    if (!child) return;

    Element* yogaParent = this->getLayoutOwner();
    
    if (child->ownsLayout()) {
        if (yogaParent && yogaParent->node) {
            const uint32_t index = (uint32_t)YGNodeGetChildCount(yogaParent->node);
            YGNodeInsertChild(yogaParent->node, child->node, index);
        }
    }
    
    children.push_back(std::move(child));
    refreshInheritanceCacheRecursive();
    markLayoutDirty();
}

bool Element::removeChild(Element *child) {
    if (!child) return false;

    for (size_t i = 0; i < children.size(); ++i) {
        if (children[i].get() == child) {
            if (child->ownsLayout()) {
                Element* yogaParent = this->getLayoutOwner();
                if (yogaParent && yogaParent->node && child->node) {
                    YGNodeRemoveChild(yogaParent->node, child->node);
                }
            }

            child->parent = nullptr;
            children.erase(children.begin() + i);

            refreshInheritanceCacheRecursive();
            markLayoutDirty();
            return true;
        }
    }
    return false;
}

bool Element::initRecursive(AssetManager &assetManager) {
    bool success = init(assetManager);
    for (auto const& c : children) if (!c->initRecursive(assetManager)) success = false;
    return success;
}

void Element::drawRecursive(Graphics &g) const {
    draw(g);
    for (auto const& c : children) c->drawRecursive(g);
}

void Element::updateRecursive(int deltaTime) {
    update(deltaTime);
    for (auto& c : children) c->updateRecursive(deltaTime);
}

void Element::layoutRecursive(float width, float height, YGDirection direction) {
    if (!layoutDirty) return;

    Element* root = this;
    while (root->parent) root = root->parent;

    Element* layoutRoot = root->getLayoutOwner(); // should always be root
    if (!layoutRoot->node) return;

    YGNodeCalculateLayout(layoutRoot->node, width, height, direction);
   
    auto walk = [&](auto&& self, Element* e, float parentAbsX, float parentAbsY) -> void {
        if (e->ownsLayout()) {
            const float left   = YGNodeLayoutGetLeft(e->node);
            const float top    = YGNodeLayoutGetTop(e->node);
            const float w      = YGNodeLayoutGetWidth(e->node);
            const float h      = YGNodeLayoutGetHeight(e->node);

            const float absX = parentAbsX + left;
            const float absY = parentAbsY + top;

            e->bounds = Rect<float>(absX, absY, w, h);

            for (auto& c : e->children) self(self, c.get(), absX, absY);
        } else {
            for (auto& c : e->children) self(self, c.get(), parentAbsX, parentAbsY);
        }
    };

    walk(walk, root, 0, 0);

    root->clearLayoutDirtyRecursive();
}