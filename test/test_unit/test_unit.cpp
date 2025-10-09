#include <Arduino.h>
#include <unity.h>
#include "values/units.h"

void test_unit_type() {
    UnitType testUnitType(
        "Base", "B", 2,
        {
            {"Second", "S", 2, 0, 2},
            {"Third", "T", 3, 0, 2}
        }
    );

    const std::vector<Unit>& units = testUnitType.getUnits();
    
    TEST_ASSERT_EQUAL(3, units.size());
    TEST_ASSERT_EQUAL_STRING("Base", units[0].name);
    TEST_ASSERT_EQUAL_STRING("Second", units[1].name);
    TEST_ASSERT_EQUAL_STRING("Third", units[2].name);

    TEST_ASSERT_EQUAL_STRING("Base", testUnitType.getBaseUnit().name);

    TEST_ASSERT_EQUAL_STRING(testUnitType.getUnit(-1).name, testUnitType.getDefaultUnit().name);
    TEST_ASSERT_EQUAL_STRING(testUnitType.getUnit(3).name, testUnitType.getDefaultUnit().name);

    TEST_ASSERT_EQUAL_STRING("Base", testUnitType.getDefaultUnit().name);

    testUnitType.setDefaultUnit(1);
    TEST_ASSERT_EQUAL_STRING("Second", testUnitType.getDefaultUnit().name);
    testUnitType.setDefaultUnit(2);
    TEST_ASSERT_EQUAL_STRING("Third", testUnitType.getDefaultUnit().name);
    testUnitType.setDefaultUnit(0);
    TEST_ASSERT_EQUAL_STRING("Base", testUnitType.getDefaultUnit().name);
    testUnitType.setDefaultUnit(-1);
    TEST_ASSERT_EQUAL_STRING("Base", testUnitType.getDefaultUnit().name);

    TEST_ASSERT_EQUAL_STRING("1.00B", testUnitType.getValueString(1.0f, 0).c_str());
    TEST_ASSERT_EQUAL_STRING("1.01B", testUnitType.getValueString(1.014f, 0).c_str());
    TEST_ASSERT_EQUAL_STRING("1.02B", testUnitType.getValueString(1.016f, 0).c_str());
    TEST_ASSERT_EQUAL_STRING("0.50B", testUnitType.getValueString(0.50f, 0).c_str());
    TEST_ASSERT_EQUAL_STRING("-1.00B", testUnitType.getValueString(-1.0f, 0).c_str());
}

void test_conversions() {
    UnitType testUnitType(
        "Base", "B", 2,
        {
            {"Second", "S", 2, 0, 2},
            {"Third", "T", 3, 0, 2}
        }
    );
    
    TEST_ASSERT_EQUAL_FLOAT(1, testUnitType.convertToBase(1, 0));
    TEST_ASSERT_EQUAL_FLOAT(1, testUnitType.convertToBase(2, 1));
    TEST_ASSERT_EQUAL_FLOAT(1, testUnitType.convertToBase(3, 2));

    TEST_ASSERT_EQUAL_FLOAT(1, testUnitType.convertFromBase(1, 0));
    TEST_ASSERT_EQUAL_FLOAT(2, testUnitType.convertFromBase(1, 1));
    TEST_ASSERT_EQUAL_FLOAT(3, testUnitType.convertFromBase(1, 2));

    TEST_ASSERT_EQUAL_FLOAT(2, testUnitType.convert(1, 0, 1));
    TEST_ASSERT_EQUAL_FLOAT(3, testUnitType.convert(1, 0, 2));

    TEST_ASSERT_EQUAL_FLOAT(212, temperature.convert(100, 0, 1)); // 100C to F
    TEST_ASSERT_EQUAL_FLOAT(373.15, temperature.convert(100, 0, 2)); // 100C to K
    TEST_ASSERT_EQUAL_FLOAT(32, temperature.convert(0, 0, 1)); // 0C to F
}


void setUp() {}
void tearDown() {}

int runUnityTests(void) {
    UNITY_BEGIN();
    RUN_TEST(test_unit_type);
    RUN_TEST(test_conversions);
    return UNITY_END();
}

void setup() {
    delay(2000);

    runUnityTests();
}

void loop() {}
