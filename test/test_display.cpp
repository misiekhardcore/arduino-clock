#include <Arduino.h>
#include "Display.h"

// Pin definitions (matching old Arduino program)
#define BCD_1_PIN 3
#define BCD_2_PIN 4
#define BCD_3_PIN 5
#define BCD_4_PIN 6
#define SHIFT_PIN 8
#define CLOCK_PIN 7

Display display;

void setup()
{
  Serial.begin(9600);

  // Initialize display with BCD multiplexing
  display.begin(BCD_1_PIN, BCD_2_PIN, BCD_3_PIN, BCD_4_PIN, SHIFT_PIN, CLOCK_PIN);

  Serial.println("BCD Display Test Started");
}

void loop()
{
  // Test 1: Count from 0 to 9 on each digit
  for (int digit = 0; digit < 6; digit++)
  {
    for (int value = 0; value < 10; value++)
    {
      // Set all digits to blank except the current one
      char chars[6] = {' ', ' ', ' ', ' ', ' ', ' '};
      chars[digit] = '0' + value;
      display.setChars(chars[0], chars[1], chars[2], chars[3], chars[4], chars[5]);

      // Update display for 1 second
      for (int i = 0; i < 100; i++)
      {
        display.update();
        delay(10);
      }
    }
  }

  // Test 2: Show "123456"
  display.setChars('1', '2', '3', '4', '5', '6');
  for (int i = 0; i < 200; i++)
  {
    display.update();
    delay(10);
  }

  // Test 3: Show time format "12:34:56" with dots
  display.setChars('1', '2', '3', '4', '5', '6');
  display.setDotState(true);
  for (int i = 0; i < 200; i++)
  {
    display.update();
    delay(10);
  }

  // Test 4: Show letters "HELLO"
  display.setChars('H', 'E', 'L', 'L', 'O', ' ');
  for (int i = 0; i < 200; i++)
  {
    display.update();
    delay(10);
  }

  // Test 5: Show "CLOCK"
  display.setChars('C', 'L', 'O', 'C', 'K', ' ');
  for (int i = 0; i < 200; i++)
  {
    display.update();
    delay(10);
  }

  // Test 6: Blink dots
  for (int blink = 0; blink < 10; blink++)
  {
    display.setDotState(true);
    for (int i = 0; i < 50; i++)
    {
      display.update();
      delay(10);
    }

    display.setDotState(false);
    for (int i = 0; i < 50; i++)
    {
      display.update();
      delay(10);
    }
  }

  Serial.println("Test cycle completed");
}