#pragma once
#include <Arduino.h>
#include "StorageManager.h"

class SleepTimer {
public:
    SleepTimer(StorageManager& storage)
      : _storage(storage), _active(false), _startTime(0)
    {
        _timeoutMs = _storage.getItem<uint32_t>("sleep_timeout_ms", 10000, true);
    }

    void setTimeout(uint32_t timeoutMs) {
        _timeoutMs = timeoutMs;
        _storage.setItem("sleep_timeout_ms", timeoutMs);
        _storage.save();
    }

    void start() {
        _startTime = millis();
        _active = true;
    }

    void stop() {
        _active = false;
    }

    void update() {
        if (_active && millis() - _startTime >= _timeoutMs) {
            Serial.println("Timeout reached. Going to deep sleep...");
            delay(100);  // дать время сериалу перед сном
            esp_deep_sleep_start();
        }
    }

private:
    StorageManager& _storage;
    uint32_t _timeoutMs;
    uint32_t _startTime;
    bool _active;
};