// ClockScreen.cpp
#include "ClockScreen.h"
#include "../../ScreenManager.h"

ClockScreen::ClockScreen(TimeManager* timeManager) : timeManager(timeManager) {
  // Constructor implementation
}

void ClockScreen::draw(ScreenManager* manager) {
  if (WiFi.status() != WL_CONNECTED) {
    manager->setFont(u8g2_font_6x12_tf);
    manager->drawStr(10, 30, "WiFi not connected");
    return;
  }

  time_t now = timeManager->now();
  if (now < 1000) {
    manager->setFont(u8g2_font_6x12_tf);
    manager->drawStr(10, 30, "Waiting for time...");
    return;
  }

  struct tm* timeinfo = localtime(&now);
  char timeStr[9];
  snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

  manager->setFont(u8g2_font_logisoso32_tr);
  int16_t width = manager->getStrWidth(timeStr);
  int16_t x = (128 - width) / 2;
  manager->drawStr(x, 40, timeStr);

  // Draw clock icon in top-right corner
  manager->setFont(u8g2_font_open_iconic_embedded_1x_t);
  manager->drawStr(128 - 8, 8, "@");
}

const char* ClockScreen::getTitle() {
  return "Clock";
}
