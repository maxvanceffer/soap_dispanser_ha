// TimeManager.h
#pragma once
#include <Arduino.h>
#include <time.h>
#include <WiFi.h>
#include "../../IService.h"

class TimeManager: public IService {
public:
  TimeManager();

  String name() const override;
  ServiceValue getValue(String key) const override;
  bool execute(String fnName, JsonVariant args) override;
  bool begin(long gmtOffset = 10800, int daylightOffset = 0);
  String getFormattedTime() const;
  bool isTimeValid() const;
  time_t now() const;

private:
  long _gmtOffset;
  int _daylightOffset;
  time_t _lastSync;
};
