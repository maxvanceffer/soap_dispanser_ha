# SoapDispenserOs - Soap Dispenser Operating System

This project provides an Arduino-based solution for integrating a soap dispenser with Home Assistant.

## Features

- Detects when soap is dispensed using an analog sensor
- Integrates with Home Assistant via MQTT
- Configurable settings stored in a persistent cache
- Support for ESP32 boards with file system capabilities

## Hardware Components

- **ESP32-C3** - Low-power RISC-V microcontroller. Any other versions of esp32 will be supported in future.
- **SSD1309 OLED Display** - OLED display module with 4-pin SPI/IIC I2C (right now only supported 2.4 inch 128x64px)
- **DRV8833** - Dual H-bridge motor driver for controlling the soap dispensing motor
- **18650 Lithium Battery** - 3.7V lithium-ion rechargeable battery for power supply
- **DC-DC Step down MP1584EN 3A** - For power up esp32 and other wakeup devices.
- **Proximity Sensor Obstacle Avoidance** - For hand detection, as wakeup device and soap dispense.
- **PIR Motion Human AM312** - Wakeup device, for screen wakeup. 
- **4.2V Charge/Discharge Module** - Boost converter with battery protection and indicator for 3.7V lithium battery

## Project Structure

  - `src/` - Contains all source code files
  - `SPHA.ino` - Main Arduino sketch
  - `README.md` - Detailed documentation

## Getting Started

1. Clone this repository
2. Open the SPHA.ino file in the Arduino IDE
3. Install the required dependencies (ArduinoJson)
4. Configure your hardware pins in the sketch
5. Upload to your Arduino board

For more detailed instructions, see the [src/README.md](SPHA/README.md) file.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
