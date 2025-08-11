#pragma once
#include <Arduino.h>
#include "../../IService.h"

class SoapMotor: public IService {
public:
    enum class Mode {
        SMALL,
        MEDIUM,
        LARGE
    };

    SoapMotor();

    String name() const;
    ServiceValue getValue(String key) const;
    bool execute(String fnName, JsonVariant args);

    void begin();

    void setMode(Mode mode);
    void dispense();

private:
    Mode _mode;
    bool dispensing;

    unsigned int getDurationForMode(Mode mode);
};
