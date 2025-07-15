#include "SettingsScreen.h"

SettingsScreen::SettingsScreen(StorageManager* storage) : storage(storage) {
  options.push_back({
    .key = "sleep_timeout",
    .label = "Sleep Timeout",
    .value = storage->getItem("sleep_timeout", "10"),
    .isBool = false
  });

  options.push_back({
    .key = "bluetooth_sensor",
    .label = "Bluetooth",
    .value = storage->getItem("bluetooth_sensor", "false") == "true" ? "On" : "Off",
    .isBool = true
  });
}

const char* SettingsScreen::getTitle() {
  return "Settings";
}

void SettingsScreen::draw(U8G2* display) {
  display->setFont(u8g2_font_6x10_tf);
  display->clearBuffer();

  for (int i = 0; i < options.size(); ++i) {
    String label = options[i].label;
    if (i == selectedIndex && editing) {
      label += " *";
    }

    String line = label + ": " + options[i].value;
    int y = 12 * (i + 1);

    if (i == selectedIndex) {
      display->setDrawColor(1);       // Белый фон
      display->drawBox(0, y - 10, 128, 12);
      display->setDrawColor(0);       // Чёрный текст
      display->drawStr(2, y, line.c_str());
      display->setDrawColor(1);       // Вернуть цвет текста
    } else {
      display->drawStr(2, y, line.c_str());
    }
  }

  display->sendBuffer();
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

void SettingsScreen::onClick() {
  editing = !editing;
}

void SettingsScreen::onDoubleClick() {
  for (const auto& option : options) {
    String rawValue = option.isBool ? (option.value == "On" ? "true" : "false") : option.value;
    storage->setItem(option.key, rawValue);
  }

  editing = false;
}
