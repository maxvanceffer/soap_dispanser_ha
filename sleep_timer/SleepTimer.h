#pragma once
#include <Arduino.h>
#include "../Container.h"
#include "../IService.h"

class SleepTimer: public IService {
public:
    SleepTimer(): _active(false), _startTime(0) {
        addSupportFeature("hibernate", true);

        _timeoutMs = Container::getInstance()
          .getStorage()
          .getItem<uint32_t>("sleep_timeout_ms", 10000, true);

        setReady(true);
    }

    String name() const override {
        return "SleepTimer";
    }

    ServiceValue getValue(String key) const override {
        if (key == "timeout") {
            return ServiceValue(_timeoutMs);
        } else if (key == "active") {
            return ServiceValue(_active);
        }

        return ServiceValue();
    }

    bool execute(String fnName, JsonVariant args) override {
        if (fnName == "start") {
            return start();
        } else if (fnName == "stop") {
            return stop();
        } else if (fnName == "update") {
            update();
            return true;
        }

        return false;
    }

    void setTimeout(uint32_t timeoutMs) {
        _timeoutMs = timeoutMs;
        _storage.setItem("sleep_timeout_ms", timeoutMs);
        _storage.save();
    }

    bool start() {
        if (_active) {
          Serial.println("SleepTimer:start is already running");
          return false;
        }

        _startTime = millis();
        _active = true;
        return true;
    }

    bool stop() {
        if (_active == false) {
          Serial.println("SleepTimer:start is already stopped");
          return false;
        }

        _active = false;
        return true;
    }

    bool update() {
        if (_active && millis() - _startTime >= _timeoutMs) {
            Serial.println("Timeout reached. Going to deep sleep...");
            delay(100);
            esp_deep_sleep_start();
        }
    }

private:
    uint32_t _timeoutMs;
    uint32_t _startTime;
    bool _active;
};
