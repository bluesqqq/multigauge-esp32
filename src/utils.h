#pragma once

#include <cstdint>     // uint8_t, uint16_t, uint32_t, int32_t
#include <cstddef>     // size_t
#include <string>      // std::string
#include <algorithm>   // std::min

// ----------[ FLOAT / MATH ]---------- //

size_t fastFloatToString(float value, uint8_t decimalPlaces, char* buf, size_t bufSize);

std::string floatToString(float value, uint8_t decimalPlaces);

float mapf(float x, float inMin, float inMax, float outMin, float outMax);

template <typename T>
static inline T lerp(T a, T b, float t) {
    return a + (b - a) * t;
}

// ----------[ BINARY READERS ]---------- //

bool read_u16(const uint8_t* data, size_t size, size_t off, uint16_t& out);

bool read_u32(const uint8_t* data, size_t size, size_t off, uint32_t& out);

bool read_i32(const uint8_t* data, size_t size, size_t off, int32_t& out);

// ----------[ BIT HELPERS ]---------- //
// Header-only helpers MUST be inline (or static) to avoid multiple definition errors.

static inline int count_trailing_zeros_u32(uint32_t v) {
    if (v == 0) return 32;
    int c = 0;
    while ((v & 1u) == 0u) { v >>= 1u; ++c; }
    return c;
}

static inline int count_bits_u32(uint32_t v) {
    int c = 0;
    while (v) { v &= (v - 1u); ++c; }
    return c;
}

static inline uint8_t extract_and_scale(uint32_t value, uint32_t mask) {
    if (mask == 0) return 0;

    const int shift = count_trailing_zeros_u32(mask);
    const int bits  = count_bits_u32(mask);

    const uint32_t raw = (value & mask) >> shift;
    if (bits <= 0) return 0;

    const uint32_t maxv = (bits >= 32) ? 0xFFFFFFFFu : ((1u << bits) - 1u);
    if (maxv == 0) return 0;

    const uint32_t scaled = (raw * 255u + (maxv / 2u)) / maxv;
    return (uint8_t)std::min<uint32_t>(scaled, 255u);
}

// ----------[ COLOR ]---------- //

uint16_t rgb888_to_565(uint8_t r, uint8_t g, uint8_t b);
