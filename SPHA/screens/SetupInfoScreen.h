#ifndef SETUP_INFO_SCREEN_H
#define SETUP_INFO_SCREEN_H

#include "IScreen.h"

class SetupInfoScreen : public IScreen {
public:
  SetupInfoScreen(std::initializer_list<String> lines, const String& icon = "", unsigned long autoDismissMs = 0, bool showSpinner = false);
  const char* getTitle() override;
  void draw(ScreenManager* manager) override;
  bool isFullScreen() override { return true; }
  bool shouldAutoDismiss() const;

private:
  std::vector<String> lines;
  String icon;
  unsigned long autoDismissMs = 0;
  unsigned long dismissAt = 0;
  bool showSpinner = false;
};

#endif
