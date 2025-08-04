#include "BatteryManager.h"
#include "../Container.h"

String BatteryManager::name() const override {
    return "Battery";
}

ServiceValue BatteryManager::getValue(String key) const override {
    if (key == "percentage") {
        return ServiceValue(getPercentage());
    } else if (key == "voltage") {
        return ServiceValue(getVoltage());
    } else if (key == "charging") {
        return ServiceValue(isCharging());
    } else if (key == "full") {
        return ServiceValue(isFullyCharged());
    }

    return ServiceValue();
}

bool BatteryManager::execute(String fnName, JsonVariant args) override {
    if (fnName == "begin") {
        return begin();
    } else if (fnName == "update") {
        return update();
    }

    return false;
}

bool BatteryManager::begin() {
    pinMode(CHARGING_DETECT_PIN, INPUT);
    analogReadResolution(12);  // for ESP32
    setReady(true);
    return true;
}

bool BatteryManager::update() {
    unsigned long now = millis();
    if (_lastReadTime == 0 || now - _lastReadTime >= 3600000 || isCharging()) {
        _lastReadTime = now;

        int raw = analogRead(BATTERY_ADC_PIN);
        if (raw <= 0 || raw >= 4095) {
            _percentage = -1;
            Container::getInstance().getStorage().setItem("battery_percent", -1);
            Container::getInstance().getStorage().setItem("battery_voltage", -1.0f);
            Container::getInstance().getStorage().setItem("battery_last_read", now);
            Container::getInstance().getStorage().save();
            return;
        }

        float v_adc = (raw / 4095.0f) * 3.3f;
        _voltage = v_adc * 2.0f; // voltage divider 100k/100k
        _percentage = voltageToPercentage(_voltage);

        Container::getInstance().getStorage().setItem("battery_percent", _percentage);
        Container::getInstance().getStorage().setItem("battery_voltage", _voltage);
        Container::getInstance().getStorage().setItem("battery_last_read", now);
        Container::getInstance().getStorage().save();
    } else {
        _percentage = Container::getInstance().getStorage().getItem<int>("battery_percent", -1);
        _voltage = Container::getInstance().getStorage().getItem<float>("battery_voltage", -1.0f);
    }

    return true;
}

int BatteryManager::getPercentage() const {
    return _percentage;
}

float BatteryManager::getVoltage() const {
    return _voltage;
}

bool BatteryManager::isCharging() const {
    return digitalRead(CHARGING_DETECT_PIN) == LOW;
}

bool BatteryManager::isFullyCharged() const {
    return isCharging() && _voltage >= 4.15f;
}

bool BatteryManager::shouldShowChargingInfo() const {
    return isCharging();
}

int BatteryManager::voltageToPercentage(float voltage) {
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