// ScreenManager.h
#pragma once

#include <vector>
#include <U8g2lib.h>
#include "./screens/IScreen.h"
#include "StorageManager.h"
#include "TimeManager.h"
#include "BatteryManager.h"
#include "GPIO.h"

class ScreenManager {
private:
  std::vector<IScreen*> screens;
  int currentIndex = 0;
  U8G2_SSD1306_128X64_NONAME_F_SW_I2C display;
  BatteryManager* battery;
  StorageManager* storage;
  TimeManager* time;

  void drawPageIndicator();

public:
  ScreenManager(StorageManager* storage, BatteryManager* battery, TimeManager* time);
  void displayScreen(IScreen* screen, bool display = true);
  void replaceScreen(IScreen* screen);
  void draw();
  void next();
  void prev();
  void click();
  void doubleClick();
  void rotate(int dir);
  void begin();

  // Methods to provide access to the display for screens
  void clearBuffer();
  void sendBuffer();
  void setFont(const uint8_t* font);
  void drawStr(int x, int y, const char* text);
  int getStrWidth(const char* text);
  void drawDisc(int x, int y, int r);
  void drawCircle(int x, int y, int r);
};
