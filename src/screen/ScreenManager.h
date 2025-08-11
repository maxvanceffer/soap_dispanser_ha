// ScreenManager.h
#pragma once

#include <vector>
#include <U8g2lib.h>
#include "IScreen.h"
#include "../../IService.h"

class ScreenManager: public IService {
private:
  std::vector<IScreen*> screens;
  int currentIndex = 0;
  U8G2_SSD1306_128X64_NONAME_F_SW_I2C display;

  void drawPageIndicator();

public:
  ScreenManager();

  String name() const override;
  ServiceValue getValue(String key) const override;
  bool execute(String fnName, JsonVariant args) override;

  void displayScreen(IScreen* screen, bool display = true);
  void replaceScreen(IScreen* screen);
  bool showScreen(const String& id);
  void draw();
  bool next();
  bool prev();
  bool click();
  bool doubleClick();
  void rotate(int dir);
  bool begin();

  // Methods to provide access to the display for screens
  void clearBuffer();
  void sendBuffer();
  void setFont(const uint8_t* font);
  void drawStr(int x, int y, const char* text);
  int getStrWidth(const char* text);
  void drawDisc(int x, int y, int r);
  void drawCircle(int x, int y, int r);
  void setDrawColor(uint8_t color);
  void drawBox(int x, int y, int w, int h);
};
