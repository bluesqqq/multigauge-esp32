#include <multigauge/App.h>

#include "platform/GraphicsContextLovyanGFX.h"
#include "platform/LittleFsFileSystem.h"
#include "platform/SerialLogger.h"
#include "platform/TimeArduino.h"

#include <Arduino.h>
#include <rapidjson/document.h>

namespace mg {
    SerialLogger logger;
    LittleFsFileSystem fileSystem;
    TimeArduino clock;
    GraphicsContextLovyanGFX context;
}

Platform plat{ mg::context, mg::fileSystem, mg::clock, &mg::logger };

void setup() { 
    setPlatform(plat);
    if (!initPlatform()) {
        // blank for now
    }

    mg::init("/gauges/gauge.json");
}

void loop() {
    mg::frame();
}