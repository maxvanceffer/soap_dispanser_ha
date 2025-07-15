#pragma once
#include <Arduino.h>
#include "GPIO.h"

class DistanceSensor {
public:
    void begin() {
        pinMode(DISTANCE_SENSOR_PIN, INPUT);
    }

    bool detectHand() {
        int raw = analogRead(DISTANCE_SENSOR_PIN);
        return raw < 1000; // порог — можно подстроить
    }
};