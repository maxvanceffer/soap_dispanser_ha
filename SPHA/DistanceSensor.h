#pragma once
#include <Arduino.h>
#include "GPIO.h"

class DistanceSensor {
public:
    void begin() {
        pinMode(DISTANCE_SENSOR_PIN, INPUT_PULLUP);  // Включаем подтяжку
    }

    bool detectHand() {
        bool detected = digitalRead(DISTANCE_SENSOR_PIN) == LOW;
        return detected;
    }
};