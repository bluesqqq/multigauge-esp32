#include "value.h"
#include <Arduino.h>

Value::Value(const std::string &name, UnitType& unitType, float minimumValue, float maximumValue) 
    : name(name), unitType(unitType), value(minimumValue), minimumValue(minimumValue), maximumValue(maximumValue) { }

float Value::getValueRaw() const { return value; }

void Value::setValueRaw(float newValue) { value = std::clamp(newValue, minimumValue, maximumValue); }

float Value::getMinimumRaw() const { return minimumValue; }

float Value::getMaximumRaw() const { return maximumValue; }

float Value::getValue(int index) const { return unitType.convertFromBase(value, index); }

void Value::setValue(float value, int index) { value = std::clamp(unitType.convertToBase(value, index), minimumValue, maximumValue); }

float Value::getMinimum(int index) const { return unitType.convertToBase(minimumValue, index); }

float Value::getMaximum(int index) const { return unitType.convertToBase(maximumValue, index); }

std::string Value::getName() const { return name; }

const UnitType& Value::getUnitType() const { return unitType; }

float Value::getInterpolationValue() const { return (value - minimumValue) / (maximumValue - minimumValue); }

std::string Value::getValueString(int index, bool abbreviation) const { return unitType.getValueString(getValue(index), index, abbreviation); }

Value engineRPM("RPM", revolutionsUnitType, 0, 8000);
Value engineCoolantTemp("Coolant Temp", temperatureUnitType, -40, 120);
Value engineOilTemp("Oil Temp", temperatureUnitType, -40, 120);
Value transmissionTemp("Transmission Temp", temperatureUnitType, -40, 120);
Value engineOilPressure("Oil Pressure", pressureUnitType, 0, 100);
Value transmissionFluidPressure("Transmission Fluid Pressure", pressureUnitType, 0, 100);
Value fuelPressure("Fuel Pressure", pressureUnitType, 0, 100);
Value boostPressure("Boost Pressure", pressureUnitType, 0, 100);
Value fuelLevel("Fuel Level", volumeUnitType, 0, 12);
Value distanceDriven("Distance Driven", distanceUnitType, 0, 999999);
Value speed("Speed", velocityUnitType, 0, 160);
Value verticalAcceleration("Vertical Accel", accelerationUnitType, -4, 4);
Value longitudinalAcceleration("Longitudinal Accel", accelerationUnitType, -4, 4);
Value lateralAcceleration("Lateral Accel", accelerationUnitType, -4, 4);
Value calculatedEngineLoad("Engine Load", percentageUnitType, 0.0, 100.0);
Value throttlePosition("Throttle Position", percentageUnitType, 0.0, 100.0);
Value engineFuelRate("Engine Fuel Rate", volumePerTimeUnitType, 0, 3212.75);
Value engineRunTimeSinceStart("Run Time Since Start", timeUnitType, 0.0, 65535.0); // 65535 is max defined by OBD2 protocol