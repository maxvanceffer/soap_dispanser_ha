#ifndef SETTINGS_SCREEN_H
#define SETTINGS_SCREEN_H

#include <Arduino.h>
#include "../IScreen.h"
#include <vector>

struct SettingOption {
  String key;
  String label;
  String value;
  bool isBool;
};

class SettingsScreen : public IScreen {
public:
  explicit SettingsScreen();

  const char* getTitle() override;
  void draw(ScreenManager* manager) override;
  void onRotate(int direction) override;
  void onRotaryClick() override;
  void onRotaryDoubleClick() override;

private:
  std::vector<SettingOption> options;
  int selectedIndex = 0;
  bool editing = false;
};

#endif
