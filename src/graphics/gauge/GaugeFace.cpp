#include "GaugeFace.h"
#include "graphics/colors/StaticColor.h"

GaugeFace::GaugeFace() : backgroundColor(std::make_unique<StaticColor>()), title(NO_TITLE_TEXT), description(NO_DESCRIPTION_TEXT), lastUpdateTime(micros()) {}

GaugeFace::GaugeFace(const char *title, const char *desc) : backgroundColor(std::make_unique<StaticColor>()), title(title), description(desc), lastUpdateTime(micros()) {}

GaugeFace::GaugeFace(JsonObject json) 
    : backgroundColor(json["backgroundColor"].is<JsonObject>() ? Color::loadFromJson(json["backgroundColor"].as<JsonObject>()) : std::make_unique<StaticColor>()),
      title(json["title"].is<const char*>() ? json["title"].as<const char*>() : NO_TITLE_TEXT),
      description(json["description"].is<const char*>() ? json["description"].as<const char*>() : NO_DESCRIPTION_TEXT) {
    
    if (json["elements"].is<JsonArray>()) {
        JsonArray elementsJson = json["elements"].as<JsonArray>();
        elements.reserve(elementsJson.size());

        for (const auto& elementJson : elementsJson)
            if (elementJson.is<JsonObject>()) addChild(elementJson.as<JsonObject>());
    }
}

bool GaugeFace::init() {
    bool result = true;

    for (const auto& element : elements) 
        if (!element->init()) result = false;
        
    return result;
}

void GaugeFace::draw(Graphics &g) const {
    g.fillAll(backgroundColor->getColor());
    
    // Currently just draws all elements relative
    for (const auto& element : elements) element->draw(g);
}

void GaugeFace::update() {
    unsigned long currentTime = micros();
    float deltaTime = (currentTime - lastUpdateTime) / 1e6f;
    lastUpdateTime = currentTime;

    for (const auto& element : elements) element->update(deltaTime);
}
