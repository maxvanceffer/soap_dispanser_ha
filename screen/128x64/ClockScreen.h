// ClockScreen.h
#pragma once

#include "IScreen.h"
#include "../TimeManager.h"
#include <WiFi.h>

class ClockScreen : public IScreen {
public:
  ClockScreen(TimeManager* timeManager);
  void draw(ScreenManager* manager) override;
  const char* getTitle() override;

private:
  TimeManager* timeManager;
};
