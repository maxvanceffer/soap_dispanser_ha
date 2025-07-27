// TimeManager.h
#pragma once
#include <Arduino.h>
#include <time.h>
#include <WiFi.h>
#include "StorageManager.h"

class TimeManager {
public:
  TimeManager(StorageManager& storage)
    : _storage(storage), _lastSync(0) {}

  void begin(long gmtOffset = 10800, int daylightOffset = 0) {
    _gmtOffset = gmtOffset;
    _daylightOffset = daylightOffset;

    time_t now = time(nullptr);
    _lastSync = _storage.getItem("last_ntp_sync", "0").toInt();

    if (WiFi.status() == WL_CONNECTED && now - _lastSync > 86400) {
      configTime(_gmtOffset, _daylightOffset, "pool.ntp.org");
      delay(2000); // wait for sync
      _lastSync = time(nullptr);
      if (_lastSync > 1000) {
        _storage.setItem("last_ntp_sync", String(_lastSync));
        Serial.println("NTP time synced.");
      } else {
        Serial.println("Failed to sync time.");
      }
    }
  }

  String getFormattedTime() {
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    char buffer[9];
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d",
             timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    return String(buffer);
  }

  bool isTimeValid() {
    return time(nullptr) > 1000;
  }

  time_t now() {
    return time(nullptr);
  }

private:
  StorageManager& _storage;
  long _gmtOffset;
  int _daylightOffset;
  time_t _lastSync;
};
