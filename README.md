# SPHA - Soap Dispenser Home Assistant Integration

This project provides an Arduino-based solution for integrating a soap dispenser with Home Assistant.

## Features

- Detects when soap is dispensed using an analog sensor
- Integrates with Home Assistant via MQTT
- Configurable settings stored in a persistent cache
- Support for ESP8266 and ESP32 boards with file system capabilities

## Project Structure

- `src/` - Contains all source code files
  - `SPHA.ino` - Main Arduino sketch
  - `Cache.h` and `Cache.cpp` - Cache class for configuration storage
  - `README.md` - Detailed documentation

## Getting Started

1. Clone this repository
2. Open the SPHA.ino file in the Arduino IDE
3. Install the required dependencies (ArduinoJson)
4. Configure your hardware pins in the sketch
5. Upload to your Arduino board

For more detailed instructions, see the [src/README.md](SPHA/README.md) file.

## License

This project is licensed under the terms of the license included in the repository.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
