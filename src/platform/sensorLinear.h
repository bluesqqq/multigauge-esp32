#pragma once

#include "sensor.h"

class SensorLinear : public Sensor {
    private:
        Value* value;

        float lowerVoltage;
        float upperVoltage;

        float lowerValue;
        float upperValue;

        uint8_t unitIndex;

        uint pin;
    
    public:
        SensorLinear(uint pin, Value* value, float lowerVoltage, float upperVoltage, float lowerValue, float upperValue, int unitIndex = 0, std::string name = "Sensor");

        bool begin() override;

        void readSensor() override;
};