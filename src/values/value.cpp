#include "value.h"
#include <Arduino.h>

std::unordered_map<std::string, Value*> Value::registry;

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

Value engineRPM("RPM", revolutions, 0.0f, 8000.0f);
Value engineCoolantTemp("Coolant Temp", temperature, -40.0f, 120.0f);
Value engineOilTemp("Oil Temp", temperature, -40.0f, 120.0f);
Value transmissionTemp("Transmission Temp", temperature, -40.0f, 120.0f);
Value engineOilPressure("Oil Pressure", pressure, 0.0f, 100.0f);
Value transmissionFluidPressure("Transmission Fluid Pressure", pressure, 0.0f, 100.0f);
Value fuelPressure("Fuel Pressure", pressure, 0.0f, 100.0f);
Value boostPressure("Boost Pressure", pressure, 0.0f, 100.0f);
Value fuelLevel("Fuel Level", volume, 0.0f, 12.0f);
Value distanceDriven("Distance Driven", distance, 0.0f, 999999.0f);
Value speed("Speed", velocity, 0.0f, 160.0f);
Value verticalAcceleration("Vertical Accel", acceleration, -4.0f, 4.0f);
Value longitudinalAcceleration("Longitudinal Accel", acceleration, -4.0f, 4.0f);
Value lateralAcceleration("Lateral Accel", acceleration, -4.0f, 4.0f);
Value calculatedEngineLoad("Engine Load", percentage, 0.0f, 100.0f);
Value throttlePosition("Throttle Position", percentage, 0.0f, 100.0f);
Value engineFuelRate("Engine Fuel Rate", volumePerTime, 0.0f, 3212.75f);
//Value engineRunTimeSinceStart("Run Time Since Start", time, 0.0f, 65535.0f); // 65535 is max defined by OBD2 protocol