#include <Arduino.h>
#include "GPIO.h"
#include "Container.h"
#include "Features.h"
#include "Utils.h"
#include "esp_sleep.h"
#include "driver/gpio.h"

#if FEATURE_SOAP_MOTOR
  #include "src/soap_motor/SoapMotor.h"
#endif

#if FEATURE_SLEEP_TIMER
  #include "src/sleep_timer/SleepTimer.h"
#endif

#if FEATURE_DISTANCE_SENSOR
  #if FEATURE_DISTANCE_PIR_SENSOR
    #include "src/distance_sensor/DistancePirSensor.h"
  #endif
  #if FEATURE_DISTANCE_TOF_SENSOR
    #include "src/distance_sensor/DistanceTofSensor.h"
  #endif
#endif

#if FEATURE_BATTERY_MANAGER
  #include "src/battery_manager/BatteryManager.h"
#endif

#if FEATURE_SETUP_MANAGER
  #include "src/setup_manager/SetupManager.h"
#endif

#if FEATURE_SCREEN
  #include "src/screen/ScreenManager.h"
#endif

#if FEATURE_TIME_MANAGER
  #include "src/time_manager/TimeManager.h"
#endif

#if FEATURE_MOTION_SENSOR
  #include "src/motion_sensor/MotionManager.h"
#endif

#if FEATURE_BATTERY_MANAGER
BatteryManager batteryManager;
#endif

#if FEATURE_TIME_MANAGER
TimeManager timeManager;
#endif

unsigned long lastDispenseTime = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("SoapDispenserOS started");

  Container::getInstance()->getStorage().begin();

  #if FEATURE_SOAP_MOTOR
    SoapMotor *soapMotor = new SoapMotor();
    soapMotor->begin();
    Container::getInstance()->addService(soapMotor);
  #endif

  #if FEATURE_DISTANCE_SENSOR
   #if FEATURE_DISTANCE_PIR_SENSOR
    DistancePirSensor *distanceSensor = new DistancePirSensor();
    distanceSensor->begin();
    Container::getInstance()->addService(distanceSensor);
   #endif
   #if FEATURE_DISTANCE_TOF_SENSOR
    DistanceTofSensor *distanceSensor = new DistanceTofSensor();
    distanceSensor->begin();
    Container::getInstance()->addService(distanceSensor);
   #endif
  #endif

  #if FEATURE_SCREEN
    ScreenManager *screenManager = new ScreenManager();
    screenManager->begin();
    Container::getInstance()->addService(screenManager);
  #endif

  #if FEATURE_MOTION_SENSOR
    MotionManager *motionManager = new MotionManager();
    motionManager->begin();
    motionManager->attachMotionInterrupt();
    Container::getInstance()->addService(motionManager);
  #endif

  #if FEATURE_DISTANCE_SENSOR
    // wakeup ir setup
    // esp_deep_sleep_enable_gpio_wakeup(1 << DISTANCE_SENSOR_PIR_PIN, ESP_GPIO_WAKEUP_GPIO_HIGH);
    // gpio_set_direction((gpio_num_t)DISTANCE_SENSOR_PIR_PIN, GPIO_MODE_INPUT);
  #endif

  // Check reason of wakeup
//   esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
//   Serial.print("Wakeup reason: ");
//   Serial.println(wakeup_reason);

  #if FEATURE_SETUP_MANAGER
    if (Container::getInstance()->getStorage().getItem("initial_setup", "true") == "true") {
      Serial.println("Initial setup called");
      SetupManager *setupManager = new SetupManager();
      setupManager->begin();
      Container::getInstance()->addService(setupManager);
    } else {
      Serial.println("Normal routine started");
  #else
    {
      Serial.println("Normal routine started");
  #endif

    #if FEATURE_DISTANCE_SENSOR && FEATURE_SOAP_MOTOR
      // if (wakeup_reason == ESP_SLEEP_WAKEUP_EXT0) {
      //   Serial.println("🖐 Обнаружена рука — подача мыла!");
      //   Container::getInstance()->getService("SoapMotor")->execute("dispense");
      //   lastDispenseTime = millis();
      // }
    #endif

    #if FEATURE_BATTERY_MANAGER
      BatteryManager *batteryManager = new BatteryManager();
      batteryManager->begin();
      Container::getInstance()->addService(batteryManager);
      Serial.print("Battery: ");
      Serial.print(batteryManager->getPercentage());
      Serial.println("%");
    #endif

    #if FEATURE_SLEEP_TIMER
      Container::getInstance()->addService(new SleepTimer());
      Container::getInstance()->getService("SleepTimer")->execute("start");
    #endif
  }
}

void loop() {
  IService* setup = Container::getInstance()->getServiceByType(IService::Type::Setup);

  if (setup != nullptr) {
      setup->loop();
      return;
  }

  for (IService* service : Container::getInstance()->getAllServices()) {
      if (service) {
          service->loop();
      }
  }

//   #if FEATURE_BATTERY_MANAGER
//     batteryManager.update();
//   #endif
//
//   #if FEATURE_SCREEN
//     screenManager.draw();
//   #endif


  IService *distanceSensor = Container::getInstance()->getService("DistanceSensor");
  bool handDetected = distanceSensor && distanceSensor->getValue("handDetected").asBool();
  if (handDetected && millis() - lastDispenseTime > 2000) {
    Serial.println("🖐 Рука снова обнаружена — подаём мыло.");
    Container::getInstance()->getService("SoapMotor")->execute("dispense");
    lastDispenseTime = millis();

    IService *sleepTimer = Container::getInstance()->getService("SleepTimer");
    if (sleepTimer) {
      sleepTimer->execute("start");
    }
  }
}
