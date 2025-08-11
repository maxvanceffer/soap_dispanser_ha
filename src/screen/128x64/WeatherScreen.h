// WeatherScreen.h
#ifndef WEATHER_SCREEN_H
#define WEATHER_SCREEN_H

#include <ArduinoJson.h>
#include <vector>

#include "../IScreen.h"

struct WeatherData {
  String title;
  String value;
  String unit;
};

class WeatherScreen : public IScreen {
public:
  explicit WeatherScreen();
  void draw(ScreenManager* manager) override;
  const char* getTitle() override;
  String id() const override;
  void refresh();

private:
  std::vector<WeatherData> sensors;
};

#endif // WEATHER_SCREEN_H
