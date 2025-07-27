#include <Arduino.h>
#include "esp_sleep.h"
#include "driver/gpio.h"
#include "GPIO.h"
#include "Features.h"
#include "StorageManager.h"
#include "Utils.h"

#if FEATURE_SOAP_MOTOR
  #include "SoapMotor.h"
#endif

#if FEATURE_SLEEP_TIMER
  #include "SleepTimer.h"
#endif

#if FEATURE_DISTANCE_SENSOR
  #include "DistanceSensor.h"
#endif

#if FEATURE_BATTERY_MANAGER
  #include "BatteryManager.h"
#endif

#if FEATURE_SETUP_MANAGER
  #include "SetupManager.h"
#endif

#if FEATURE_SCREEN
  #include "ScreenManager.h"
#endif

#if FEATURE_TIME_MANAGER
  #include "TimeManager.h"
#endif

#if FEATURE_MOTION_SENSOR
  #include "MotionManager.h"
#endif

#if FEATURE_SCREEN
  #include "./screens/Screens.cpp"
#endif

#if FEATURE_STORAGE
StorageManager storage;
#endif

#if FEATURE_DISTANCE_SENSOR
DistanceSensor distanceSensor;
#endif

#if FEATURE_SOAP_MOTOR
SoapMotor soapMotor(storage);
#endif

#if FEATURE_SLEEP_TIMER
SleepTimer sleepTimer(storage);
#endif

#if FEATURE_BATTERY_MANAGER
BatteryManager batteryManager(storage);
#endif

#if FEATURE_TIME_MANAGER
TimeManager timeManager(storage);
#endif

#if FEATURE_SCREEN
ScreenManager screenManager(&storage, 
  #if FEATURE_BATTERY_MANAGER
    &batteryManager, 
  #else
    nullptr,
  #endif
  #if FEATURE_TIME_MANAGER
    &timeManager
  #else
    nullptr
  #endif
);
#endif

#if FEATURE_MOTION_SENSOR
MotionManager motionManager(
  #if FEATURE_SLEEP_TIMER
    &sleepTimer
  #else
    nullptr
  #endif
);
#endif

#if FEATURE_SETUP_MANAGER
SetupManager* setupManager = nullptr;
#else
void* setupManager = nullptr; // Dummy pointer to maintain code compatibility
#endif

unsigned long lastDispenseTime = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("SoapDispenserOS started");

  #if FEATURE_SOAP_MOTOR
    soapMotor.begin();
  #endif

  #if FEATURE_STORAGE
    storage.begin();
  #endif

  #if FEATURE_DISTANCE_SENSOR
    distanceSensor.begin();
  #endif

  #if FEATURE_SCREEN
    screenManager.begin();
  #endif

  #if FEATURE_MOTION_SENSOR
    motionManager.begin();
    motionManager.attachMotionInterrupt();
  #endif

  #if FEATURE_DISTANCE_SENSOR
    // Пробуждение от ИК-датчика
    esp_deep_sleep_enable_gpio_wakeup(1 << DISTANCE_SENSOR_PIN, ESP_GPIO_WAKEUP_GPIO_HIGH);
    gpio_set_direction((gpio_num_t)DISTANCE_SENSOR_PIN, GPIO_MODE_INPUT);
  #endif

  // Проверка причины пробуждения
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  Serial.print("Wakeup reason: ");
  Serial.println(wakeup_reason);

  #if FEATURE_STORAGE && FEATURE_SETUP_MANAGER
    if (storage.getItem("initial_setup", "true") == "true") {
      Serial.println("Initial setup called");
      #if FEATURE_SCREEN
        setupManager = new SetupManager(storage, sleepTimer, screenManager);
        setupManager->begin();
      #else
        Serial.println("Setup requires screen feature to be enabled");
      #endif
    } else {
      Serial.println("Normal routine started");
  #else
    {
      Serial.println("Normal routine started");
  #endif

    #if FEATURE_DISTANCE_SENSOR && FEATURE_SOAP_MOTOR
      if (wakeup_reason == ESP_SLEEP_WAKEUP_EXT0) {
        Serial.println("🖐 Обнаружена рука — подача мыла!");
        soapMotor.dispense();
        lastDispenseTime = millis();
      }
    #endif

    #if FEATURE_BATTERY_MANAGER
      batteryManager.begin();
      batteryManager.update();
      Serial.print("Battery: ");
      Serial.print(batteryManager.getPercentage());
      Serial.println("%");
    #endif

    #if FEATURE_SLEEP_TIMER
      sleepTimer.start();
    #endif
  }
}

void loop() {
  #if FEATURE_SETUP_MANAGER
    if (setupManager) {
      setupManager->loop();
      return;
    }
  #endif

  #if FEATURE_SLEEP_TIMER
    sleepTimer.update();
  #endif

  #if FEATURE_BATTERY_MANAGER
    batteryManager.update();
  #endif

  #if FEATURE_SCREEN
    screenManager.draw();
  #endif

  #if FEATURE_DISTANCE_SENSOR && FEATURE_SOAP_MOTOR
    if (distanceSensor.detectHand() && millis() - lastDispenseTime > 2000) {
      Serial.println("🖐 Рука снова обнаружена — подаём мыло.");
      soapMotor.dispense();
      lastDispenseTime = millis();
      
      #if FEATURE_SLEEP_TIMER
        sleepTimer.start(); // restart timer
      #endif
    }
  #endif
}
