#include "TimeManager.h"
#include "../../Container.h"

TimeManager::TimeManager(): _lastSync(0) {
}

String TimeManager::name() const {
  return "Time";
}

ServiceValue TimeManager::getValue(String key) const {
  if (key == "offset") {
    return ServiceValue(_gmtOffset);
  } else if (key == "daylightOffset") {
    return ServiceValue(_daylightOffset);
  } else if (key == "lastSync") {
    return ServiceValue(_lastSync);
  } else if (key == "time") {
    return ServiceValue(getFormattedTime());
  } else if (key == "isTimeValid") {
    return ServiceValue(isTimeValid());
  }

  return ServiceValue();
}

bool TimeManager::execute(String fnName, JsonVariant args) {
  if (fnName == "begin") {
    return begin();
  }
  
  return false;
}

bool TimeManager::begin(long gmtOffset, int daylightOffset) {
  _gmtOffset = gmtOffset;
  _daylightOffset = daylightOffset;

  time_t now = time(nullptr);
  _lastSync = Container::getInstance()
      ->getStorage()
      .getItem("last_ntp_sync", "0")
      .toInt();

  if (WiFi.status() == WL_CONNECTED && now - _lastSync > 86400) {
    configTime(_gmtOffset, _daylightOffset, "pool.ntp.org");
    delay(2000); // wait for sync
    _lastSync = time(nullptr);
    if (_lastSync > 1000) {
      Container::getInstance()
          ->getStorage()
          .setItem("last_ntp_sync", String(_lastSync));

      Serial.println("NTP time synced.");
      return true;
    } else {
      Serial.println("Failed to sync time.");
    }
  }

  return false;
}

String TimeManager::getFormattedTime() const {
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);
  char buffer[9];
  snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d",
           timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
  return String(buffer);
}

bool TimeManager::isTimeValid() const {
  return time(nullptr) > 1000;
}

time_t TimeManager::now() const {
  return time(nullptr);
}

void TimeManager::buildSettingsSchema(JsonObject schema) const {
  // Initialize schema skeleton (from IService)
  initSchemaSkeleton(schema);

  JsonObject props = schema["properties"].as<JsonObject>();

  // NTP server address
  JsonObject srv = props.createNestedObject("server");
  srv["type"] = "string";
  srv["default"] = "pool.ntp.org"; // current hardcoded default
  srv["title"] = "NTP server";
  srv["description"] = "Hostname of the NTP server.";
  {
    JsonObject comp = srv.createNestedObject("component");
    comp["ui"] = "input";
    comp["type"] = "string";
  }

  // GMT offset in seconds (-12h .. +14h)
  JsonObject gmt = props.createNestedObject("gmtOffset");
  gmt["type"] = "integer";
  gmt["minimum"] = -43200; // -12 hours
  gmt["maximum"] =  50400; // +14 hours
  gmt["default"] = (long)_gmtOffset;
  gmt["title"] = "GMT offset (sec)";
  gmt["description"] = "Time zone offset from UTC in seconds (-43200..50400).";
  {
    JsonObject comp = gmt.createNestedObject("component");
    comp["ui"] = "input";
    comp["type"] = "int";
  }

  // Daylight Saving offset in seconds (0..7200)
  JsonObject dst = props.createNestedObject("daylightOffset");
  dst["type"] = "integer";
  dst["minimum"] = 0;
  dst["maximum"] = 7200;
  dst["default"] = (int)_daylightOffset;
  dst["title"] = "Daylight offset (sec)";
  dst["description"] = "DST offset in seconds (0..7200).";
  {
    JsonObject comp = dst.createNestedObject("component");
    comp["ui"] = "input";
    comp["type"] = "int";
  }
}