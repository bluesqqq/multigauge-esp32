#pragma once


#include <Arduino.h>
#include "values/units.h"
#include "values/value.h"

class SensorLinear {
    private:
        Value& value;

        float lowerVoltage;
        float upperVoltage;

        float lowerValue;
        float upperValue;

        int unitIndex;

        uint pin;

        std::string name;
    
    public:
        SensorLinear(uint pin, Value& value, float lowerVoltage, float upperVoltage, float lowerValue, float upperValue, int unitIndex = 0, std::string name = "Sensor");

        void begin();

        void update();
};