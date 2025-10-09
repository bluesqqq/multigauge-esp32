#include <Arduino.h>
#include <unity.h>
#include "values/value.h"

Value testValue("Test", temperature, 0.0f, 100.0f);

void test_min_max() {
    // Minimum
    TEST_ASSERT_EQUAL_FLOAT(0.0f, testValue.getMinimumBase());
    TEST_ASSERT_EQUAL_FLOAT(0.0f, testValue.getMinimum(0));
    TEST_ASSERT_EQUAL_FLOAT(32.0f, testValue.getMinimum(1));
    TEST_ASSERT_EQUAL_FLOAT(273.15f, testValue.getMinimum(2));

    // Maximum
    TEST_ASSERT_EQUAL_FLOAT(100.0f, testValue.getMaximumBase());
    TEST_ASSERT_EQUAL_FLOAT(100.0f, testValue.getMaximum(0));
    TEST_ASSERT_EQUAL_FLOAT(212.0f, testValue.getMaximum(1));
    TEST_ASSERT_EQUAL_FLOAT(373.15f, testValue.getMaximum(2));
}

void test_interpolation() {
    testValue.setValue(0.0f);

    // Interpolation
    TEST_ASSERT_EQUAL_FLOAT(0.0f, testValue.getInterpolationValue());
    testValue.setValueBase(50.0f);
    TEST_ASSERT_EQUAL_FLOAT(0.5f, testValue.getInterpolationValue());
    testValue.setValueBase(100.0f);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, testValue.getInterpolationValue());
    testValue.setValueBase(0.0f);
}

void test_value() {
    // Value
    TEST_ASSERT_EQUAL_FLOAT(0.0f, testValue.getValueBase());
    TEST_ASSERT_EQUAL_FLOAT(0.0f, testValue.getValue(0));
    TEST_ASSERT_EQUAL_FLOAT(32.0f, testValue.getValue(1));
    TEST_ASSERT_EQUAL_FLOAT(273.15f, testValue.getValue(2));

    testValue.setValue(-10.0f); // Below minimum
    TEST_ASSERT_EQUAL_FLOAT(0.0f, testValue.getValueBase());

    testValue.setValue(110.0f); // Above maximum
    TEST_ASSERT_EQUAL_FLOAT(100.0f, testValue.getValueBase());

    testValue.setValue(50.0f);
    TEST_ASSERT_EQUAL_FLOAT(50.0f, testValue.getValueBase());
    TEST_ASSERT_EQUAL_FLOAT(50.0f, testValue.getValue(0));
    TEST_ASSERT_EQUAL_FLOAT(122.0f, testValue.getValue(1));
    TEST_ASSERT_EQUAL_FLOAT(323.15f, testValue.getValue(2));
}

void setUp() {}
void tearDown() {}

int runUnityTests(void) {
    UNITY_BEGIN();
    RUN_TEST(test_min_max);
    RUN_TEST(test_value);
    RUN_TEST(test_interpolation);
    return UNITY_END();
}

void setup() {
    delay(2000);

    runUnityTests();
}

void loop() {}
