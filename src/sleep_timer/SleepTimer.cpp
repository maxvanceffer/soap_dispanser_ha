#include "SleepTimer.h"
#include "../../Container.h"

SleepTimer::SleepTimer(): _active(false), _startTime(0) {
    addSupportFeature("hibernate", true);

    _timeoutMs = Container::getInstance()
      ->getStorage()
      .getItem<uint32_t>("sleep_timeout_ms", 10000, true);

    setReady(true);
}

String SleepTimer::name() const {
    return "SleepTimer";
}

ServiceValue SleepTimer::getValue(String key) const {
    if (key == "timeout") {
        return ServiceValue(_timeoutMs);
    } else if (key == "active") {
        return ServiceValue(_active);
    }

    return ServiceValue();
}

bool SleepTimer::execute(String fnName, JsonVariant args) {
    if (fnName == "start") {
        return start();
    } else if (fnName == "stop") {
        return stop();
    }

    return false;
}

void SleepTimer::setTimeout(uint32_t timeoutMs) {
    _timeoutMs = timeoutMs;
    Container::getInstance()
          ->getStorage()
          .setItem("sleep_timeout_ms", timeoutMs);

    Container::getInstance()
          ->getStorage()
          .save();
}

bool SleepTimer::start() {
    Serial.println("SleepTimer:start restarting sleep timer");
    _startTime = millis();
    _active = true;
    return true;
}

bool SleepTimer::stop() {
    if (_active == false) {
      Serial.println("SleepTimer:start is already stopped");
      return false;
    }

    _active = false;
    return true;
}

void SleepTimer::loop() {
    if (!_active) return;

    unsigned long now = millis();
    unsigned long elapsed = now - _startTime;

    if (elapsed >= _timeoutMs) {
        Serial.println("Timeout reached. Going to deep sleep...");
        delay(100);
        esp_deep_sleep_start();
    }

    static unsigned long lastLog = 0;
    if (now - lastLog >= 1000) {
        lastLog = now;
        unsigned long remaining = (_timeoutMs > elapsed) ? (_timeoutMs - elapsed) : 0;
        Serial.printf("💤 Осталось до сна: %lu сек\n", remaining / 1000);
    }
}

void SleepTimer::buildSettingsSchema(JsonObject schema) const {
    // Prepare base JSON Schema skeleton
    initSchemaSkeleton(schema);

    JsonObject props = schema["properties"].as<JsonObject>();

    // Bounds in milliseconds: 10s .. 30s
    const uint32_t kMin = 10000; // 10 seconds
    const uint32_t kMax = 30000; // 30 seconds

    uint32_t def = _timeoutMs;
    if (def < kMin) def = kMin;
    if (def > kMax) def = kMax;

    JsonObject t = props.createNestedObject("sleep_timeout_ms");
    t["type"] = "integer";
    t["minimum"] = kMin;
    t["maximum"] = kMax;
    t["default"] = def;
    t["title"] = "Sleep timeout (ms)";
    t["description"] = "Device enters deep sleep after this idle time (10–30 seconds).";

    // UI metadata (not part of JSON Schema)
    JsonObject comp = t.createNestedObject("component");
    comp["ui"] = "input";
    comp["type"] = "int";
}