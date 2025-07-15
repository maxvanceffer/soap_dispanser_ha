#include "DeviceInfoScreen.h"
#include "../ScreenManager.h"
#include <WiFi.h>

DeviceInfoScreen::DeviceInfoScreen(StorageManager* storage, BatteryManager* battery) : storage(storage), battery(battery) {
  // Constructor implementation
}

const char* DeviceInfoScreen::getTitle() {
  return "Device Info";
}

void DeviceInfoScreen::draw(ScreenManager* manager) {
  manager->setFont(u8g2_font_6x10_tf);

  char line[32];
  int y = 12;

  sprintf(line, "Battery: %d%%", battery->getPercentage());
  manager->drawStr(0, y, line); y += 12;

  IPAddress ip = WiFi.localIP();
  sprintf(line, "IP: %s", ip.toString().c_str());
  manager->drawStr(0, y, line); y += 12;

  String haIp = storage->getItem("ha_ip", "");
  if (!haIp.isEmpty()) {
    sprintf(line, "HA IP: %s", haIp.c_str());
    manager->drawStr(0, y, line); y += 12;
  }

  int sleepTimeout = storage->getItem("sleep_timeout", "10").toInt();
  sprintf(line, "Sleep: %ds", sleepTimeout);
  manager->drawStr(0, y, line);
}
