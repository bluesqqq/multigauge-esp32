#pragma once

#include <string>

size_t fastFloatToString(float value, uint8_t decimalPlaces, char* buf, size_t bufSize);

std::string floatToString(float value, uint8_t decimalPlaces);

float mapf(float x, float inMin, float inMax, float outMin, float outMax);