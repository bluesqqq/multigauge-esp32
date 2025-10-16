#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "graphics/Graphics.h"
#include "graphics/contexts/GraphicsContextLovyanGFX.h"

#include "graphics/colors/ColorTimeline.h"
#include "graphics/colors/StaticColor.h"
#include "graphics/gauge/GaugeFace.h"
#include "graphics/gauge/elements/utilities/GroupElement.h"

#include <xtensa/hal.h>
#include "utils.h"
#include <random>
#include <functional>

#ifndef PIO_UNIT_TESTING

GraphicsContext* context = new GraphicsContextLovyanGFX();

Graphics g(context);

GaugeFace face = GaugeFace();

void setup() { 
    Serial.begin(115200);

    std::unique_ptr<GroupElement> group = std::make_unique<GroupElement>();

    group->addChild(std::make_unique<GroupElement>());
    group->addChild(std::make_unique<GroupElement>());
    group->addChild(std::make_unique<GroupElement>());
    group->addChild(std::make_unique<GroupElement>());
    group->addChild(std::make_unique<GroupElement>());

    face.addChild(std::move(group));
    
    face.init();
    
    context->init();
}

int t = 0;

uint32_t lastTime = 0;

void loop() {
    uint32_t currentTime = millis();
    uint32_t lastFrameTime = currentTime - lastTime;
    float lastFPS = 1000.0f / (float)lastFrameTime;

    if (t % 60 == 0) {
        Serial.println(lastFPS);
    }
    
    t++;

    face.layout(g.getScreenBounds());

    face.update();

    context->beginFrame();

    face.draw(g);

    context->endFrame();

    lastTime = currentTime;
}

#endif