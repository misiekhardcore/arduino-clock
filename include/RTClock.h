#ifndef RTC_H
#define RTC_H

#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>

// Time and Date structures
struct Time
{
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
};

struct Date
{
  uint8_t day;
  uint8_t month;
  uint16_t year;
};

class RTClock
{
private:
  RTC_DS1307 rtcModule;
  uint8_t sdaPin;
  uint8_t sclPin;

public:
  RTClock();

  void begin(uint8_t sdaPin, uint8_t sclPin);

  // Time and date getters
  Time getTime();
  Date getDate();

  // Time and date setters
  void setTime(const Time &time);
  void setDate(const Date &date);

  // RTC module access
  RTC_DS1307 *getModule();
};

#endif