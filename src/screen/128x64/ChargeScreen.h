#pragma once

#include "../IScreen.h"

class ChargeScreen : public IScreen {
public:
  ChargeScreen();

  const char* getTitle() override;
  void draw(ScreenManager* manager) override;
  String id() const override;
  bool isFullScreen() override { return true; }
  void onRotate(int direction) override {}
  void onRotaryClick() override {}
  void onRotaryDoubleClick() override {}
};
