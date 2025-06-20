#ifndef RTC_H
#define RTC_H

#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include "Clock.h"

class RTClock
{
private:
  RTC_DS1307 rtcModule;
  int sdaPin;
  int sclPin;

public:
  RTClock();

  void begin(int sdaPin, int sclPin);

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