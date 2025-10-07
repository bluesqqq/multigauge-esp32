#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "io/sensorLinear.h"
#include "values/units.h"
#include "values/value.h"
#include "utils.h"

#ifndef PIO_UNIT_TESTING

SensorLinear sensor1(1, &boostPressure, 0, 3.3, 0, 100);
SensorLinear sensor2(1, &engineCoolantTemp, 0, 3.3, 0, 100);
SensorLinear sensor3(1, &engineRPM, 0, 3.3, 0, 8000);

void sensorTask(void* parameter) {
    SensorLinear* sensor = (SensorLinear*)parameter;

    const TickType_t delayTicks = pdMS_TO_TICKS(50);
    while (true) {
        sensor->readSensor();
        vTaskDelay(delayTicks);
    }
}

void setup() { 
    Serial.begin(115200);
    sensor1.begin();
    sensor2.begin();
    sensor3.begin();

    boostPressure.onChange = [](float newValue) {
        Serial.print("Boost Pressure changed: ");
        Serial.println(newValue);
    };

    xTaskCreate(sensorTask, "BoostSensor", 2048, &sensor1, 1, NULL);
}

void loop() { }

#endif