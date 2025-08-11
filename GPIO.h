#pragma once

//
// ESP32-C3 Super Mini

// Rotary knob
#define ROTARY_WAKEUP_PIN 33

// Motor driver
#define MOTOR_IN1     7 // already soldered
#define MOTOR_IN2     6 // already soldered
#define MOTOR_N_SLEEP 12 // not yet soldered

// Пины для Rotary Encoder (если используешь)
#define ROTARY_DT_PIN   32 // not yet soldered
#define ROTARY_CLK_PIN  35 // not yet soldered
#define ROTARY_BTN_PIN  34 // not yet soldered

// OLED (I2C)
#define OLED_SDA 21 // not yet soldered
#define OLED_SCL 22 // not yet soldered

// Distance IR sensor
#define DISTANCE_SENSOR_PIR_PIN 4 // not yet soldered
#define DISTANCE_SENSOR_TOF_SDA_PIN 8 // already soldered
#define DISTANCE_SENSOR_TOF_SCL_PIN 9 // already soldered
#define DISTANCE_SENSOR_TOF_SHUT_PIN 3 // already soldered

// PIR sensor
#define PIR_GPIO 2 // already soldered

// battery and power pins
#define BATTERY_ADC_PIN      39  // not yet soldered
#define CHARGING_DETECT_PIN  10   // not yet soldered
