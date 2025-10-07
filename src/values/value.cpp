#include "value.h"
#include <Arduino.h>

namespace Values {

    Value::Value(const char* name, const UnitType& unitType, float minimumValue, float maximumValue) 
        : name(name), unitType(unitType), value(minimumValue), minimumValue(minimumValue), maximumValue(maximumValue) { 
        registry[name] = this;
    }

    Value *Value::find(std::string name) {
        auto it = registry.find(name);
        return (it != registry.end()) ? it->second : nullptr;
    }

    Value::operator float() const { return getValueBase(); }

    Value &Value::operator=(float newValue) {
        setValueBase(newValue);
        return *this;
    }

    float Value::getValueBase() const { return value; }

    void Value::setValueBase(float newValue) { 
        value = std::clamp(newValue, minimumValue, maximumValue);
        if (onChange) onChange(value);
    }

    float Value::getMinimumBase() const { return minimumValue; }

    float Value::getMaximumBase() const { return maximumValue; }

    float Value::getValue(int index) const { return unitType.convertFromBase(value, index); }

    void Value::setValue(float newValue, int index) {
        value = std::clamp(unitType.convertToBase(newValue, index), minimumValue, maximumValue);
        if (onChange) onChange(value);
    }

    float Value::getMinimum(int index) const { return unitType.convertToBase(minimumValue, index); }

    float Value::getMaximum(int index) const { return unitType.convertToBase(maximumValue, index); }

    const char* Value::getName() const { return name; }

    const UnitType& Value::getUnitType() const { return unitType; }

    float Value::getInterpolationValue() const { return (value - minimumValue) / (maximumValue - minimumValue); }

    std::string Value::getValueString(int index, bool abbreviation) const { return unitType.getValueString(getValue(index), index, abbreviation); }

    std::string Value::getLongestValueString(int index, bool abbreviation) {
        std::string minimumString = unitType.getValueString(getMinimum(index), index, abbreviation);
        std::string maximumString = unitType.getValueString(getMaximum(index), index, abbreviation);
        return (maximumString.length() > minimumString.length()) ? maximumString : minimumString;
    }

    Value engineRPM("RPM", revolutions, 0, 8000);
    Value engineCoolantTemp("Coolant Temp", temperature, -40, 120);
    Value engineOilTemp("Oil Temp", temperature, -40, 120);
    Value transmissionTemp("Transmission Temp", temperature, -40, 120);
    Value engineOilPressure("Oil Pressure", pressure, 0, 100);
    Value transmissionFluidPressure("Transmission Fluid Pressure", pressure, 0, 100);
    Value fuelPressure("Fuel Pressure", pressure, 0, 100);
    Value boostPressure("Boost Pressure", pressure, 0, 100);
    Value fuelLevel("Fuel Level", volume, 0, 12);
    Value distanceDriven("Distance Driven", distance, 0, 999999);
    Value speed("Speed", velocity, 0, 160);
    Value verticalAcceleration("Vertical Accel", acceleration, -4, 4);
    Value longitudinalAcceleration("Longitudinal Accel", acceleration, -4, 4);
    Value lateralAcceleration("Lateral Accel", acceleration, -4, 4);
    Value calculatedEngineLoad("Engine Load", percentage, 0.0, 100.0);
    Value throttlePosition("Throttle Position", percentage, 0.0, 100.0);
    Value engineFuelRate("Engine Fuel Rate", volumePerTime, 0, 3212.75);
    Value engineRunTimeSinceStart("Run Time Since Start", time, 0.0, 65535.0); // 65535 is max defined by OBD2 protocol

}