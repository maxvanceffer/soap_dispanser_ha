#include "SetupInfoScreen.h"
#include "ChargeScreen.h"
#include "ClockScreen.h"
#include "SensorScreen.h"
#include "WeatherScreen.h"
#include "DeviceInfoScreen.h"

// This file is used to ensure that all screen implementations are included in the build.
// The Arduino IDE sometimes has issues with linking when the implementation is in a separate file.

// Include all implementation files
#include "SetupInfoScreen.cpp"
#include "ChargeScreen.cpp"
#include "ClockScreen.cpp"
#include "SensorScreen.cpp"
#include "WeatherScreen.cpp"
#include "DeviceInfoScreen.cpp"
