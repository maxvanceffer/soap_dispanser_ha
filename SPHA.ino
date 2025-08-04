#include <Arduino.h>
#include "GPIO.h"
#include "Features.h"
#include "Utils.h"
#include "esp_sleep.h"
#include "driver/gpio.h"
#include "Container.h"

#if FEATURE_SOAP_MOTOR
  #include "soap_motor/SoapMotor.h"
#endif

#if FEATURE_SLEEP_TIMER
  #include "sleep_timer/SleepTimer.h"
#endif

#if FEATURE_DISTANCE_SENSOR
  #if FEATURE_DISTANCE_PIR_SENSOR
    #include "distance_sensor/DistancePirSensor.h"
  #endif
  #if FEATURE_DISTANCE_TOF_SENSOR
    #include "distance_sensor/DistanceTofSensor.h"
  #endif
#endif

#if FEATURE_BATTERY_MANAGER
  #include "battery_manager/BatteryManager.h"
#endif

#if FEATURE_SETUP_MANAGER
  #include "setup_manager/SetupManager.h"
#endif

#if FEATURE_SCREEN
  #include "screen/ScreenManager.h"
#endif

#if FEATURE_TIME_MANAGER
  #include "time_manager/TimeManager.h"
#endif

#if FEATURE_MOTION_SENSOR
  #include "motion_sensor/MotionManager.h"
#endif

#if FEATURE_SCREEN
  #include "./screen/128x64/Screens.cpp"
#endif

#if FEATURE_DISTANCE_SENSOR
 #if FEATURE_DISTANCE_PIR_SENSOR
  DistancePirSensor distanceSensor;
 #endif
 #if FEATURE_DISTANCE_TOF_SENSOR
  DistanceTofSensor distanceSensor;
 #endif
#endif

#if FEATURE_SOAP_MOTOR
SoapMotor soapMotor();
#endif

#if FEATURE_SLEEP_TIMER
SleepTimer sleepTimer();
#endif

#if FEATURE_BATTERY_MANAGER
BatteryManager batteryManager();
#endif

#if FEATURE_TIME_MANAGER
TimeManager timeManager();
#endif

#if FEATURE_SCREEN
ScreenManager screenManager(
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

  Container::getInstance().getStorage().begin();

  #if FEATURE_SOAP_MOTOR
    soapMotor.begin();
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
    // wakeup ir setup
    esp_deep_sleep_enable_gpio_wakeup(1 << DISTANCE_SENSOR_PIN, ESP_GPIO_WAKEUP_GPIO_HIGH);
    gpio_set_direction((gpio_num_t)DISTANCE_SENSOR_PIN, GPIO_MODE_INPUT);
  #endif

  // Check reason of wakeup
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  Serial.print("Wakeup reason: ");
  Serial.println(wakeup_reason);

  #if FEATURE_SETUP_MANAGER
    if (Container::getInstance().getStorage().getItem("initial_setup", "true") == "true") {
      Serial.println("Initial setup called");
      #if FEATURE_SCREEN
        setupManager = new SetupManager(Container::getInstance().getStorage(), sleepTimer, screenManager);
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
