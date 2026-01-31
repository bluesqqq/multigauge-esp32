#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <algorithm>

#include "io/logging/Logger.h"
#include "utils.h"

// BMP compression types (from BMP spec)
constexpr uint32_t BI_RGB            = 0; // no compression
constexpr uint32_t BI_RLE8           = 1;
constexpr uint32_t BI_RLE4           = 2;
constexpr uint32_t BI_BITFIELDS      = 3;
constexpr uint32_t BI_JPEG           = 4;
constexpr uint32_t BI_PNG            = 5;
constexpr uint32_t BI_ALPHABITFIELDS = 6; // adds alpha mask

static inline bool canDecodeBMP(const uint8_t* data, size_t size) {
    if (!data || size < 2) return false;
    return data[0] == 'B' && data[1] == 'M';
}

struct ImageInfo {
    int width = 0;
    int height = 0;
    // RGB565 pixels, row-major, owns its memory
    std::vector<uint16_t> pixels;
};

static inline bool decodeBMP(const uint8_t* data, size_t size, ImageInfo& out, Logger& log) {
    out = ImageInfo{};

    if (!canDecodeBMP(data, size)) {
        LOG_ERROR(log, "BMP", "Data is not a BMP.");
        return false;
    }

    // BITMAPFILEHEADER
    uint32_t pixelOffset = 0;
    if (!read_u32(data, size, 10, pixelOffset)) {
        LOG_ERROR(log, "BMP", "Truncated BMP header.");
        return false;
    }
    if (pixelOffset >= size) {
        LOG_ERROR(log, "BMP", "Pixel offset out of range.");
        return false;
    }

    // DIB header size
    uint32_t dibSize = 0;
    if (!read_u32(data, size, 14, dibSize)) {
        LOG_ERROR(log, "BMP", "Missing DIB header.");
        return false;
    }
    if (14 + (size_t)dibSize > size) {
        LOG_ERROR(log, "BMP", "Truncated DIB header.");
        return false;
    }
    if (dibSize < 40) {
        LOG_ERROR(log, "BMP", "Unsupported DIB header size: %u", (unsigned)dibSize);
        return false;
    }

    int32_t width = 0, height = 0;
    uint16_t planes = 0, bpp = 0;
    uint32_t compression = 0, colorsUsed = 0;

    if (!read_i32(data, size, 18, width) ||
        !read_i32(data, size, 22, height) ||
        !read_u16(data, size, 26, planes) ||
        !read_u16(data, size, 28, bpp) ||
        !read_u32(data, size, 30, compression) ||
        !read_u32(data, size, 46, colorsUsed)) {
        LOG_ERROR(log, "BMP", "Truncated BITMAPINFOHEADER.");
        return false;
    }

    if (planes != 1) {
        LOG_ERROR(log, "BMP", "Invalid planes (expected 1, got %u)", (unsigned)planes);
        return false;
    }
    if (width <= 0 || height == 0) {
        LOG_ERROR(log, "BMP", "Invalid dimensions (W=%d, H=%d)", (int)width, (int)height);
        return false;
    }

    const bool topDown = (height < 0);
    const int32_t absH = topDown ? -height : height;

    // Reject RLE (you can add support later)
    if (compression == BI_RLE8 || compression == BI_RLE4) {
        LOG_ERROR(log, "BMP", "RLE compressed BMP not supported (compression=%u).", (unsigned)compression);
        return false;
    }

    // Allow only these compression modes
    if (!(compression == BI_RGB || compression == BI_BITFIELDS || compression == BI_ALPHABITFIELDS)) {
        LOG_ERROR(log, "BMP", "Unsupported BMP compression: %u", (unsigned)compression);
        return false;
    }

    // Masks (for 16/32bpp when BITFIELDS)
    uint32_t rMask = 0, gMask = 0, bMask = 0, aMask = 0;
    const size_t masksOffset = 14 + 40;

    if (compression == BI_BITFIELDS || compression == BI_ALPHABITFIELDS) {
        if (!read_u32(data, size, masksOffset + 0, rMask) ||
            !read_u32(data, size, masksOffset + 4, gMask) ||
            !read_u32(data, size, masksOffset + 8, bMask)) {
            LOG_ERROR(log, "BMP", "Missing BITFIELDS masks.");
            return false;
        }
        if (masksOffset + 16 <= size) {
            (void)read_u32(data, size, masksOffset + 12, aMask);
        }
    } else {
        // Default masks for common BI_RGB encodings
        if (bpp == 16) {
            // Many BI_RGB 16bpp files are 555; some are 565 but then they should use BITFIELDS.
            rMask = 0x7C00; gMask = 0x03E0; bMask = 0x001F;
        } else if (bpp == 32) {
            rMask = 0x00FF0000; gMask = 0x0000FF00; bMask = 0x000000FF; aMask = 0xFF000000;
        }
    }

    // Palette handling for 1/4/8 bpp
    std::vector<uint32_t> palette;
    if (bpp <= 8) {
        uint32_t paletteEntries = colorsUsed;
        if (paletteEntries == 0) paletteEntries = 1u << bpp;

        const size_t palOffset = 14 + (size_t)dibSize;
        const size_t palBytes = (size_t)paletteEntries * 4;

        if (palOffset + palBytes > size) {
            LOG_ERROR(log, "BMP", "Truncated palette.");
            return false;
        }

        palette.resize(paletteEntries);
        for (uint32_t i = 0; i < paletteEntries; ++i) {
            const size_t o = palOffset + (size_t)i * 4;
            const uint8_t b = data[o + 0];
            const uint8_t g = data[o + 1];
            const uint8_t r = data[o + 2];
            palette[i] = ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
        }
    }

    const int outW = (int)width;
    const int outH = (int)absH;

    out.width = outW;
    out.height = outH;
    out.pixels.assign((size_t)outW * (size_t)outH, 0);

    auto rowStrideBytes = [&](int w, uint16_t bitsPerPixel) -> size_t {
        const size_t bits = (size_t)w * (size_t)bitsPerPixel;
        const size_t bytes = (bits + 7) / 8;
        return (bytes + 3) & ~size_t(3);
    };

    const size_t inStride = rowStrideBytes(outW, bpp);
    const size_t pixelBase = (size_t)pixelOffset;

    if (pixelBase >= size) {
        LOG_ERROR(log, "BMP", "Pixel data starts out of bounds.");
        return false;
    }
    if (pixelBase + inStride * (size_t)outH > size) {
        // allow if file is slightly truncated? No, fail to be safe.
        LOG_ERROR(log, "BMP", "Pixel data truncated (need %u bytes).", (unsigned)(inStride * (size_t)outH));
        return false;
    }

    auto write565 = [&](int x, int y, uint16_t px) {
        out.pixels[(size_t)y * (size_t)outW + (size_t)x] = px;
    };

    // Decode rows
    for (int y = 0; y < outH; ++y) {
        const int srcY = topDown ? y : (outH - 1 - y);
        const size_t rowOff = pixelBase + (size_t)srcY * inStride;
        const uint8_t* row = data + rowOff;

        if (bpp == 24) {
            const size_t need = (size_t)outW * 3;
            for (int x = 0; x < outW; ++x) {
                const size_t o = (size_t)x * 3;
                const uint8_t b = row[o + 0];
                const uint8_t g = row[o + 1];
                const uint8_t r = row[o + 2];
                write565(x, y, rgb888_to_565(r, g, b));
            }
            (void)need;
        }
        else if (bpp == 32) {
            for (int x = 0; x < outW; ++x) {
                const size_t o = (size_t)x * 4;
                const uint32_t v =
                    (uint32_t)row[o + 0] |
                    ((uint32_t)row[o + 1] << 8) |
                    ((uint32_t)row[o + 2] << 16) |
                    ((uint32_t)row[o + 3] << 24);

                uint8_t r = 0, g = 0, b = 0;
                if (compression == BI_BITFIELDS || compression == BI_ALPHABITFIELDS) {
                    r = extract_and_scale(v, rMask);
                    g = extract_and_scale(v, gMask);
                    b = extract_and_scale(v, bMask);
                } else {
                    // BI_RGB default: BGRA
                    b = (uint8_t)(v & 0xFF);
                    g = (uint8_t)((v >> 8) & 0xFF);
                    r = (uint8_t)((v >> 16) & 0xFF);
                }
                write565(x, y, rgb888_to_565(r, g, b));
            }
        }
        else if (bpp == 16) {
            for (int x = 0; x < outW; ++x) {
                const size_t o = (size_t)x * 2;
                const uint16_t v16 = (uint16_t)row[o] | ((uint16_t)row[o + 1] << 8);

                uint8_t r = 0, g = 0, b = 0;
                if (compression == BI_BITFIELDS || compression == BI_ALPHABITFIELDS || rMask != 0) {
                    r = extract_and_scale((uint32_t)v16, rMask);
                    g = extract_and_scale((uint32_t)v16, gMask);
                    b = extract_and_scale((uint32_t)v16, bMask);
                } else {
                    // Fallback: assume 555
                    r = (uint8_t)(((v16 >> 10) & 0x1F) * 255 / 31);
                    g = (uint8_t)(((v16 >> 5)  & 0x1F) * 255 / 31);
                    b = (uint8_t)(((v16)       & 0x1F) * 255 / 31);
                }
                write565(x, y, rgb888_to_565(r, g, b));
            }
        }
        else if (bpp == 8) {
            if (palette.empty()) {
                LOG_ERROR(log, "BMP", "8bpp BMP missing palette.");
                return false;
            }
            for (int x = 0; x < outW; ++x) {
                const uint8_t idx = row[x];
                const uint32_t rgb = palette[(size_t)std::min<uint32_t>(idx, (uint32_t)palette.size() - 1)];
                const uint8_t r = (uint8_t)((rgb >> 16) & 0xFF);
                const uint8_t g = (uint8_t)((rgb >> 8) & 0xFF);
                const uint8_t b = (uint8_t)(rgb & 0xFF);
                write565(x, y, rgb888_to_565(r, g, b));
            }
        }
        else if (bpp == 4) {
            if (palette.empty()) {
                LOG_ERROR(log, "BMP", "4bpp BMP missing palette.");
                return false;
            }
            for (int x = 0; x < outW; ++x) {
                const uint8_t byte = row[(size_t)x / 2];
                const uint8_t idx = (x % 2 == 0) ? (byte >> 4) : (byte & 0x0F);
                const uint32_t rgb = palette[(size_t)std::min<uint32_t>(idx, (uint32_t)palette.size() - 1)];
                const uint8_t r = (uint8_t)((rgb >> 16) & 0xFF);
                const uint8_t g = (uint8_t)((rgb >> 8) & 0xFF);
                const uint8_t b = (uint8_t)(rgb & 0xFF);
                write565(x, y, rgb888_to_565(r, g, b));
            }
        }
        else if (bpp == 1) {
            if (palette.empty()) {
                LOG_ERROR(log, "BMP", "1bpp BMP missing palette.");
                return false;
            }
            for (int x = 0; x < outW; ++x) {
                const uint8_t byte = row[(size_t)x / 8];
                const uint8_t bit = 7 - (x % 8);
                const uint8_t idx = (byte >> bit) & 1u;
                const uint32_t rgb = palette[(size_t)idx];
                const uint8_t r = (uint8_t)((rgb >> 16) & 0xFF);
                const uint8_t g = (uint8_t)((rgb >> 8) & 0xFF);
                const uint8_t b = (uint8_t)(rgb & 0xFF);
                write565(x, y, rgb888_to_565(r, g, b));
            }
        }
        else {
            LOG_ERROR(log, "BMP", "Unsupported bpp: %u", (unsigned)bpp);
            return false;
        }
    }

    return true;
}