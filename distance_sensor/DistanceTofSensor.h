#pragma once
#include <Arduino.h>
#include "../IService.h"
#include "../GPIO.h"
#include "../Container.h"

class DistanceTofSensor: public IService {
public:
    DistanceTofSensor() {
      addSupportFeature("distance", true);
      addSupportFeature("hand", true);
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

    void begin() {
        pinMoe(DISTANCE_SENSOR_PIN, INPUT_PULLUP);
        setReady(true);
    }

    bool detectHand() {
        bool detected = digitalRead(DISTANCE_SENSOR_PIN) == LOW;
        return detected;
    }

private:
  int handDistance = 100; // 10cm
};