// ClockScreen.h
#pragma once

#include <WiFi.h>
#include "../IScreen.h"

class ClockScreen : public IScreen {
public:
  ClockScreen();

  const char* getTitle() override;
  void draw(ScreenManager* manager) override;
  String id() const override;
};
