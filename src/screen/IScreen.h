#pragma once

#include <Arduino.h>

// Forward declaration
class ScreenManager;

class IScreen {
public:
  virtual ~IScreen() = default;

  virtual void draw(ScreenManager* manager) = 0;
  virtual const char* getTitle() = 0;
  virtual String id() const = 0;
  virtual bool isInteractive() { return false; }
  virtual bool isFullScreen() { return false; }
  virtual void onRotaryClick() {}
  virtual void onRotaryDoubleClick() {}
  virtual void onRotate(int direction) {}
};
