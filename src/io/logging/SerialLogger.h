#pragma once

#include "Logger.h"
#include <Arduino.h>
#include <cstdarg>
#include <cstdio>

class SerialLogger final : public Logger {
    private:
        uint32_t baud;
        bool wait;

        static char levelToChar(LogLevel level);
    
    protected:
        void _log(LogLevel level, const char* tag, const char* fmt, va_list args) override;

    public:
        explicit SerialLogger(uint32_t baud = 115200, bool waitForSerial = false);

        bool init() override;
};