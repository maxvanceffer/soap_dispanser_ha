#include "MotionManager.h"
#include "../GPIO.h"

SleepTimer* MotionManager::sleepTimerInstance = nullptr;

MotionManager::MotionManager(SleepTimer* sleepTimer) {
  sleepTimerInstance = sleepTimer;
}

void MotionManager::begin() {
  pinMode(PIR_GPIO, INPUT);
}

void MotionManager::attachMotionInterrupt() {
  attachInterrupt(digitalPinToInterrupt(PIR_GPIO), handleInterrupt, RISING);
}

void IRAM_ATTR MotionManager::handleInterrupt() {
  if (sleepTimerInstance != nullptr) {
    sleepTimerInstance->start();
  }

  // Так как это ISR, используем минимальные средства
  ets_printf("Motion detected (ISR)\n");  // ISR-safe лог
}
