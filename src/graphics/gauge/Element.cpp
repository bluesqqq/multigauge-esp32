#include "Element.h"

void Element::clearLayoutDirtyRecursive() {
    layoutDirty = false;
    for (auto& child : children) child->clearLayoutDirtyRecursive();
}

void Element::initializeNode(YGConfigRef config) {
    node = YGNodeNewWithConfig(config);
    YGNodeSetContext(node, this);
}

void Element::markLayoutDirty() {
    Element* n = this;
    while (n->parent) n = n->parent;
    n->layoutDirty = true;
}

Element::Element(YGConfigRef config) : config(config) { initializeNode(config); }

Element::Element(YGConfigRef config, const rapidjson::Value::ConstObject json) : config(config) {
    initializeNode(config);

    loadLayout(getNode(), json);

    if (!json.HasMember("children") || !json["children"].IsArray()) return;
    for (const auto& child : json["children"].GetArray())
        addChild(child.GetObject());
}

Element::~Element() {
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

std::unique_ptr<Element> Element::fromJson(YGConfigRef config, const rapidjson::Value::ConstObject json) {
    if (!json.HasMember("type") || !json["type"].IsString()) return std::make_unique<Element>(config, json);

    const char* type = json["type"].GetString();

    if (strcmp(type, "text") == 0)      return std::make_unique<TextElement>(config, json);
    if (strcmp(type, "rectangle") == 0) return std::make_unique<RectangleElement>(config, json);
    if (strcmp(type, "circle") == 0)    return std::make_unique<CircleElement>(config, json);
    if (strcmp(type, "image") == 0)     return std::make_unique<ImageElement>(config, json);
    if (strcmp(type, "horizon") == 0)   return std::make_unique<Horizon>(config, json);
    if (strcmp(type, "graph") == 0)     return std::make_unique<Graph>(config, json);

    return std::make_unique<Element>(config, json);
}

void Element::addChild(std::unique_ptr<Element> child) {
    child->parent = this;
    YGNodeInsertChild(this->node, child->node, (uint32_t)children.size());
    children.push_back(std::move(child));
    markLayoutDirty();
}

void Element::addChild(const rapidjson::Value::ConstObject json) {
    std::unique_ptr<Element> element = fromJson(config, json);
    if (element) addChild(std::move(element));
}

bool Element::removeChild(Element *child) {
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
    YGNodeCalculateLayout(node, width, height, direction);
   
    auto walk = [&](auto&& self, Element* e, float parentAbsX, float parentAbsY) -> void {
        const float left   = YGNodeLayoutGetLeft(e->node);
        const float top    = YGNodeLayoutGetTop(e->node);
        const float w      = YGNodeLayoutGetWidth(e->node);
        const float h      = YGNodeLayoutGetHeight(e->node);

        const float absX = parentAbsX + left;
        const float absY = parentAbsY + top;

        e->bounds = Rect<float>(absX, absY, w, h);

        for (auto& c : e->children) self(self, c.get(), absX, absY);
    };

    walk(walk, this, 0, 0);
    clearLayoutDirtyRecursive();
}