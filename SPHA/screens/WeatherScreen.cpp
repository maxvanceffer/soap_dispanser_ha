#include "WeatherScreen.h"
#include "../ScreenManager.h"
#include <ArduinoJson.h>
#include "../StorageManager.h"

WeatherScreen::WeatherScreen(StorageManager* storage) : storage(storage) {
  // Constructor implementation
}

void WeatherScreen::draw(ScreenManager* manager) {
  manager->setFont(u8g2_font_6x12_tf);
  manager->clearBuffer();

  String json = storage->getItem("ha_sensors", "[]");
  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, json);
  if (error) {
    manager->drawStr(0, 15, "Failed to parse sensors");
    manager->sendBuffer();
    return;
  }

  int y = 14;
  for (JsonObject item : doc.as<JsonArray>()) {
    String title = item["title"] | "";
    String value;

    if (item["value"].is<JsonArray>()) {
      JsonArray arr = item["value"].as<JsonArray>();
      for (int i = 0; i < arr.size(); ++i) {
        value += String(arr[i].as<const char*>());
        if (i < arr.size() - 1) value += ", ";
      }
    } else {
      value = item["value"].as<String>();
    }

    String unit = item["unit"] | "";
    String line = title + ": " + value;
    if (unit.length()) line += " " + unit;

    manager->drawStr(0, y, line.c_str());
    y += 12;
    if (y > 64) break;
  }

  manager->sendBuffer();
}

const char* WeatherScreen::getTitle() {
  return "Weather";
}
