#include "MotionManager.h"
#include "../../GPIO.h"
#include "../../Container.h"

MotionManager::MotionManager() {}

String MotionManager::name() const {
    return "Motion";
}

ServiceValue MotionManager::getValue(String key) const {
    return ServiceValue();
}

bool MotionManager::execute(String fnName, JsonVariant args) {
    return false;
}

void MotionManager::begin() {
  pinMode(PIR_GPIO, INPUT);
  setReady(true);
}

void MotionManager::attachMotionInterrupt() {
  attachInterrupt(digitalPinToInterrupt(PIR_GPIO), handleInterrupt, RISING);
}

void IRAM_ATTR MotionManager::handleInterrupt() {
  IService *sleepTimer = Container::getInstance()->getService("SleepTimer");
  if (sleepTimer) {
    sleepTimer->execute("start");
  }

  // Так как это ISR, используем минимальные средства
  ets_printf("Motion detected (ISR)\n");  // ISR-safe лог
}
