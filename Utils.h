#pragma once

#include <Arduino.h>
#include "esp_sleep.h"

/**
 * Utils.h - Utility functions for the SPHA project
 * 
 * This file contains helper functions that can be used across the project.
 * Add any general-purpose utility functions here.
 */

/**
 * Prints the reason for ESP32 wakeup from sleep
 */
void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0:
      Serial.println("Wakeup caused by external signal using RTC_IO");
      break;
    case ESP_SLEEP_WAKEUP_EXT1:
      Serial.println("Wakeup caused by external signal using RTC_CNTL");
      break;
    case ESP_SLEEP_WAKEUP_TIMER:
      Serial.println("Wakeup caused by timer");
      break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD:
      Serial.println("Wakeup caused by touchpad");
      break;
    case ESP_SLEEP_WAKEUP_ULP:
      Serial.println("Wakeup caused by ULP program");
      break;
    // new wakeup reason for the RiscV ESP32Cx
    case ESP_SLEEP_WAKEUP_GPIO:
      Serial.println("Wakeup caused by a GPIO");
      break;
    default:
      Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
      break;
  }
}