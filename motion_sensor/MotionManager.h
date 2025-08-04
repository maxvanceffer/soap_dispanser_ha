#ifndef MOTION_MANAGER_H
#define MOTION_MANAGER_H

#include <Arduino.h>
#include "../IService.h"

class MotionManager: public IService {
  public:
    MotionManager();

    String name() const override;
    ServiceValue BatteryManager::getValue(String key) const override;
    bool execute(String fnName, JsonVariant args) override;

    void begin();
    void attachMotionInterrupt();

  private:
    static void handleInterrupt();
};

#endif
