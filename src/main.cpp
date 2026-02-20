#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "platform/GraphicsContextLovyanGFX.h"
#include <multigauge/graphics/gauge/GaugeFace.h>

#include <xtensa/hal.h>
#include <random>
#include <functional>

#include <rapidjson/document.h>

#include "platform/LittleFsFileSystem.h"

#include "platform/SerialLogger.h"

#include "platform/TimeArduino.h"

#include <multigauge/AssetManager.h>

#ifndef PIO_UNIT_TESTING

SerialLogger logger = SerialLogger();

LittleFsFileSystem fileSystem = LittleFsFileSystem();

namespace mg {
    TimeArduino clock;
}

GraphicsContextLovyanGFX context = GraphicsContextLovyanGFX();
Graphics g(&context);

AssetManager assetManager = AssetManager(fileSystem, context);

static std::unique_ptr<GaugeFace> face;

void setup() { 
    setLogger(logger);
    logger.init();

    if (!fileSystem.init()) LOG_ERROR("file", "FileSystem failed to mount.");
    else LOG_INFO("file", "FileSystem successfully mounted.");

    std::vector<std::string> dirs;
    fileSystem.listDirectories("/", dirs);

    context.init();

    rapidjson::Document doc;
    if (assetManager.loadJson("/gauges/gauge.json", doc)) {
        face = std::make_unique<GaugeFace>(doc);
        LOG_INFO("gauge", "Successfully loaded test gaugeface file.");
    } else {
        face = std::make_unique<GaugeFace>();
        LOG_INFO("gauge", "Failed to load test gaugeface file.");
    }

    face->initRecursive(assetManager);
}

int t = 0;

uint32_t lastTime = 0;

#include "graphics_test.h"

void draw(uint32_t deltaUs) {
    auto screen = g.getScreenBounds().toFloat();

    g.clearColorCache();
    context.beginFrame();

    //face->layoutRecursive(screen.width, screen.height);
    //face->updateRecursive(deltaUs);
    //face->drawRecursive(g);

    uint32_t nowUs = mg::clock.getMicros();
    drawGraphicsPrimitiveTest(g, context, assetManager, nowUs);

    context.endFrame();
}

void loop() {
uint32_t currentTime = mg::clock.getMicros();
    uint32_t deltaUs = currentTime - lastTime;

    if (t % 60 == 0) {
        float fps = 1000000.0f / (float)deltaUs;
        LOG_INFO("perf", "fps=%.2f dt_us=%u", fps, (unsigned)deltaUs);
    }

    t++;

    draw(deltaUs);

    lastTime = currentTime;

    engineRPM.setValueBase(sin((float)currentTime / 1000000.0f) * 3000 + 4000);
    engineCoolantTemp.setValueBase(sin((float)currentTime / 5000000.0f) * 10 + 0);

}

#endif
