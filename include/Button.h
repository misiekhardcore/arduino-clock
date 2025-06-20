#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
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
  bool isPressed() const;
  bool wasPressed();
  bool isSinglePressed();
  bool isLongPressed();
  unsigned long getPressDuration() const;
  
  // Reset flags
  void resetWasPressed();
  void resetWasSinglePressed();
  void resetWasLongPressed();
};

#endif 