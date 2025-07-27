#ifndef MOTION_MANAGER_H
#define MOTION_MANAGER_H

#include <Arduino.h>
#include "../sleep_timer/SleepTimer.h"

class MotionManager {
  public:
    MotionManager(SleepTimer* sleepTimer);
    void begin();
    void attachMotionInterrupt();

  private:
    static void handleInterrupt();
    static SleepTimer* sleepTimerInstance;
};

#endif
