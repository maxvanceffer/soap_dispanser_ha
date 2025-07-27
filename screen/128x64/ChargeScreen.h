#pragma once

#include "IScreen.h"
#include "../BatteryManager.h"

class ChargeScreen : public IScreen {
private:
  BatteryManager* battery;

public:
  ChargeScreen(BatteryManager* battery);
  const char* getTitle() override;
  void draw(ScreenManager* manager) override;
  bool isFullScreen() override { return true; }
  void onRotate(int direction) override {}
  void onRotaryClick() override {}
  void onRotaryDoubleClick() override {}
};
