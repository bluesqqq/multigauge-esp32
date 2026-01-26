#include "SerialLogger.h"

char SerialLogger::levelToChar(LogLevel level) {
    switch (level) {
        switch (level) {
            case LogLevel::Debug: return 'D';
            case LogLevel::Info:  return 'I';
            case LogLevel::Warn:  return 'W';
            case LogLevel::Error: return 'E';
        }
        return '?';
    }
}

void SerialLogger::_log(LogLevel level, const char *tag, const char *fmt, va_list args) {
    char msg[256];
    vsnprintf(msg, sizeof(msg), fmt, args);

    Serial.print('[');
    Serial.print(levelToChar(level));
    Serial.print("] ");

    if (tag && tag[0]) {
        Serial.print(tag);
        Serial.print(": ");
    }

    Serial.println(msg);
}

SerialLogger::SerialLogger(uint32_t baud, bool waitForSerial) : baud(baud), wait(waitForSerial) {}

bool SerialLogger::init() {
    Serial.begin(baud);
    if (wait) {
        while (!Serial) { delay(10); }
    }
    return true;
}