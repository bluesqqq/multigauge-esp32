#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "graphics/Graphics.h"
#include "graphics/contexts/GraphicsContextLovyanGFX.h"

#include "graphics/colors/ColorTimeline.h"
#include "graphics/colors/StaticColor.h"
#include "graphics/gauge/GaugeFace.h"
#include "graphics/gauge/elements/primitives/CircleElement.h"
#include "graphics/gauge/elements/primitives/RectangleElement.h"
#include "graphics/gauge/elements/primitives/TextElement.h"

#include <xtensa/hal.h>
#include "utils.h"
#include <random>
#include <functional>

#ifndef PIO_UNIT_TESTING

GraphicsContext* context = new GraphicsContextLovyanGFX();

Graphics g(context);

static std::unique_ptr<GaugeFace> face;

void setup() { 
    Serial.begin(115200);

    context->init();

    YGConfigRef config = YGConfigNew();
    YGConfigSetUseWebDefaults(config, false);

    face = std::make_unique<GaugeFace>(config);

    YGNodeStyleSetFlexDirection(face->getNode(), YGFlexDirectionRow);
    

    auto circle = std::make_unique<CircleElement>(config);
    auto textElement = std::make_unique<TextElement>(config, "TESTING OF THE TEXT ELEMENT. SCALING TEXT AND WRAP.", 15.0f, new StaticColor(0xFFFF));
    
    auto styleEqual = [](YGNodeRef n) {
        YGNodeStyleSetFlexGrow(n, 1.0f);
        YGNodeStyleSetFlexShrink(n, 1.0f);
        YGNodeStyleSetFlexBasis(n, 0.0f);
        YGNodeStyleSetAlignSelf(n, YGAlignStretch);
    };

    styleEqual(circle->getNode());
    styleEqual(textElement->getNode());

    face->addChild(std::move(circle));
    face->addChild(std::move(textElement));
}

int t = 0;

uint32_t lastTime = 0;

void draw() {
    auto screen = g.getScreenBounds().toFloat();

    context->beginFrame();

    g.fillAll(TFT_BLACK);

    face->calculateLayout(screen.width, screen.height);
    face->update(1);
    face->draw(g);

    context->endFrame();
}

void loop() {
    uint32_t currentTime = micros();
    uint32_t lastFrameTime = currentTime - lastTime;          // microseconds
    float lastFPS = 1000000.0f / (float)lastFrameTime;        // FPS from us

    if (t % 60 == 0) Serial.println(lastFPS);
    
    t++;

    draw();

    lastTime = currentTime;

    engineRPM.setValueBase(sin((float)currentTime / 10000000.0f) * 1000 + 2000);
}

#endif
