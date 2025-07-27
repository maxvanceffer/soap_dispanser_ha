#pragma once
#include <Arduino.h>
#include "../GPIO.h"
#include "StorageManager.h"

class BatteryManager {
public:
    BatteryManager(StorageManager& storage)
        : _storage(storage), _lastReadTime(0), _voltage(0.0), _percentage(-1) {}

    void begin() {
        pinMode(CHARGING_DETECT_PIN, INPUT);
        analogReadResolution(12);  // for ESP32
    }

    void update() {
        unsigned long now = millis();
        if (_lastReadTime == 0 || now - _lastReadTime >= 3600000 || isCharging()) {
            _lastReadTime = now;

            int raw = analogRead(BATTERY_ADC_PIN);
            if (raw <= 0 || raw >= 4095) {
                _percentage = -1;
                _storage.setItem("battery_percent", -1);
                _storage.setItem("battery_voltage", -1.0f);
                _storage.setItem("battery_last_read", now);
                _storage.save();
                return;
            }

            float v_adc = (raw / 4095.0f) * 3.3f;
            _voltage = v_adc * 2.0f; // voltage divider 100k/100k
            _percentage = voltageToPercentage(_voltage);

            _storage.setItem("battery_percent", _percentage);
            _storage.setItem("battery_voltage", _voltage);
            _storage.setItem("battery_last_read", now);
            _storage.save();
        } else {
            _percentage = _storage.getItem<int>("battery_percent", -1);
            _voltage = _storage.getItem<float>("battery_voltage", -1.0f);
        }
    }

    int getPercentage() const {
        return _percentage;
    }

    float getVoltage() const {
        return _voltage;
    }

    bool isCharging() const {
        return digitalRead(CHARGING_DETECT_PIN) == LOW;
    }

    bool isFullyCharged() const {
        return isCharging() && _voltage >= 4.15f;
    }

    bool shouldShowChargingInfo() const {
        return isCharging();
    }

private:
    StorageManager& _storage;
    unsigned long _lastReadTime;
    float _voltage;
    int _percentage;

    int voltageToPercentage(float voltage) {
        if (voltage >= 4.2f) return 100;
        if (voltage >= 4.1f) return 90;
        if (voltage >= 4.0f) return 80;
        if (voltage >= 3.9f) return 70;
        if (voltage >= 3.8f) return 60;
        if (voltage >= 3.7f) return 50;
        if (voltage >= 3.6f) return 40;
        if (voltage >= 3.5f) return 30;
        if (voltage >= 3.4f) return 20;
        if (voltage >= 3.3f) return 10;
        return 0;
    }
};
