#pragma once
#include <Arduino.h>
#include "../../GPIO.h"
#include "../../IService.h"

class DistancePirSensor: public IService {
public:
    DistancePirSensor() {
      addSupportFeature("hand", true);
    }

    void begin() {
        pinMode(DISTANCE_SENSOR_PIN, INPUT_PULLUP);
        setReady(true);
    }

    bool detectHand() {
        bool detected = digitalRead(DISTANCE_SENSOR_PIN) == LOW;
        return detected;
    }

    String name() const override {
        return "DistanceSensor";
    }

    ServiceValue getValue(String key) const override {
        if (key == "handDetected") {
            // This is not ideal as it's modifying state in a const method,
            // but we'll use a const_cast as a workaround
            bool detected = digitalRead(DISTANCE_SENSOR_PIN) == LOW;
            return ServiceValue(detected);
        }
        return ServiceValue();
    }

    bool execute(String fnName, JsonVariant args) override {
        if (fnName == "detectHand") {
            return detectHand();
        }
        return false;
    }
};