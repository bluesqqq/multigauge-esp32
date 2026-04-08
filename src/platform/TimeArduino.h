#pragma once

#include <multigauge/io/Time.h>
#include <Arduino.h>

class TimeArduino : public Time {
    public:
        uint64_t getMillis() override { return millis(); }
        uint64_t getMicros() override { return micros(); }
};