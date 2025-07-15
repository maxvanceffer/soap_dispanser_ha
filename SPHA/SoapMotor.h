#pragma once
#include <Arduino.h>
#include "StorageManager.h"
#include "GPIO.h"

class SoapMotor {
public:
    enum class Mode {
        SMALL,
        MEDIUM,
        LARGE
    };

    SoapMotor(StorageManager& storage) : _storage(storage) {
        String value = _storage.getItem<String>("soap_mode", "MEDIUM");
        if (value == "SMALL") _mode = Mode::SMALL;
        else if (value == "LARGE") _mode = Mode::LARGE;
        else _mode = Mode::MEDIUM;
    }

    void setMode(Mode mode) {
        _mode = mode;
        String value;
        switch (mode) {
            case Mode::SMALL: value = "SMALL"; break;
            case Mode::MEDIUM: value = "MEDIUM"; break;
            case Mode::LARGE: value = "LARGE"; break;
        }
        _storage.setItem("soap_mode", value);
        _storage.save();
    }

    void begin() {
        pinMode(MOTOR_IN1, OUTPUT);
        pinMode(MOTOR_IN2, OUTPUT);
        digitalWrite(MOTOR_IN1, LOW);
        digitalWrite(MOTOR_IN2, LOW);
    }

    void dispense() {
        unsigned int duration = getDurationForMode(_mode);

        digitalWrite(MOTOR_IN1, HIGH);
        digitalWrite(MOTOR_IN2, LOW);
        delay(duration);
        digitalWrite(MOTOR_IN1, LOW);
        digitalWrite(MOTOR_IN2, LOW);
    }

private:
    StorageManager& _storage;
    Mode _mode;

    unsigned int getDurationForMode(Mode mode) {
        switch (mode) {
            case Mode::SMALL: return 300;
            case Mode::MEDIUM: return 600;
            case Mode::LARGE: return 1000;
        }
        return 600;
    }
};
