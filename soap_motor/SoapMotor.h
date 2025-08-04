#pragma once
#include <Arduino.h>
#include "../IService.h"
#include "../storage/StorageManager.h"
#include "../GPIO.h"

class SoapMotor: public IService {
public:
    enum class Mode {
        SMALL,
        MEDIUM,
        LARGE
    };

    SoapMotor(StorageManager& storage) : _storage(storage), dispensing(false) {
        String value = _storage.getItem<String>("soap_mode", "MEDIUM");
        if (value == "SMALL") _mode = Mode::SMALL;
        else if (value == "LARGE") _mode = Mode::LARGE;
        else _mode = Mode::MEDIUM;

        addSupportFeature("dispense", true)
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
        Serial.println("SoapMotor:begin started");
        pinMode(MOTOR_IN1, OUTPUT);
        Serial.println("SoapMotor:begin MOTOR_IN1, OUTPUT");
        pinMode(MOTOR_IN2, OUTPUT);
        Serial.println("SoapMotor:begin MOTOR_IN2, OUTPUT");
//        pinMode(MOTOR_N_SLEEP, OUTPUT);
//        Serial.println("SoapMotor:begin MOTOR_N_SLEEP, OUTPUT");
        digitalWrite(MOTOR_IN1, LOW);
        Serial.println("SoapMotor:begin MOTOR_IN1, LOW");
        digitalWrite(MOTOR_IN2, LOW);
        Serial.println("SoapMotor:begin MOTOR_IN2, LOW");
//        digitalWrite(MOTOR_N_SLEEP, LOW); // Ensure motor driver is in sleep mode initially
//        Serial.println("SoapMotor:begin MOTOR_N_SLEEP, LOW");
        Serial.println("SoapMotor:begin finished");
    }

    void dispense() {
        if (dispensing === true) {
          Serial.println("Dispense already running");
          return;
        }

        dispensing = true;
        unsigned int duration = getDurationForMode(_mode);
        Serial.println("Dispense started");
        Serial.println(duration);

        // Wake up the motor driver
        digitalWrite(MOTOR_N_SLEEP, HIGH);
        delay(5); // Small delay to ensure the driver is fully awake
        
        // Start the motor
        digitalWrite(MOTOR_IN1, HIGH);
        digitalWrite(MOTOR_IN2, LOW);
        
        // Run for the specified duration
        delay(duration);
        Serial.println("Delay resumed");
        
        // Stop the motor
        digitalWrite(MOTOR_IN1, LOW);
        digitalWrite(MOTOR_IN2, LOW);
        
        // Put the motor driver back to sleep
        digitalWrite(MOTOR_N_SLEEP, LOW);
        Serial.println("Dispense stoped");
        dispensing = false;
    }

    String name() const override {
        return "SoapMotor";
    }

    ServiceValue getValue(String key) {
      if (key === "isDispensing") return ServiceValue(dispensing);

      return ServiceValue();
    }

    bool execute(String fnName, JsonVariant args) {
      if (fnName === "dispense") {
        dispense();
        return ture;
      }

      return false;
    }

private:
    StorageManager& _storage;
    Mode _mode;
    bool dispensing;

    unsigned int getDurationForMode(Mode mode) {
        switch (mode) {
            case Mode::SMALL: return 300;
            case Mode::MEDIUM: return 600;
            case Mode::LARGE: return 1000;
        }
        return 600;
    }
};
