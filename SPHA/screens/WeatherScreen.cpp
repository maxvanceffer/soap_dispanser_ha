#include "WeatherScreen.h"
#include "../ScreenManager.h"
#include <ArduinoJson.h>
#include "../StorageManager.h"

WeatherScreen::WeatherScreen(StorageManager* storage) : storage(storage) {
  refresh();
}

void WeatherScreen::draw(ScreenManager* manager) {
  manager->setFont(u8g2_font_6x12_tf);
  manager->clearBuffer();

  if (sensors.empty()) {
    manager->drawStr(0, 15, "No data");
    manager->sendBuffer();
    return;
  }

  int y = 14;
  for (const auto& item : sensors) {
    String line = item.title + ": " + item.value;
    if (item.unit.length()) line += " " + item.unit;

    manager->drawStr(0, y, line.c_str());
    y += 12;
    if (y > 64) break;
  }

  manager->sendBuffer();
}

const char* WeatherScreen::getTitle() {
  return "Weather";
}

void WeatherScreen::refresh() {
  sensors.clear();

  String json = storage->getItem("ha_sensors", "[]");
  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, json);
  if (error) return;

  for (JsonObject item : doc.as<JsonArray>()) {
    WeatherData data;
    data.title = item["title"].as<String>();

    if (item["value"].is<JsonArray>()) {
      JsonArray arr = item["value"].as<JsonArray>();
      for (int i = 0; i < arr.size(); ++i) {
        data.value += String(arr[i].as<const char*>());
        if (i < arr.size() - 1) data.value += ", ";
      }
    } else {
      data.value = item["value"].as<String>();
    }

    data.unit = item["unit"].as<String>();
    sensors.push_back(data);
  }
}
