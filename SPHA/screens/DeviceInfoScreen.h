#pragma once

#include "IScreen.h"
#include "../StorageManager.h"
#include "../BatteryManager.h"

class DeviceInfoScreen : public IScreen {
private:
  StorageManager* storage;
  BatteryManager* battery;

public:
  DeviceInfoScreen(StorageManager* storage, BatteryManager* battery);
  const char* getTitle() override;
  void draw(ScreenManager* manager) override;
  void onRotate(int direction) override {}
  void onRotaryClick() override {}
  void onRotaryDoubleClick() override {}
};
