#include <ArduinoJson.h>

#include "SensorScreen.h"
#include "../ScreenManager.h"
#include "../../../Container.h"

SensorScreen::SensorScreen() {
  String jsonPayload = Container::getInstance()->getStorage().getItem("ha_sensors", "");
  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, jsonPayload);
  if (error) return;

  for (JsonObject obj : doc.as<JsonArray>()) {
    SensorData data;
    data.title = obj["title"].as<String>();
    if (obj["value"].is<JsonArray>()) {
      JsonArray arr = obj["value"].as<JsonArray>();
      data.value = arr[0].as<String>();
    } else {
      data.value = obj["value"].as<String>();
    }
    data.unit = obj["unit"].as<String>();
    sensors.push_back(data);
  }
}

const char* SensorScreen::getTitle() {
  return "Sensors";
}

String SensorScreen::id() const {
  return "sensor";
}

void SensorScreen::draw(ScreenManager* manager) {
  if (sensors.empty()) {
    manager->setFont(u8g2_font_6x10_tf);
    manager->drawStr(0, 10, "No data");
    return;
  }

  manager->setFont(u8g2_font_6x10_tf);
  manager->drawStr(0, 10, sensors[index].title.c_str());

  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%s %s", sensors[index].value.c_str(), sensors[index].unit.c_str());
  manager->drawStr(0, 30, buffer);
}

void SensorScreen::onRotate(int direction) {
  if (sensors.empty()) return;

  if (direction > 0) {
    index = (index + 1) % sensors.size();
  } else {
    index = (index + sensors.size() - 1) % sensors.size();
  }
}
