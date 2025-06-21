#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button
{
private:
  int pin;
  bool lastState;
  bool currentState;
  bool wasPressedFlag;
  bool wasSinglePressedFlag;
  bool wasLongPressedFlag;
  unsigned long pressStartTime;
  unsigned long lastDebounceTime;
  const unsigned long debounceDelay = 50;
  const unsigned long longPressDelay = 3000; // 3 seconds for long press

public:
  Button(int pin);

  void begin();
  void update();

  // State queries
  bool wasPressed();
  bool wasSinglePressed();
  bool wasLongPressed();
  bool isPressed() const;
  bool isSinglePressed();
  bool isLongPressed();
  unsigned long getPressDuration() const;

  // Reset all flags
  void reset();
};

#endif