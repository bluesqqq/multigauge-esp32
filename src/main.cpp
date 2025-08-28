#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "io/sensor.h"
#include "values/units.h"
#include "values/value.h"
#include "utils.h"

SensorLinear sensor(1, calculatedEngineLoad, 0, 3.3, 0, 100);

#ifndef PIO_UNIT_TESTING

void setup() { 
    Serial.begin(115200);
    sensor.begin();

    delay(2000);
    const std::vector<Unit>& units = calculatedEngineLoad.getUnitType().getUnits();
    Serial.println(("Available units for " + calculatedEngineLoad.getName() + ":").c_str());
    for (auto& unit : units) {
        Serial.println(("  " + unit.name + " (" + unit.abbreviation + ")").c_str());
    }
    delay(2000);
}

void loop() { 
    delay(1000);
}

#endif