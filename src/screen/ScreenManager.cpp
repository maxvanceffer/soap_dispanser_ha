#include "ScreenManager.h"
#include "../../Container.h"
#include "../storage/StorageManager.h"

#if FEATURE_SCREEN_128x64
#include "./128x64/ClockScreen.h"
#include "./128x64/ChargeScreen.h"
#include "./128x64/DeviceInfoScreen.h"
#include "./128x64/SensorScreen.h"
#include "./128x64/SetupInfoScreen.h"
#include "./128x64/WeatherScreen.h"
#endif

ScreenManager::ScreenManager(): display(U8G2_R0, /* clock=*/ 22, /* data=*/ 21, /* reset=*/ U8X8_PIN_NONE) {
    StorageManager& storage = Container::getInstance()->getStorage();

    if (storage.isKeySetAndNotEmpty("wifi_ssid") && storage.isKeySetAndNotEmpty("wifi_password")) {
      Serial.println("WiFi credentials found, adding ClockScreen");
      // screens.push_back(new ClockScreen());

      if (storage.isKeySetAndNotEmpty("ha_endpoint") && storage.isKeySetAndNotEmpty("ha_ip") && storage.isKeySetAndNotEmpty("ha_token")) {
        Serial.println("HA integration is setup, add sensor screen");
        // screens.push_back(new SensorScreen());
      }
    }

    //screens.push_back(new DeviceInfoScreen());
}

String ScreenManager::name() const {
    return "Screen";
}

ServiceValue ScreenManager::getValue(String key) const {
    if (key == "currentIndex") {
        return ServiceValue(currentIndex);
    } else if (key == "currentId") {
        return ServiceValue(currentIndex);
    }

    return ServiceValue();
}

bool ScreenManager::execute(String fnName, JsonVariant args) {
    if (fnName == "begin") {
        return begin();
    } else if (fnName == "next") {
        return next();
    } else if (fnName == "prev") {
        return prev();
    } else if (fnName == "click") {
        return click();
    } else if (fnName == "doubleClick") {
        return doubleClick();
    } else if (fnName == "showScreen" && args.is<String>()) {
        return showScreen(args.as<String>());
    }

    return false;
}

bool ScreenManager::begin() {
    display.begin();
    display.clearBuffer();
    display.setFont(u8g2_font_6x10_tf);
    display.drawStr(0, 0, "Check screen working...");
    display.sendBuffer();
    return true;
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

bool ScreenManager::next() {
  if (screens.size() > 1) {
    currentIndex = (currentIndex + 1) % screens.size();
  }
  return true;
}

bool ScreenManager::prev() {
  if (screens.size() > 1) {
    currentIndex = (currentIndex - 1 + screens.size()) % screens.size();
  }
  return true;
}

bool ScreenManager::click() {
  screens[currentIndex]->onRotaryClick();
  return true;
}

bool ScreenManager::doubleClick() {
  screens[currentIndex]->onRotaryDoubleClick();
  return true;
}

bool ScreenManager::showScreen(const String& id) {
  for (size_t i = 0; i < screens.size(); i++) {
    if (screens[i]->id() == id) {
      currentIndex = i;
      draw();
      return true;
    }
  }
  Serial.print("Screen with id '");
  Serial.print(id);
  Serial.println("' not found");
  return false;
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
