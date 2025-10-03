#pragma once

#include "values/value.h"

class Sensor {
    protected:
        std::string name;

    public:
        Sensor(const std::string& name = "Sensor") : name(name) {}

        virtual ~Sensor() = default;

        virtual bool begin() = 0;

        virtual void readSensor() = 0;

        const std::string& getName() const { return name; }
};