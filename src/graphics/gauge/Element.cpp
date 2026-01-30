#include "Element.h"

Element::Element(YGConfigRef config) : config(config) { initializeNode(config); }

Element::Element(YGConfigRef config, const rapidjson::Value::ConstObject json) : config(config) {
    initializeNode(config); 

    if (!json.HasMember("children") || !json["children"].IsArray()) return;
    for (const auto& child : json["children"].GetArray())
        addChild(child.GetObject());
}

Element::~Element() {
    if (parent && parent->node) YGNodeRemoveChild(parent->node, node);

    YGNodeRemoveAllChildren(node);
    YGNodeFree(node);
    node = nullptr;
}

#include "elements/primitives/TextElement.h"
#include "elements/primitives/RectangleElement.h"
#include "elements/primitives/CircleElement.h"
#include "elements/Horizon.h"

std::unique_ptr<Element> Element::fromJson(YGConfigRef config, const rapidjson::Value::ConstObject json) {
    if (!json.HasMember("type") || !json["type"].IsString()) return nullptr; // typeless elements are undefined behavior

    const char* type = json["type"].GetString();

    if (strcmp(type, "text") == 0) {
        return std::make_unique<TextElement>(config, json);
    }
    if (strcmp(type, "rectangle") == 0) {
        return std::make_unique<RectangleElement>(config, json);
    }
    if (strcmp(type, "circle") == 0) {
        return std::make_unique<CircleElement>(config, json);
    }
    if (strcmp(type, "horizon") == 0) {
        return std::make_unique<Horizon>(config, json);
    }   

}

void Element::addChild(std::unique_ptr<Element> child) {
    child->parent = this;

    const uint32_t index = (uint32_t)children.size();
    YGNodeInsertChild(this->node, child->node, index);

    children.push_back(std::move(child));
}

void Element::addChild(const rapidjson::Value::ConstObject json) {
    std::unique_ptr<Element> element = fromJson(config, json);
    if (element) addChild(std::move(element));
}

bool Element::init(AssetManager &assetManager) {
    for (auto const &c : children) {
        c->init(assetManager);
    }
    return true;
}

void Element::draw(Graphics &g) const { for (auto const& c : children) { c->draw(g); } }

void Element::update(int deltaTime) { for (auto& c : children) c->update(deltaTime); }
