#ifndef SENSOR_SCREEN_H
#define SENSOR_SCREEN_H

#include <vector>
#include <Arduino.h>
#include "IScreen.h"
#include "../StorageManager.h"

struct SensorData {
  String title;
  String value;
  String unit;
};

class SensorScreen : public IScreen {
public:
  explicit SensorScreen(StorageManager* storage);
  const char* getTitle() override;
  void draw(ScreenManager* manager) override;
  void onRotate(int direction) override;

private:
  std::vector<SensorData> sensors;
  int index = 0;
};

#endif
