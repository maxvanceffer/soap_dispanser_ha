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

    void begin();
    void setMode(Mode mode);
    void dispense();
    void buildSettingsSchema(JsonObject schema) const override;

    bool hasSettingsSchema() const override { return true; }
    bool execute(String fnName, JsonVariant args);

private:
    Mode _mode;
    bool dispensing;

    unsigned int getDurationForMode(Mode mode);
};
