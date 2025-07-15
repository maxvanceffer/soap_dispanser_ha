// ScreenManager.cpp with replaceScreen method to avoid memory leaks
#include "ScreenManager.h"
#include "./screens/ClockScreen.h"
#include "./screens/ChargeScreen.h"
#include "./screens/DeviceInfoScreen.h"
#include "./screens/SensorScreen.h"
#include "./screens/SetupInfoScreen.h"
#include "./screens/WeatherScreen.h"

ScreenManager::ScreenManager(StorageManager* storage, BatteryManager* battery, TimeManager* time)
  : display(U8G2_R0, /* clock=*/ 22, /* data=*/ 21, /* reset=*/ U8X8_PIN_NONE),
    storage(storage),
    battery(battery),
    time(time) {

    if (storage->isKeySetAndNotEmpty("wifi_ssid") && storage->isKeySetAndNotEmpty("wifi_password")) {
      Serial.println("WiFi credentials found, adding ClockScreen");
      screens.push_back(new ClockScreen(time));

      if (storage->isKeySetAndNotEmpty("ha_endpoint") && storage->isKeySetAndNotEmpty("ha_ip") && storage->isKeySetAndNotEmpty("ha_token")) {
        Serial.println("HA integration is setup, add sensor screen")
        screens.push_back(new SensorScreen(storage))
      }
    }

    screens.push_back(new DeviceInfoScreen(storage, battery));
}

void ScreenManager::begin() {
    display.begin();
    display.clearBuffer();
    display.setFont(u8g2_font_6x10_tf);
    display.drawStr(0, 0, "Check screen working...");
    display.sendBuffer();
}

void ScreenManager::displayScreen(IScreen* screen, bool display) {
  Serial.println("Display screen called");
  screens.push_back(screen);
  if (display) {
    currentIndex = screens.size() - 1;
    draw();
  }
}

void ScreenManager::replaceScreen(IScreen* screen) {
  Serial.println("Replace screen called");
  for (auto* s : screens) delete s;
  screens.clear();
  screens.push_back(screen);
  currentIndex = 0;
  draw();
}

void ScreenManager::draw() {
  Serial.println("Draw current screen");
  if (screens.empty()) return;
  Serial.println("Screen found start draw");
  display.clearBuffer();

  if (screens[currentIndex]->isFullScreen()) {
    // For full-screen mode, just let the screen draw everything
    screens[currentIndex]->draw(this);
  } else {
    // Normal mode with title and page indicator
    display.setFont(u8g2_font_6x10_tf);
    display.drawStr(0, 10, screens[currentIndex]->getTitle());
    screens[currentIndex]->draw(this);
    drawPageIndicator();
  }

  display.sendBuffer();
}

void ScreenManager::next() {
  if (screens.size() > 1) {
    currentIndex = (currentIndex + 1) % screens.size();
  }
}

void ScreenManager::prev() {
  if (screens.size() > 1) {
    currentIndex = (currentIndex - 1 + screens.size()) % screens.size();
  }
}

void ScreenManager::click() {
  screens[currentIndex]->onRotaryClick();
}

void ScreenManager::doubleClick() {
  screens[currentIndex]->onRotaryDoubleClick();
}

void ScreenManager::rotate(int dir) {
  screens[currentIndex]->onRotate(dir);
}

void ScreenManager::drawPageIndicator() {
  Serial.println("ScreenManager::drawPageIndicator");
  int count = screens.size();
  if (count <= 1) return;

  const int spacing = 8;
  const int radius = 2;
  int totalWidth = spacing * (count - 1);
  int startX = (128 - totalWidth) / 2;
  int y = 63;

  for (int i = 0; i < count; ++i) {
    int x = startX + i * spacing;
    if (i == currentIndex) {
      display.drawDisc(x, y, radius);
    } else {
      display.drawCircle(x, y, radius);
    }
  }
}

void ScreenManager::clearBuffer() {
  display.clearBuffer();
}

void ScreenManager::sendBuffer() {
  display.sendBuffer();
}

void ScreenManager::setFont(const uint8_t* font) {
  display.setFont(font);
}

void ScreenManager::drawStr(int x, int y, const char* text) {
  display.drawStr(x, y, text);
}

int ScreenManager::getStrWidth(const char* text) {
  return display.getStrWidth(text);
}

void ScreenManager::drawDisc(int x, int y, int r) {
  display.drawDisc(x, y, r);
}

void ScreenManager::drawCircle(int x, int y, int r) {
  display.drawCircle(x, y, r);
}

void ScreenManager::setDrawColor(uint8_t color) {
  display.setDrawColor(color);
}

void ScreenManager::drawBox(int x, int y, int w, int h) {
  display.drawBox(x, y, w, h);
}
