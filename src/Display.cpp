#include "Display.h"

// 7-segment display patterns (matching old Arduino program exactly)
// B00111111, B00000110, B01011011, B01001111, B01100110,
// B01101101, B01111101, B00100111, B01111111, B01101111,
// B01100011, B00111001, B00000000, B01110110
const uint8_t Display::digitTable[14] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00100111, // 7
    0b01111111, // 8
    0b01101111, // 9
    0b01100011, // 10 (special character)
    0b00111001, // 11 (special character)
    0b00000000, // 12 (blank)
    0b01110110  // 13 (special character)
};

// 7-segment display patterns for letters (common cathode)
// Segment order: a, b, c, d, e, f, g, dp
const uint8_t Display::letterPatterns[26] = {
    0b11101110, // A
    0b00111110, // b
    0b00111001, // C
    0b01111010, // d
    0b10011110, // E
    0b10001110, // F
    0b10111100, // G
    0b01110110, // H
    0b01100000, // I
    0b01110000, // J
    0b00000000, // K (not supported)
    0b00011100, // L
    0b00000000, // M (not supported)
    0b00101010, // n
    0b11111100, // O
    0b11001110, // P
    0b00000000, // Q (not supported)
    0b11001110, // R
    0b10110110, // S
    0b00000000, // T (not supported)
    0b01111100, // U
    0b00000000, // V (not supported)
    0b00000000, // W (not supported)
    0b00000000, // X (not supported)
    0b01100110, // Y
    0b11011010  // Z
};

Display::Display() : bcd1Pin(0), bcd2Pin(0), bcd3Pin(0), bcd4Pin(0),
                     shiftPin(0), clockPin(0), digit1(0), digit2(0),
                     digit3(0), digit4(0), digit5(0), digit6(0), dotState(false)
{
}

void Display::begin(int bcd1Pin, int bcd2Pin, int bcd3Pin, int bcd4Pin, int shiftPin, int clockPin)
{
  this->bcd1Pin = bcd1Pin;
  this->bcd2Pin = bcd2Pin;
  this->bcd3Pin = bcd3Pin;
  this->bcd4Pin = bcd4Pin;
  this->shiftPin = shiftPin;
  this->clockPin = clockPin;

  pinMode(bcd1Pin, OUTPUT);
  pinMode(bcd2Pin, OUTPUT);
  pinMode(bcd3Pin, OUTPUT);
  pinMode(bcd4Pin, OUTPUT);
  pinMode(shiftPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  digitalWrite(bcd1Pin, LOW);
  digitalWrite(bcd2Pin, LOW);
  digitalWrite(bcd3Pin, LOW);
  digitalWrite(bcd4Pin, LOW);
  digitalWrite(shiftPin, LOW);
  digitalWrite(clockPin, LOW);

  clear();
}

void Display::displayDigit(uint8_t pattern)
{
  // Shift out the pattern (matching old Arduino program's Display function)
  shiftOut(shiftPin, clockPin, MSBFIRST, pattern);
}

void Display::multiplexDisplay()
{
  // Multiplex through all 6 digits (matching old Arduino program exactly)
  // Order: DIGIT_6, DIGIT_5, DIGIT_4, DIGIT_3, DIGIT_2, DIGIT_1

  // DIGIT_6 (rightmost)
  digitalWrite(bcd1Pin, HIGH);
  digitalWrite(bcd2Pin, LOW);
  digitalWrite(bcd3Pin, LOW);
  digitalWrite(bcd4Pin, LOW);
  displayDigit(getPatternForChar(digit6));
  delay(2);

  // DIGIT_5
  digitalWrite(bcd1Pin, LOW);
  digitalWrite(bcd2Pin, HIGH);
  digitalWrite(bcd3Pin, LOW);
  digitalWrite(bcd4Pin, LOW);
  displayDigit(getPatternForChar(digit5));
  delay(2);

  // DIGIT_4 (with dot)
  digitalWrite(bcd1Pin, HIGH);
  digitalWrite(bcd2Pin, HIGH);
  digitalWrite(bcd3Pin, LOW);
  digitalWrite(bcd4Pin, LOW);
  displayDigit(getPatternForChar(digit4) + (dotState ? 0b10000000 : 0));
  delay(2);

  // DIGIT_3
  digitalWrite(bcd1Pin, LOW);
  digitalWrite(bcd2Pin, LOW);
  digitalWrite(bcd3Pin, HIGH);
  digitalWrite(bcd4Pin, LOW);
  displayDigit(getPatternForChar(digit3));
  delay(2);

  // DIGIT_2 (with dot)
  digitalWrite(bcd1Pin, HIGH);
  digitalWrite(bcd2Pin, LOW);
  digitalWrite(bcd3Pin, HIGH);
  digitalWrite(bcd4Pin, LOW);
  displayDigit(getPatternForChar(digit2) + (dotState ? 0b10000000 : 0));
  delay(2);

  // DIGIT_1 (leftmost)
  digitalWrite(bcd1Pin, LOW);
  digitalWrite(bcd2Pin, HIGH);
  digitalWrite(bcd3Pin, HIGH);
  digitalWrite(bcd4Pin, LOW);
  displayDigit(getPatternForChar(digit1));
  delay(2);
}

uint8_t Display::getPatternForChar(char c)
{
  // Handle digits 0-9
  if (c >= '0' && c <= '9')
  {
    return digitTable[c - '0'];
  }

  // Handle special characters 10-13 (for backward compatibility)
  if (c >= 10 && c <= 13)
  {
    return digitTable[(int)c];
  }

  // Handle asterisk as degrees symbol
  if (c == '*')
  {
    return digitTable[10]; // degrees symbol
  }

  // Handle space
  if (c == ' ')
  {
    return digitTable[12]; // blank
  }

  // Handle letters A-Z
  if (c >= 'A' && c <= 'Z')
  {
    return letterPatterns[c - 'A'];
  }

  // Handle letters a-z (convert to uppercase)
  if (c >= 'a' && c <= 'z')
  {
    return letterPatterns[c - 'a'];
  }

  // Return blank for unsupported characters
  return digitTable[12]; // blank
}

void Display::setDigits(int d1, int d2, int d3, int d4, int d5, int d6)
{
  digit1 = d1;
  digit2 = d2;
  digit3 = d3;
  digit4 = d4;
  digit5 = d5;
  digit6 = d6;
}

void Display::setChars(char c1, char c2, char c3, char c4, char c5, char c6)
{
  digit1 = c1;
  digit2 = c2;
  digit3 = c3;
  digit4 = c4;
  digit5 = c5;
  digit6 = c6;
}

void Display::setDotState(bool dotState)
{
  this->dotState = dotState;
}

void Display::update()
{
  multiplexDisplay();
}

void Display::print(const char *str)
{
  // Extract exactly 6 characters from the string
  setChars(str[0], str[1], str[2], str[3], str[4], str[5]);
}

void Display::clear()
{
  setChars(12, 12, 12, 12, 12, 12);
  setDotState(false);
}

void Display::showNumber(int number, int position)
{
  if (position >= 0 && position < 6)
  {
    int digits[6] = {digit1, digit2, digit3, digit4, digit5, digit6};
    digits[position] = number % 10;
    setDigits(digits[0], digits[1], digits[2], digits[3], digits[4], digits[5]);
  }
}

void Display::showDigit(uint8_t digit, int position)
{
  if (position >= 0 && position < 6 && digit < 10)
  {
    int digits[6] = {digit1, digit2, digit3, digit4, digit5, digit6};
    digits[position] = digit;
    setDigits(digits[0], digits[1], digits[2], digits[3], digits[4], digits[5]);
  }
}

void Display::showLetter(char letter, int position)
{
  if (position >= 0 && position < 6)
  {
    char chars[6] = {digit1, digit2, digit3, digit4, digit5, digit6};
    chars[position] = letter;
    setChars(chars[0], chars[1], chars[2], chars[3], chars[4], chars[5]);
  }
}