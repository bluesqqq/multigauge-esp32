#include "AssetManager.h"

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