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

#include "io/time/TimeArduino.h"

#include "AssetManager.h"

#ifndef PIO_UNIT_TESTING

SerialLogger logger = SerialLogger();

LittleFsFileSystem fileSystem = LittleFsFileSystem();

namespace mg {
    TimeArduino clock;
}

GraphicsContextLovyanGFX context = GraphicsContextLovyanGFX();
Graphics g(&context);

AssetManager assetManager = AssetManager(fileSystem, context, logger);

static std::unique_ptr<GaugeFace> face;

void setup() { 
    logger.init();

    if (!fileSystem.init()) LOG_ERROR(logger, "file", "FileSystem failed to mount.");
    else LOG_INFO(logger, "file", "FileSystem successfully mounted.");

    std::vector<std::string> dirs;
    fileSystem.listDirectories("/", dirs);

    context.init();

    YGConfigRef config = YGConfigNew();
    YGConfigSetUseWebDefaults(config, false);

    rapidjson::Document doc;
    if (assetManager.loadJson("/gauge.json", doc)) {
        face = std::make_unique<GaugeFace>(config, doc);
        LOG_INFO(logger, "gauge", "Successfully loaded test gaugeface file.");
    } else {
        face = std::make_unique<GaugeFace>(config);
        LOG_INFO(logger, "gauge", "Failed to load test gaugeface file.");
    }

    face->init(assetManager);
}

int t = 0;

uint32_t lastTime = 0;

void draw(uint32_t deltaUs) {
    auto screen = g.getScreenBounds().toFloat();

    g.clearColorCache();
    context.beginFrame();

    face->calculateLayout(screen.width, screen.height);
    face->update(deltaUs);
    face->draw(g);

    context.endFrame();
}

void loop() {
uint32_t currentTime = mg::clock.getMicros();
    uint32_t deltaUs = currentTime - lastTime;

    if (t % 60 == 0) {
        float fps = 1000000.0f / (float)deltaUs;
        LOG_INFO(logger, "perf", "fps=%.2f dt_us=%u", fps, (unsigned)deltaUs);
    }

    t++;

    draw(deltaUs);

    lastTime = currentTime;

    engineRPM.setValueBase(sin((float)currentTime / 1000000.0f) * 3000 + 4000);

}

#endif
