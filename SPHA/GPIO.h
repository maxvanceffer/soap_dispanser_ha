#pragma once

// Rotary knob
#define ROTARY_WAKEUP_PIN 33

// Motor driver
#define MOTOR_IN1     5
#define MOTOR_IN2     7
#define MOTOR_N_SLEEP 12

// Пины для Rotary Encoder (если используешь)
#define ROTARY_DT_PIN   32
#define ROTARY_CLK_PIN  35
#define ROTARY_BTN_PIN  34

// OLED (I2C)
#define OLED_SDA 21
#define OLED_SCL 22

// Distance IR sensor
#define DISTANCE_SENSOR_PIN 36  // или другой подходящий ADC пин

// PIR sensor
#define PIR_GPIO 2

// battery and power pins
#define BATTERY_ADC_PIN      39  // ADC1 канал
#define CHARGING_DETECT_PIN  4   // LOW = заряжается (например, с IP5306)
