#ifndef SETTINGS_SCREEN_H
#define SETTINGS_SCREEN_H

#include "IScreen.h"
#include "../StorageManager.h"

struct SettingOption {
  String key;
  String label;
  String value;
  bool isBool;
};

class SettingsScreen : public IScreen {
public:
  explicit SettingsScreen(StorageManager* storage);
  const char* getTitle() override;
  void draw(U8G2* display) override;
  void onRotate(int direction) override;
  void onRotaryClick() override;
  void onRotaryDoubleClick() override;

private:
  StorageManager* storage;
  std::vector<SettingOption> options;
  int selectedIndex = 0;
  bool editing = false;
};

#endif
