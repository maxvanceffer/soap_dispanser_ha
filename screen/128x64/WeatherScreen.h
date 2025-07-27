// WeatherScreen.h
#ifndef WEATHER_SCREEN_H
#define WEATHER_SCREEN_H

#include "IScreen.h"
#include "../StorageManager.h"
#include <ArduinoJson.h>
#include <vector>

struct WeatherData {
  String title;
  String value;
  String unit;
};

class WeatherScreen : public IScreen {
public:
  explicit WeatherScreen(StorageManager* storage);
  void draw(ScreenManager* manager) override;
  const char* getTitle() override;
  void refresh();

private:
  StorageManager* storage;
  std::vector<WeatherData> sensors;
};

#endif // WEATHER_SCREEN_H
