#include <Arduino.h>
#include "GPIO.h"
#include "StorageManager.h"
#include "SoapMotor.h"
#include "SleepTimer.h"
#include "DistanceSensor.h"
#include "BatteryManager.h"
#include "SetupManager.h"
#include "ScreenManager.h"
#include "TimeManager.h"
#include "MotionManager.h"
#include "./screens/Screens.cpp"

StorageManager storage;
// DistanceSensor distanceSensor;
// SoapMotor soapMotor(storage);
SleepTimer sleepTimer(storage);
BatteryManager batteryManager(storage);
TimeManager timeManager(storage);
ScreenManager screenManager(&storage, &batteryManager, &timeManager);
// MotionManager motionManager(&sleepTimer);

SetupManager* setupManager = nullptr;

unsigned long lastDispenseTime = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Soap Dispenser");

  if (!LittleFS.begin(true)) {
     Serial.println("❌ Failed to mount LittleFS even after formatting");
  } else {
     Serial.println("✅ LittleFS mounted successfully");
  }

  File file = LittleFS.open("/test.txt", "w");
  if (!file) {
    Serial.println("❌ Failed to create file");
  } else {
    file.println("LittleFS works!");
    file.close();
    Serial.println("✅ Test file written");
  }

  storage.begin();
//   screenManager.begin();
//   motionManager.begin();
//   motionManager.attachMotionInterrupt();

  // storage.setItem("initial_setup", "true");

  // Check initial setup mode
  if (storage.getItem("initial_setup", "true") == "true") {
    Serial.println("Initial setup called");
    // setupManager = new SetupManager(storage, sleepTimer, screenManager);
    // setupManager->begin();
  } else {
    Serial.println("Normal routine started");
//     soapMotor.begin();

//     esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
//     if (wakeup_reason == ESP_SLEEP_WAKEUP_EXT0) {
//       Serial.println("Dispensing soap...");
//       soapMotor.dispense();
//     }

    Serial.print("Wakeup reason: ");
//     Serial.println(wakeup_reason);

  //   distanceSensor.begin();
  //   batteryManager.begin();
  //   batteryManager.update();
  //   Serial.print("Battery: ");
  //   Serial.print(batteryManager.getPercentage());
  //   Serial.println("%");

    // pinMode(ROTARY_WAKEUP_PIN, INPUT);
    // esp_sleep_enable_ext0_wakeup(ROTARY_WAKEUP_PIN, 1);
    // esp_sleep_enable_ext0_wakeup((gpio_num_t)PIR_GPIO, 1);

    sleepTimer.start();
  }
}

void loop() {
  if (setupManager) {
//     setupManager->loop();
    return;
  }

//   sleepTimer.update();
  // batteryManager.update();
  // screenManager.draw();

  // if (distanceSensor.detectHand() && millis() - lastDispenseTime > 2000) {
  //   Serial.println("Detected hand again — dispensing soap.");
  //   soapMotor.dispense();
  //   lastDispenseTime = millis();
  //   sleepTimer.start(); // restart timer
  // }
}
