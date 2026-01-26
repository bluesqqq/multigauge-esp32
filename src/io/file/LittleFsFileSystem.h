#pragma once

#include "FileSystem.h"

#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>

class LittleFsFileSystem : public FileSystem {
    public:
        bool init() override {
            return LittleFS.begin(true);
        }

        bool readAll(const std::string& path, std::vector<uint8_t>& out) override {
            out.clear();

            File f = LittleFS.open(path.c_str(), "r");
            if (!f) return false;
            if (f.isDirectory()) { f.close(); return false; }

            const size_t length = (size_t)f.size();
            out.resize(length);

            size_t total = 0;
            while (total < length) {
                int n = f.read(out.data() + total, length - total);
                if (n <= 0) break;
                total += (size_t)n;
            }

            f.close();

            if (total != length) {
                out.clear();
                return false;
            }

            return true;
        }

    bool writeAll(const std::string& path, const uint8_t* data, size_t length) override {
        File f = LittleFS.open(path.c_str(), "w");
        if (!f) return false;
        if (f.isDirectory()) { f.close(); return false; }

        size_t written = 0;
        while (written < length) {
            size_t n = f.write(data + written, length - written);
            if (n == 0) break;
            written += n;
        }

        f.close();
        return written == length;
    }

    bool exists(const std::string& path) override { return LittleFS.exists(path.c_str()); }

    bool size(const std::string& path, size_t& outsize) override {
        File f = LittleFS.open(path.c_str(), "r");
        if (!f) return false;
        if (f.isDirectory()) { f.close(); return false; }

        outsize = (size_t)f.size();
        f.close();
        return true;
    }

    bool remove(const std::string& path) override {
        return LittleFS.remove(path.c_str());
    }

    bool rename(const std::string& from, const std::string& to) override {
        return LittleFS.rename(from.c_str(), to.c_str());
    }

    bool makeDirectory(const std::string& path) override {
        return LittleFS.mkdir(path.c_str());
    }

    bool listDirectories(const std::string& path, std::vector<std::string>& outNames) {
        outNames.clear();

        File dir = LittleFS.open(path.c_str(), "r");
        if (!dir) return false;
        if (!dir.isDirectory()) { dir.close(); return false; }
        for (File f = dir.openNextFile(); f; f = dir.openNextFile()) {
            const char* name = f.name();
            if (name) outNames.emplace_back(name);
            f.close();
        }

        dir.close();
        return true;
    }
};
