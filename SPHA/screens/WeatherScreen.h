// WeatherScreen.h
#ifndef WEATHER_SCREEN_H
#define WEATHER_SCREEN_H

#include "IScreen.h"
#include "../StorageManager.h"
#include <ArduinoJson.h>

class WeatherScreen : public IScreen {
public:
  explicit WeatherScreen(StorageManager* storage);
  void draw(ScreenManager* manager) override;
  const char* getTitle() override;

private:
  StorageManager* storage;
};

#endif // WEATHER_SCREEN_H
