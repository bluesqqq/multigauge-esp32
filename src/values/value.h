#pragma once

#include <string>
#include "units.h"

class Value {
    private:
        std::string name;
        float value;
        float minimumValue;
        float maximumValue;

        UnitType& unitType;

    public:
        Value(const std::string& name, UnitType& unitType, float minimumValue, float maximumValue);

        float getValueRaw() const;
        void setValueRaw(float value);
        float getMinimumRaw() const;
        float getMaximumRaw() const;


        float getValue(int index) const;
        void setValue(float value, int index);
        float getMinimum(int index) const;
        float getMaximum(int index) const;

        std::string getName() const;
        const UnitType& getUnitType() const;

        float getInterpolationValue() const;

        std::string getValueString(int index = DEFAULT_UNIT, bool abbreviation = true) const;
};

extern Value engineRPM;
extern Value engineCoolantTemp;
extern Value calculatedEngineLoad;
extern Value throttlePosition;
extern Value engineRunTimeSinceStart;
extern Value engineFuelRate;
extern Value engineOilTemp;
extern Value transmissionTemp;
extern Value engineOilPressure;
extern Value transmissionFluidPressure;
extern Value fuelPressure;
extern Value boostPressure;
extern Value fuelLevel;
extern Value distanceDriven;
extern Value speed;
extern Value verticalAcceleration;
extern Value longitudinalAcceleration;
extern Value lateralAcceleration;