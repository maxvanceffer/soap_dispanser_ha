#include "ChargeScreen.h"
#include "../ScreenManager.h"
#include "../../../Container.h"

ChargeScreen::ChargeScreen() {
  // Constructor implementation
}

const char* ChargeScreen::getTitle() {
  return "Charging";
}

String ChargeScreen::id() const {
  return "charge";
}

void ChargeScreen::draw(ScreenManager* manager) {
  IService *battery = Container::getInstance()->getService("Battery");
  if (battery) {
    char buf[16];
    sprintf(buf, "%d%%", battery->getValue("percentage").asInt());

    manager->setFont(u8g2_font_6x10_tf);
    manager->drawStr(0, 20, "Charging");

    manager->setFont(u8g2_font_logisoso32_tf);
    manager->drawStr(0, 60, buf);
  }
}
