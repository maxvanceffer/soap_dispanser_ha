#pragma once

// Rotary knob
#define ROTARY_WAKEUP_PIN GPIO_NUM_33

// Motor driver
#define MOTOR_IN1     GPIO_NUM_25
#define MOTOR_IN2     GPIO_NUM_26
#define MOTOR_N_SLEEP GPIO_NUM_12

// Пины для Rotary Encoder (если используешь)
#define ROTARY_DT_PIN   GPIO_NUM_32
#define ROTARY_CLK_PIN  GPIO_NUM_35
#define ROTARY_BTN_PIN  GPIO_NUM_34

// OLED (I2C)
#define OLED_SDA GPIO_NUM_21
#define OLED_SCL GPIO_NUM_22

// Distance IR sensor
#define DISTANCE_SENSOR_PIN GPIO_NUM_36  // или другой подходящий ADC пин

// PIR sensor
#define PIR_GPIO GPIO_NUM_2

// battery and power pins
#define BATTERY_ADC_PIN      GPIO_NUM_39  // ADC1 канал
#define CHARGING_DETECT_PIN  GPIO_NUM_4   // LOW = заряжается (например, с IP5306)