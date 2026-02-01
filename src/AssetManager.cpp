#include "AssetManager.h"
#include "images/ImageDecoder.h"

bool AssetManager::loadJson(const std::string &path, rapidjson::Document &out) {
    std::vector<uint8_t> bytes;
    if (!fs.readAll(path, bytes)) {
        LOG_ERROR(log, "asset", "loadJson: readAll failed: %s", path.c_str());
        return false;
    }

    if (bytes.empty()) {
        LOG_ERROR(log, "asset", "loadJson: empty file: %s", path.c_str());
        return false;
    }

    bytes.push_back(0);
    
    const char* json = reinterpret_cast<const char*>(bytes.data());
    const size_t length = bytes.size() - 1;

    out.Parse(json, length);

    if (out.HasParseError()) {
        const auto code = out.GetParseError();
        const size_t off = out.GetErrorOffset();
        LOG_ERROR(log, "asset", "loadJson: parse error: %s (offset=%u) file=%s", rapidjson::GetParseErrorFunc(code), (unsigned)off, path.c_str());
        return false;
    }

    return true;
}

bool AssetManager::loadImage(const std::string &path, Image &out) {
    if (!fs.exists(path)) {
        LOG_ERROR(log, "asset", "Image does not exist: %s", path.c_str());
        return false;
    }

    std::vector<uint8_t> data;
    if (!fs.readAll(path, data)) {
        LOG_ERROR(log, "asset", "Failed to read image: %s", path.c_str());
        return false;
    }

    if (data.empty()) {
        LOG_ERROR(log, "asset", "Image is empty: %s", path.c_str());
        return false;
    }

    ImageType type = detectImageType(data.data(), data.size());

    ImageInfo info;

    switch (type) {
        case ImageType::BMP: {
            LOG_DEBUG(log, "asset", "decoding BMP: %s", path.c_str());
            if (!decodeBMP(data.data(), data.size(), info, log)) return false;
            break;
        }

        case ImageType::PNG:
            LOG_DEBUG(log, "asset", "decoding PNG: %s", path.c_str());
            if (!decodePNG(data.data(), data.size(), info, log)) return false;
            break;

        case ImageType::JPG:
            LOG_WARN(log, "asset", "JPG not implemented yet.");
            return false;

        default:
            LOG_DEBUG(log, "asset", "Unsupported image format: %s", path.c_str());
            return false;
    }
    
    out = ctx.createNativeImage(info.pixels.data(), info.width, info.height);
    return true;
}
