#pragma once
#include <Arduino.h>
#include "../../Container.h"
#include "../../IService.h"

class SleepTimer: public IService {
public:
    SleepTimer();

    String name() const override;
    ServiceValue getValue(String key) const override;
    bool execute(String fnName, JsonVariant args) override;

    void setTimeout(uint32_t timeoutMs);

    bool start();
    bool stop();
    void loop();

private:
    uint32_t _timeoutMs;
    uint32_t _startTime;
    bool _active;
};
