#pragma once

#include <stdint.h>
#include <algorithm>
#include <cmath>
#include "rapidjson/document.h"

struct rgba {
    uint8_t r, g, b, a;

    rgba() : r(0), g(0), b(0), a(255) { }

    rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r(r), g(g), b(b), a(a) {}

    rgba(const char* s) {
        *this = parse_hex(s);
    }

    static rgba parse_hex(const char* s) {
        // Trim leading/trailing whitespace
        while (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r') ++s;

        const char* end = s + std::strlen(s);
        while (end > s && (end[-1] == ' ' || end[-1] == '\t' || end[-1] == '\n' || end[-1] == '\r')) --end;

        // Make a small trimmed view without allocating
        auto len = static_cast<size_t>(end - s);
        if (len == 0) return rgba(255, 0, 255, 255);

        // Optional prefixes: # or 0x / 0X
        if (s[0] == '#') { ++s; --len; }
        else if (len >= 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) { s += 2; len -= 2; }

        auto hexval = [](char c) -> int {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
        if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
        return -1;
        };

        auto byte2 = [&](char hi, char lo) -> int {
        int a = hexval(hi), b = hexval(lo);
        if (a < 0 || b < 0) return -1;
        return (a << 4) | b;
        };

        auto expand4 = [&](char c) -> int {
        int v = hexval(c);
        if (v < 0) return -1;
        return (v << 4) | v; // e.g. 'A' -> 0xAA
        };

        rgba out(0, 0, 0, 255); // default on parse failure

        if (len == 3 || len == 4) {
        int R = expand4(s[0]);
        int G = expand4(s[1]);
        int B = expand4(s[2]);
        if (R < 0 || G < 0 || B < 0) return out;
        out.r = (uint8_t)R; out.g = (uint8_t)G; out.b = (uint8_t)B;
        if (len == 4) {
            int A = expand4(s[3]);
            if (A < 0) return out;
            out.a = (uint8_t)A;
        } else {
            out.a = 255;
        }
        return out;
        }

        if (len == 6 || len == 8) {
        int R = byte2(s[0], s[1]);
        int G = byte2(s[2], s[3]);
        int B = byte2(s[4], s[5]);
        if (R < 0 || G < 0 || B < 0) return out;
        out.r = (uint8_t)R; out.g = (uint8_t)G; out.b = (uint8_t)B;
        if (len == 8) {
            int A = byte2(s[6], s[7]);
            if (A < 0) return out;
            out.a = (uint8_t)A;
        } else {
            out.a = 255;
        }
        return out;
        }

        return out; // unsupported length
    }

    void blend(const rgba& other, float t) {
        t = std::clamp(t, 0.0f, 1.0f);

        auto lerp_u8 = [&](uint8_t x, uint8_t y) -> uint8_t {
            float v = (1.0f - t) * float(x) + t * float(y);
            v = std::clamp(v, 0.0f, 255.0f);
            return (uint8_t)std::lround(v);
        };

        r = lerp_u8(r, other.r);
        g = lerp_u8(g, other.g);
        b = lerp_u8(b, other.b);
        a = lerp_u8(a, other.a);
    }

    rgba blended(const rgba& other, float t) const {
        t = std::clamp(t, 0.0f, 1.0f);

        auto lerp_u8 = [&](uint8_t x, uint8_t y) -> uint8_t {
            float v = (1.0f - t) * float(x) + t * float(y);
            v = std::clamp(v, 0.0f, 255.0f);
            return (uint8_t)std::lround(v);
        };

        return rgba{
            lerp_u8(r, other.r),
            lerp_u8(g, other.g),
            lerp_u8(b, other.b),
            lerp_u8(a, other.a),
        };
    }
};

inline rgba rgb(uint8_t r, uint8_t g, uint8_t b) { return rgba(r, g, b); }