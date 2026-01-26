#pragma once

#include <cstdarg>
#include <cstdint>

#define LOG_DEBUG(logger, tag, fmt, ...) (logger).log(LogLevel::Debug, tag, fmt, ##__VA_ARGS__)
#define LOG_INFO(logger, tag, fmt, ...) (logger).log(LogLevel::Info,  tag, fmt, ##__VA_ARGS__)
#define LOG_WARN(logger, tag, fmt, ...) (logger).log(LogLevel::Warn,  tag, fmt, ##__VA_ARGS__)
#define LOG_ERROR(logger, tag, fmt, ...) (logger).log(LogLevel::Error, tag, fmt, ##__VA_ARGS__)

enum class LogLevel : uint8_t {
    Debug,
    Info,
    Warn,
    Error
};

class Logger {
    private:
        LogLevel minLevel = LogLevel::Debug;

    protected:
        virtual void _log(LogLevel level, const char* tag, const char* fmt, va_list args) = 0;

    public:
        virtual ~Logger() = default;

        virtual bool init();

        void log(LogLevel level, const char* tag, const char* fmt, ...);

        void setMinLevel(LogLevel l);
};