#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include "Clock.h"

class Display
{
private:
  // BCD selector pins for digit multiplexing
  int bcd1Pin;
  int bcd2Pin;
  int bcd3Pin;
  int bcd4Pin;

  // Shift register pins
  int shiftPin;
  int clockPin;

  // 7-segment display patterns (matching old Arduino program)
  static const uint8_t digitTable[14];

  // 7-segment display patterns for letters
  static const uint8_t letterPatterns[26];

  void displayDigit(uint8_t pattern);
  void multiplexDisplay();
  uint8_t getPatternForChar(char c);

public:
  Display();

  void begin(int bcd1Pin, int bcd2Pin, int bcd3Pin, int bcd4Pin, int shiftPin, int clockPin);

  // Display state (public for direct access like old Arduino program)
  char digit1, digit2, digit3, digit4, digit5, digit6;
  bool dotState;

  // Display methods
  // Note: '*' character is mapped to degrees symbol (circle pattern)
  void print(const char *str);
  void clear();

  // Direct digit control (matching old Arduino approach)
  void setDigits(int d1, int d2, int d3, int d4, int d5, int d6);
  void setChars(char c1, char c2, char c3, char c4, char c5, char c6);
  void setDotState(bool dotState);
  void update();

  // Helper methods
  void showNumber(int number, int position);
  void showDigit(uint8_t digit, int position);
  void showLetter(char letter, int position);
};

#endif