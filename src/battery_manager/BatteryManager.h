#pragma once
#include <Arduino.h>
#include "../../GPIO.h"
#include "../../IService.h"

class BatteryManager: public IService {
public:
    BatteryManager(): _lastReadTime(0), _voltage(0.0), _percentage(-1) {}

    String name() const override;
    ServiceValue getValue(String key) const override;
    bool execute(String fnName, JsonVariant args) override;

    bool begin();
    bool update();
    int getPercentage() const;
    float getVoltage() const;
    bool isCharging() const;
    bool isFullyCharged() const;
    bool shouldShowChargingInfo() const;

private:
    unsigned long _lastReadTime;
    float _voltage;
    int _percentage;

    int voltageToPercentage(float voltage);
};
