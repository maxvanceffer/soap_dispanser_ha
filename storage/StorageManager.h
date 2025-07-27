#pragma once

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

class StorageManager {
public:
    StorageManager(const String& filename = "/data.json") : _filename(filename) {}

    bool begin() {
        if (!LittleFS.begin()) {
            Serial.println("Failed to initialize LittleFS");
            Serial.println("Formatting LittleFS...");
            if (!LittleFS.format()) {
                Serial.println("Failed to format LittleFS");
                return false;
            }
            Serial.println("LittleFS formatted successfully");
            if (!LittleFS.begin()) {
                Serial.println("Failed to initialize LittleFS after formatting");
                return false;
            }
            Serial.println("LittleFS initialized successfully after formatting");
        }
        return load();
    }

    template <typename T>
    void setItem(const String& key, const T& value) {
        _data[key] = value;
        _dirty = true;
    }

    template <typename T>
    T getItem(const String& key, const T& defaultValue = T(), bool persistDefault = false) {
        if (_data.containsKey(key)) {
            return _data[key].as<T>();
        }
        if (persistDefault) {
            _data[key] = defaultValue;
            _dirty = true;
        }
        return defaultValue;
    }

    // Specialized version for string literals
    String getItem(const String& key, const char* defaultValue, bool persistDefault = false) {
        if (_data.containsKey(key)) {
            return _data[key].as<String>();
        }
        if (persistDefault) {
            _data[key] = defaultValue;
            _dirty = true;
        }
        return String(defaultValue);
    }

    bool hasItem(const String& key) {
        return _data.containsKey(key);
    }

    bool isKeySetAndNotEmpty(const String& key) {
        if (!_data.containsKey(key)) {
            return false;
        }
        String value = _data[key].as<String>();
        return value.length() > 0;
    }

    bool save() {
        if (!_dirty) return true;  // Нет изменений
        File file = LittleFS.open(_filename, "w");
        if (!file) {
            Serial.println("Failed to open file for writing");
            return false;
        }

        if (serializeJson(_data, file) == 0) {
            Serial.println("Failed to write JSON to file");
            return false;
        }

        file.close();
        _dirty = false;
        return true;
    }

    bool load() {
        Serial.println("Loading config file");
        File file = LittleFS.open(_filename, "r");
        if (!file) {
            Serial.println("No existing config file found");
            // Create an empty config file
            _dirty = true;
            return save();
        }

        DeserializationError error = deserializeJson(_data, file);
        file.close();

        if (error) {
            Serial.print("Failed to parse config file: ");
            Serial.println(error.c_str());
            // If the file is corrupted, create a new empty one
            _data.clear();
            _dirty = true;
            return save();
        }

        _dirty = false;
        Serial.println("Config loaded");
        return true;
    }

private:
    String _filename;
    DynamicJsonDocument _data{2048};
    bool _dirty = false;
};
