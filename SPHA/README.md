# SPHA - Soap Dispenser Home Assistant Integration

This folder contains the source code for the Soap Dispenser Home Assistant Integration project.

## Project Structure

- `SPHA.ino`: Main Arduino sketch file
- `Cache.h` and `Cache.cpp`: Cache class for storing configuration and JSON-like data
- `README.md`: This file

## Dependencies

- [ArduinoJson](https://arduinojson.org/) - Used for JSON handling

## Hardware Requirements

- ESP8266 or ESP32 board (recommended for file system support)
- Analog sensor for soap dispenser detection
- Optional: LED for visual feedback

## Setup Instructions

1. Install the ArduinoJson library using the Arduino Library Manager
2. Connect your hardware according to the pin configuration in the sketch
3. Upload the sketch to your board
4. Monitor the serial output to verify the configuration

## Cache Class Usage

The Cache class provides a simple way to store and retrieve configuration values and JSON-like data. Here's how to use it:

```cpp
// Create a cache instance (optionally with a file path for persistent storage)
Cache cache("/config.json");

// Store values
cache.setString("device_name", "soap_dispenser");
cache.setInt("sensor_pin", 5);
cache.setFloat("trigger_threshold", 10.0);
cache.setBool("led_enabled", true);

// Create and store JSON objects
JsonObject haConfig = cache.createNestedObject("ha_config");
haConfig["entity_id"] = "binary_sensor.soap_dispenser";
haConfig["name"] = "Soap Dispenser";

// Retrieve values
String deviceName = cache.getString("device_name");
int sensorPin = cache.getInt("sensor_pin");
float threshold = cache.getFloat("trigger_threshold");
bool ledEnabled = cache.getBool("led_enabled");

// Retrieve JSON objects
JsonObject config = cache.getJson("ha_config");
String entityId = config["entity_id"].as<String>();

// Save to file (if file path was provided)
cache.save();

// Load from file (if file path was provided)
cache.load();
```

## Home Assistant Integration

This project can be integrated with Home Assistant using MQTT. The configuration for the Home Assistant integration is stored in the cache under the "ha_config" key.

To integrate with Home Assistant, you'll need to:

1. Set up an MQTT broker
2. Configure the MQTT settings in the sketch
3. Add the MQTT integration in Home Assistant
4. Use the auto-discovery feature of Home Assistant to discover the soap dispenser sensor

## License

See the LICENSE file in the root directory for license information.