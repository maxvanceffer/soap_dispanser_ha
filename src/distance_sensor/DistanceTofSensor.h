#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include "../../IService.h"
#include "../../GPIO.h"

class DistanceTofSensor: public IService {
public:
    DistanceTofSensor();

    String name() const override;
    ServiceValue getValue(String key) const override;
    bool execute(String fnName, JsonVariant args) override;

    void begin();
    void loop();

    bool detectHand() const;

private:
  mutable Adafruit_VL53L0X lox;
  int handDetectThresholdMm = 100; // 100mm
};