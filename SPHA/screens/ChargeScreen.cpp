#include "ChargeScreen.h"
#include "../ScreenManager.h"

ChargeScreen::ChargeScreen(BatteryManager* battery) : battery(battery) {
  // Constructor implementation
}

const char* ChargeScreen::getTitle() {
  return "Charging";
}

void ChargeScreen::draw(ScreenManager* manager) {
  char buf[16];
  sprintf(buf, "%d%%", battery->getPercentage());

  manager->setFont(u8g2_font_6x10_tf);
  manager->drawStr(0, 20, "Charging");

  manager->setFont(u8g2_font_logisoso32_tf);
  manager->drawStr(0, 60, buf);
}
