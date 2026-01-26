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
#include "graphics/gauge/elements/ShiftLightElement.h"

#include <xtensa/hal.h>
#include "utils.h"
#include <random>
#include <functional>

#include "io/file/FileSystem.h"
#include "io/file/LittleFsFileSystem.h"

#include "io/logging/Logger.h"
#include "io/logging/SerialLogger.h"

#ifndef PIO_UNIT_TESTING

SerialLogger logger = SerialLogger();

FileSystem* fileSystem = new LittleFsFileSystem();

GraphicsContext* context = new GraphicsContextLovyanGFX();

Graphics g(context);
static std::unique_ptr<GaugeFace> face;

void setup() { 
    logger.init();

    if (!fileSystem->init()) LOG_ERROR(logger, "file", "FileSystem failed to mount.");
    else LOG_INFO(logger, "file", "FileSystem successfully mounted.");

    context->init();

    YGConfigRef config = YGConfigNew();
    YGConfigSetUseWebDefaults(config, false);

    face = std::make_unique<GaugeFace>(config);

    // Root fills the screen and centers its only child
    YGNodeStyleSetWidthPercent(face->getNode(), 100);
    YGNodeStyleSetHeightPercent(face->getNode(), 100);
    YGNodeStyleSetFlexDirection(face->getNode(), YGFlexDirectionRow);
    YGNodeStyleSetJustifyContent(face->getNode(), YGJustifyCenter);
    YGNodeStyleSetAlignItems(face->getNode(), YGAlignCenter);

    // One element
    auto graphElement = std::make_unique<ShiftLightElement>(
        config, engineRPM, new StaticColor(0xFFFF)
    );

    // Half width + half height, centered by parent settings above
    YGNodeStyleSetWidthPercent(graphElement->getNode(), 50);
    YGNodeStyleSetHeightPercent(graphElement->getNode(), 50);

    face->addChild(std::move(graphElement));
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

    if (t % 60 == 0) LOG_INFO(logger, "perf", "fps=%.2f dt_us=%u", lastFPS, (unsigned)lastFrameTime);
    
    t++;

    draw();

    lastTime = currentTime;

    engineRPM.setValueBase(sin((float)currentTime / 1000000.0f) * 3000 + 4000);
}

#endif
