#include "Button.h"

Button::Button(int pin) : pin(pin), lastState(false), currentState(false),
                          wasPressedFlag(false), wasSinglePressedFlag(false), wasLongPressedFlag(false),
                          pressStartTime(0), lastDebounceTime(0)
{
}

void Button::begin()
{
  pinMode(pin, INPUT);
  digitalWrite(pin, LOW);
}

void Button::update()
{
  // Read the current state
  bool reading = digitalRead(pin); // Inverted because of pull-up

  // Check if the switch changed
  if (reading != lastState)
  {
    lastDebounceTime = millis();
  }

  // If enough time has passed since the last change
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    // If the button state has changed
    if (reading != currentState)
    {
      currentState = reading;

      if (currentState)
      {
        // Button pressed
        pressStartTime = millis();
        wasPressedFlag = true;
      }
      else
      {
        // Button released
        unsigned long pressDuration = millis() - pressStartTime;

        if (pressDuration < longPressDelay && pressDuration > 50)
        {
          // Single press (between 50ms and 3 seconds)
          wasSinglePressedFlag = true;
        }
      }
    }
  }

  // Check for long press
  if (currentState && (millis() - pressStartTime) >= longPressDelay)
  {
    wasLongPressedFlag = true;
  }

  lastState = reading;
}

bool Button::isPressed() const
{
  return currentState;
}

bool Button::wasPressed()
{
  bool result = wasPressedFlag;
  wasPressedFlag = false;
  return result;
}

bool Button::isSinglePressed()
{
  bool result = wasSinglePressedFlag;
  wasSinglePressedFlag = false;
  return result;
}

bool Button::isLongPressed()
{
  bool result = wasLongPressedFlag;
  wasLongPressedFlag = false;
  return result;
}

unsigned long Button::getPressDuration() const
{
  if (currentState)
  {
    return millis() - pressStartTime;
  }
  return 0;
}

void Button::resetWasPressed()
{
  wasPressedFlag = false;
}

void Button::resetWasSinglePressed()
{
  wasSinglePressedFlag = false;
}

void Button::resetWasLongPressed()
{
  wasLongPressedFlag = false;
}