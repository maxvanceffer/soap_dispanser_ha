#include "SoapMotor.h"
#include "../../GPIO.h"
#include "../../Container.h"

SoapMotor::SoapMotor() : dispensing(false) {
    String value = Container::getInstance()->getStorage().getItem<String>("soap_mode", "MEDIUM");

    if (value == "SMALL") _mode = Mode::SMALL;
    else if (value == "LARGE") _mode = Mode::LARGE;
    else _mode = Mode::MEDIUM;

    addSupportFeature("dispense", true);
}

void SoapMotor::setMode(Mode mode) {
    _mode = mode;
    String value;
    switch (mode) {
        case Mode::SMALL: value = "SMALL"; break;
        case Mode::MEDIUM: value = "MEDIUM"; break;
        case Mode::LARGE: value = "LARGE"; break;
    }

    Container::getInstance()->getStorage().setItem("soap_mode", value);
    Container::getInstance()->getStorage().save();
}

void SoapMotor::begin() {
    Serial.println("SoapMotor:begin started");
    pinMode(MOTOR_IN1, OUTPUT);
    Serial.println("SoapMotor:begin MOTOR_IN1, OUTPUT");
    pinMode(MOTOR_IN2, OUTPUT);
    Serial.println("SoapMotor:begin MOTOR_IN2, OUTPUT");
//    pinMode(MOTOR_N_SLEEP, OUTPUT);
//    Serial.println("SoapMotor:begin MOTOR_N_SLEEP, OUTPUT");
    digitalWrite(MOTOR_IN1, LOW);
    Serial.println("SoapMotor:begin MOTOR_IN1, LOW");
    digitalWrite(MOTOR_IN2, LOW);
    Serial.println("SoapMotor:begin MOTOR_IN2, LOW");
//    digitalWrite(MOTOR_N_SLEEP, LOW); // Ensure motor driver is in sleep mode initially
//    Serial.println("SoapMotor:begin MOTOR_N_SLEEP, LOW");
    Serial.println("SoapMotor:begin finished");
}

void SoapMotor::dispense() {
    if (dispensing == true) {
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

String SoapMotor::name() const {
    return "SoapMotor";
}

ServiceValue SoapMotor::getValue(String key) const {
  if (key == "isDispensing") return ServiceValue(dispensing);

  return ServiceValue();
}

bool SoapMotor::execute(String fnName, JsonVariant args) {
  if (fnName == "dispense") {
    dispense();
    return true;
  }

  return false;
}

void SoapMotor::buildSettingsSchema(JsonObject schema) const {
    // Initialize base schema skeleton from IService
    initSchemaSkeleton(schema);

    JsonObject props = schema["properties"].as<JsonObject>();

    // 1) Mode selector
    JsonObject m = props.createNestedObject("mode");
    m["type"] = "string";
    JsonArray en = m.createNestedArray("enum");
    en.add("SMALL");
    en.add("MEDIUM");
    en.add("LARGE");

    const char* defMode = "MEDIUM";
    switch (_mode) {
        case Mode::SMALL:  defMode = "SMALL"; break;
        case Mode::LARGE:  defMode = "LARGE"; break;
        case Mode::MEDIUM: default: defMode = "MEDIUM"; break;
    }
    m["default"] = defMode;
    m["title"] = "Dispense mode";
    m["description"] = "Preset amount of soap.";

    // UI metadata for mode (not part of JSON Schema)
    {
      JsonObject comp = m.createNestedObject("component");
      comp["ui"] = "select";
      comp["type"] = "string";
    }
}

unsigned int SoapMotor::getDurationForMode(Mode mode) {
    switch (mode) {
        case Mode::SMALL: return 300;
        case Mode::MEDIUM: return 600;
        case Mode::LARGE: return 1000;
    }
    return 600;
}