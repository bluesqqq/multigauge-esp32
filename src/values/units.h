#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>

struct Unit {
    std::string name;
    std::string abbreviation;
    float factor;
    float offset;
    int decimalPlaces;
};

#define DEFAULT_UNIT -1

class UnitType {
    private:
        std::vector<Unit> units;
        int defaultUnitIndex;

    public:
        UnitType(std::string baseName, std::string baseAbbreviation, int baseDecimalPlaces, const std::vector<Unit>& conversionUnits = {}, int defaultUnitIndex = 0);

        void setDefaultUnit(int index);

        float convert(float value, int fromIndex, int toIndex) const;

        float convertToBase(float value, int index) const;

        float convertFromBase(float value, int index) const;

        const Unit& getUnit(int index = DEFAULT_UNIT) const;

        const std::vector<Unit>& getUnits() const;

        const Unit& getBaseUnit() const;

        const Unit& getDefaultUnit() const;

        std::string getValueString(float value, int index = DEFAULT_UNIT, bool abbreviation = true) const;
};

extern UnitType temperatureUnitType;
extern UnitType distanceUnitType;
extern UnitType pressureUnitType;
extern UnitType velocityUnitType;
extern UnitType accelerationUnitType;
extern UnitType volumeUnitType;
extern UnitType volumePerTimeUnitType;
extern UnitType revolutionsUnitType;
extern UnitType angleUnitType;
extern UnitType percentageUnitType;
extern UnitType timeUnitType;