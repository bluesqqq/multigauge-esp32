#pragma once

#include "io/file/FileSystem.h"
#include "io/logging/Logger.h"

#include <rapidjson/document.h>

class AssetManager {
    private:
        FileSystem& fs;
        Logger& log;

    public:
        AssetManager(FileSystem& fs, Logger& log) : fs(fs), log(log) {}

        bool loadJson(const std::string& path, rapidjson::Document& out);
};