#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "graphics/Graphics.h"
#include "graphics/colors/ColorTimeline.h"
#include "graphics/colors/StaticColor.h"

#include <xtensa/hal.h>
#include "utils.h"
#include <random>
#include <functional>

#ifndef PIO_UNIT_TESTING

//Graphics g;

void setup() { 
    Serial.begin(115200);
}

void loop() {

}

#endif