#pragma once

#include "../IScreen.h"

class DeviceInfoScreen : public IScreen {
public:
  DeviceInfoScreen();

  const char* getTitle() override;
  void draw(ScreenManager* manager) override;
  String id() const override;
  void onRotate(int direction) override {}
  void onRotaryClick() override {}
  void onRotaryDoubleClick() override {}
};
