#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "graphics/Graphics.h"
#include "graphics/contexts/GraphicsContextLovyanGFX.h"

#include "graphics/colors/ColorTimeline.h"
#include "graphics/colors/StaticColor.h"

#include <xtensa/hal.h>
#include "utils.h"
#include <random>
#include <functional>

#ifndef PIO_UNIT_TESTING


GraphicsContext* context = new GraphicsContextLovyanGFX();

Graphics g(context);

void setup() { 
    Serial.begin(115200);

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
    context->beginFrame();

    g.fillAll(TFT_BLACK);

    g.setStroke(TFT_WHITE);
    g.setFill(TFT_WHITE);

    for (int i = 0; i < 24; i++) {
        int r = (i * 10) + (t % 10);
        g.strokeCircle(120, 120, r);
    }

    g.fillCircle(120, 120, 20);

    context->endFrame();

    lastTime = currentTime;
}

#endif