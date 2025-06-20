#include "RTClock.h"

RTClock::RTClock() : sdaPin(0), sclPin(0)
{
}

void RTClock::begin(int sdaPin, int sclPin)
{
  this->sdaPin = sdaPin;
  this->sclPin = sclPin;

  Wire.begin();

  if (!rtcModule.begin())
  {
    Serial.println("Couldn't find RTC");
  }
  else
  {
    if (!rtcModule.isrunning())
    {
      Serial.println("RTC is NOT running!");
      // Set the RTC to the date & time this sketch was compiled
      rtcModule.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
  }
}

Time RTClock::getTime()
{
  DateTime now = rtcModule.now();
  return {now.hour(), now.minute(), now.second()};
}

Date RTClock::getDate()
{
  DateTime now = rtcModule.now();
  return {now.day(), now.month(), now.year()};
}

void RTClock::setTime(const Time &time)
{
  DateTime now = rtcModule.now();
  DateTime newDateTime(now.year(), now.month(), now.day(),
                       time.hour, time.minute, time.second);
  rtcModule.adjust(newDateTime);
}

void RTClock::setDate(const Date &date)
{
  DateTime now = rtcModule.now();
  DateTime newDateTime(date.year, date.month, date.day,
                       now.hour(), now.minute(), now.second());
  rtcModule.adjust(newDateTime);
}

RTC_DS1307 *RTClock::getModule()
{
  return &rtcModule;
}