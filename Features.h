#pragma once

/**
 * Features Configuration File
 * 
 * This file contains feature flags to enable or disable various components
 * of the Soap Dispenser project. Change the values to 1 (enabled) or 0 (disabled)
 * according to your needs.
 * 
 * Minimal working set includes:
 * - DistanceSensor
 * - SoapMotor
 * 
 * Component Dependencies:
 * - SoapMotor requires StorageManager
 * - SleepTimer requires StorageManager
 * - ScreenManager requires StorageManager
 * - BatteryScreen requires BatteryManager
 * - ClockScreen requires TimeManager
 * - SetupManager requires StorageManager, SleepTimer, and ScreenManager
 * 
 * Dependencies are automatically enforced - enabling a component will
 * automatically enable its dependencies.
 */

// Core components (minimal working set)
#define FEATURE_DISTANCE_SENSOR 1  // IR sensor to detect hands
#define FEATURE_SOAP_MOTOR 1       // Motor for dispensing soap
#define FEATURE_SLEEP_TIMER 0      // Timer for sleep mode (required for power management)

// Core components variants
#define FEATURE_DISTANCE_PIR_SENSOR 0 // Use PIR sensor for detecting hand (only one must be enabled at time)
#define FEATURE_DISTANCE_TOF_SENSOR 1 // Use TOF sensor for detecting hand (only one must be enabled at time)

// Optional components
#define FEATURE_BATTERY_MANAGER 0   // Battery monitoring
#define FEATURE_TIME_MANAGER 0      // Time-related functions
#define FEATURE_MOTION_SENSOR 0     // PIR motion detection
#define FEATURE_SETUP_MANAGER 1     // Initial setup mode

#define FEATURE_SCREEN 0            // OLED display
#define FEATURE_SCREEN_128x64 1     // OLED display resolution

// Screen features (only relevant if FEATURE_SCREEN is enabled)
#define FEATURE_CLOCK_SCREEN 0     // Clock display screen
#define FEATURE_BATTERY_SCREEN 0   // Battery status screen
#define FEATURE_SETTINGS_SCREEN 0  // Settings configuration screen
#define FEATURE_WEATHER_SCREEN 0   // Weather information screen
#define FEATURE_SENSOR_SCREEN 0    // Sensor data display screen

// Dependency enforcement - DO NOT MODIFY THIS SECTION
// If a component is enabled, its dependencies are automatically enabled

// SoapMotor requires StorageManager
#if FEATURE_SOAP_MOTOR
  #undef FEATURE_STORAGE
  #define FEATURE_STORAGE 1
#endif

// SleepTimer requires StorageManager
#if FEATURE_SLEEP_TIMER
  #undef FEATURE_STORAGE
  #define FEATURE_STORAGE 1
#endif

// ScreenManager requires StorageManager
#if FEATURE_SCREEN
  #undef FEATURE_STORAGE
  #define FEATURE_STORAGE 1
#endif

// SetupManager requires StorageManager, SleepTimer, and ScreenManager
#if FEATURE_SETUP_MANAGER
  #undef FEATURE_STORAGE
  #define FEATURE_STORAGE 1
  #undef FEATURE_SLEEP_TIMER
  #define FEATURE_SLEEP_TIMER 1
  #undef FEATURE_SCREEN
  #define FEATURE_SCREEN 0
#endif

// Screen-specific features require FEATURE_SCREEN
#if FEATURE_CLOCK_SCREEN || FEATURE_BATTERY_SCREEN || FEATURE_SETTINGS_SCREEN || FEATURE_WEATHER_SCREEN || FEATURE_SENSOR_SCREEN
  #undef FEATURE_SCREEN
  #define FEATURE_SCREEN 1
#endif

// ClockScreen requires TimeManager
#if FEATURE_CLOCK_SCREEN
  #undef FEATURE_TIME_MANAGER
  #define FEATURE_TIME_MANAGER 1
#endif

// BatteryScreen requires BatteryManager
#if FEATURE_BATTERY_SCREEN
  #undef FEATURE_BATTERY_MANAGER
  #define FEATURE_BATTERY_MANAGER 1
#endif