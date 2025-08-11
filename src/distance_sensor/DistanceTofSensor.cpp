#include "DistanceTofSensor.h"
#include "../../GPIO.h"
#include "../../Container.h"

#define DISTANCE_SENSOR_XSHUT_PIN 3  // XSHUT подключён к GPIO3

DistanceTofSensor::DistanceTofSensor() : lox(), handDetectThresholdMm(100) {
  addSupportFeature("distance", true);
  addSupportFeature("hand", true);
}

String DistanceTofSensor::name() const {
  return "DistanceSensor";
}

ServiceValue DistanceTofSensor::getValue(String key) const {
  if (key == "handDetected") {
    return ServiceValue(detectHand());
  }
  return ServiceValue();
}

bool DistanceTofSensor::execute(String fnName, JsonVariant args) {
  if (fnName == "detectHand") {
    return detectHand();
  }
  return false;
}

void DistanceTofSensor::begin() {
  // Загрузка настройки расстояния для определения руки
  String distanceStr = Container::getInstance()->getStorage().getItem("hand_distance_threshold", "100");
  handDetectThresholdMm = distanceStr.toInt();
  Serial.printf("🛠 Порог обнаружения руки: %d мм\n", handDetectThresholdMm);

  // Инициализация XSHUT
  pinMode(DISTANCE_SENSOR_XSHUT_PIN, OUTPUT);
  digitalWrite(DISTANCE_SENSOR_XSHUT_PIN, HIGH);
  delay(100);

  Wire.begin(8, 9);

  if (!lox.begin()) {
    Serial.println("❌ Не удалось найти VL53L0X");
    setReady(false);
    return;
  }

  Serial.println("✅ TOF-сенсор инициализирован");
  setReady(true);
}

void DistanceTofSensor::loop() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);

  if (measure.RangeStatus != 4) {
    Serial.print("Расстояние: ");
    Serial.print(measure.RangeMilliMeter);
    Serial.println(" мм");
  } else {
    Serial.println("Нет объекта в зоне действия");
  }
}

bool DistanceTofSensor::detectHand() const {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);

  if (measure.RangeStatus != 4 && measure.RangeMilliMeter <= handDetectThresholdMm) {
    Serial.printf("🖐 Обнаружена рука на расстоянии %d мм\n", measure.RangeMilliMeter);
    return true;
  }

  return false;
}