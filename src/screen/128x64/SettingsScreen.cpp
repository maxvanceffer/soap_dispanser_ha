#include "SettingsScreen.h"
#include "../ScreenManager.h"
#include "../../../Container.h"

SettingsScreen::SettingsScreen() {
  options.push_back({
    .key = "sleep_timeout",
    .label = "Sleep Timeout",
    .value = Container::getInstance()->getStorage().getItem("sleep_timeout", "10"),
    .isBool = false
  });

  options.push_back({
    .key = "bluetooth_sensor",
    .label = "Bluetooth",
    .value = Container::getInstance()->getStorage().getItem("bluetooth_sensor", "false") == "true" ? "On" : "Off",
    .isBool = true
  });
}

const char* SettingsScreen::getTitle() {
  return "Settings";
}

void SettingsScreen::draw(ScreenManager* manager) {
  manager->setFont(u8g2_font_6x10_tf);
  manager->clearBuffer();

  for (int i = 0; i < options.size(); ++i) {
    String label = options[i].label;
    if (i == selectedIndex && editing) {
      label += " *";
    }

    String line = label + ": " + options[i].value;
    int y = 12 * (i + 1);

    if (i == selectedIndex) {
      manager->setDrawColor(1);       // Белый фон
      manager->drawBox(0, y - 10, 128, 12);
      manager->setDrawColor(0);       // Чёрный текст
      manager->drawStr(2, y, line.c_str());
      manager->setDrawColor(1);       // Вернуть цвет текста
    } else {
      manager->drawStr(2, y, line.c_str());
    }
  }

  manager->sendBuffer();
}

void SettingsScreen::onRotate(int direction) {
  if (!editing) {
    selectedIndex += direction;
    if (selectedIndex < 0) selectedIndex = 0;
    if (selectedIndex >= options.size()) selectedIndex = options.size() - 1;
  } else {
    // Change value
    auto& option = options[selectedIndex];
    if (option.isBool) {
      option.value = (option.value == "On") ? "Off" : "On";
    } else {
      int current = option.value.toInt();
      current += direction;
      if (current < 1) current = 1;
      option.value = String(current);
    }
  }
}

void SettingsScreen::onRotaryClick() {
  editing = !editing;
}

void SettingsScreen::onRotaryDoubleClick() {
  for (const auto& option : options) {
    String rawValue = option.isBool ? (option.value == "On" ? "true" : "false") : option.value;
    Container::getInstance()->getStorage().setItem(option.key, rawValue);
  }

  editing = false;
}
