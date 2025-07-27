#include "SetupInfoScreen.h"
#include "../../ScreenManager.h"

SetupInfoScreen::SetupInfoScreen(std::initializer_list<String> lines, const String& icon, unsigned long autoDismissMs, bool showSpinner)
  : lines(lines), icon(icon), autoDismissMs(autoDismissMs), showSpinner(showSpinner) {
  if (autoDismissMs > 0) {
    dismissAt = millis() + autoDismissMs;
  }
}

const char* SetupInfoScreen::getTitle() {
  return "Setup";
}

void SetupInfoScreen::draw(ScreenManager* manager) {
Serial.println("SetupInfoScreen:draw called");

    manager->clearBuffer();
    manager->setFont(u8g2_font_5x8_tf);

    const int lineHeight = 12;
    int y = 0;

    if (!icon.isEmpty()) {
      manager->setFont(u8g2_font_open_iconic_embedded_1x_t);
      manager->drawStr(60, 10, icon.c_str());
      manager->setFont(u8g2_font_5x8_tf);
      y += 6;
    }

    if (showSpinner) {
      static const char* frames[] = {"-", "\\", "|", "/"};
      static uint8_t frameIndex = 0;
      frameIndex = (millis() / 250) % 4;
      manager->drawStr(118, 10, frames[frameIndex]);
    }

    int totalHeight = lines.size() * lineHeight;
    y += (64 - totalHeight) / 2 + lineHeight;

    for (const auto& line : lines) {
      manager->drawStr(6, y, line.c_str());
      y += lineHeight;
    }

    manager->sendBuffer();
    Serial.println("SetupInfoScreen:draw send buffer");
}

bool SetupInfoScreen::shouldAutoDismiss() const {
  return autoDismissMs > 0 && millis() >= dismissAt;
}
