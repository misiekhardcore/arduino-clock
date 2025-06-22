#include "Clock.h"
#include "RTClock.h"
#include "HTSensor.h"
#include "Buzzer.h"
#include "EEPROMStorage.h"

// PROGMEM constants to save RAM
const char PROGMEM DEGREE_SYMBOL = '*';
const char PROGMEM TEMP_UNIT = 'C';
const char PROGMEM HUMIDITY_UNIT = 'H';

Clock::Clock() : rtc(nullptr), dht11(nullptr), buzzer(nullptr)
{
}

void Clock::begin(RTClock *rtc, HTSensor *dht11, Buzzer *buzzer)
{
  this->rtc = rtc;
  this->dht11 = dht11;
  this->buzzer = buzzer;
  timer.begin();
  alarm.begin(buzzer);
}

void Clock::update()
{
  // Update timer
  timer.update();

  // Update alarm
  Time currentTime = rtc->getTime();
  alarm.update(currentTime);
}

Time Clock::getTime() const
{
  return rtc->getTime();
}

Date Clock::getDate() const
{
  return rtc->getDate();
}

TimerData Clock::getTimerTime()
{
  return timer.getTime();
}

int8_t Clock::getTemperature() const
{
  return dht11->getTemperature();
}

int8_t Clock::getHumidity() const
{
  return dht11->getHumidity();
}

char *Clock::getTimeString() const
{
  static char timeString[6];
  Time currentTime = rtc->getTime();

  timeString[0] = '0' + currentTime.hour / 10;
  timeString[1] = '0' + currentTime.hour % 10;
  timeString[2] = '0' + currentTime.minute / 10;
  timeString[3] = '0' + currentTime.minute % 10;
  timeString[4] = '0' + currentTime.second / 10;
  timeString[5] = '0' + currentTime.second % 10;
  return timeString;
}

char *Clock::getDateString() const
{
  static char dateString[6];
  static unsigned long lastDateUpdate = 0;
  static Date cachedDate = {1, 1, 2024};

  // Cache date for 1 second to avoid multiple RTC calls
  unsigned long currentMillis = millis();
  if (currentMillis - lastDateUpdate >= 1000)
  {
    cachedDate = rtc->getDate();
    lastDateUpdate = currentMillis;
  }

  dateString[0] = '0' + cachedDate.day / 10;
  dateString[1] = '0' + cachedDate.day % 10;
  dateString[2] = '0' + cachedDate.month / 10;
  dateString[3] = '0' + cachedDate.month % 10;
  dateString[4] = '0' + (cachedDate.year % 100) / 10;
  dateString[5] = '0' + (cachedDate.year % 100) % 10;
  return dateString;
}

char *Clock::getAlarmTimeString() const
{
  static char alarmString[6];
  AlarmData alarmData = alarm.getTime();
  alarmString[0] = '0' + alarmData.hour / 10;
  alarmString[1] = '0' + alarmData.hour % 10;
  alarmString[2] = '0' + alarmData.minute / 10;
  alarmString[3] = '0' + alarmData.minute % 10;
  alarmString[4] = '0';
  alarmString[5] = '0';
  return alarmString;
}

char *Clock::getTimerString() const
{
  static char timerString[6];
  TimerData timerData = timer.getTime();
  timerString[0] = '0' + timerData.hour / 10;
  timerString[1] = '0' + timerData.hour % 10;
  timerString[2] = '0' + timerData.minute / 10;
  timerString[3] = '0' + timerData.minute % 10;
  timerString[4] = '0' + timerData.second / 10;
  timerString[5] = '0' + timerData.second % 10;
  return timerString;
}

char *Clock::getTemperatureString() const
{
  static char temperatureString[6];
  temperatureString[0] = ' ';
  temperatureString[1] = ' ';
  temperatureString[2] = '0' + getTemperature() / 10;
  temperatureString[3] = '0' + getTemperature() % 10;
  temperatureString[4] = DEGREE_SYMBOL;
  temperatureString[5] = TEMP_UNIT;
  return temperatureString;
}

char *Clock::getHumidityString() const
{
  static char humidityString[6];
  humidityString[0] = ' ';
  humidityString[1] = ' ';
  humidityString[2] = '0' + getHumidity() / 10;
  humidityString[3] = '0' + getHumidity() % 10;
  humidityString[4] = DEGREE_SYMBOL;
  humidityString[5] = HUMIDITY_UNIT;
  return humidityString;
}

AlarmData Clock::getAlarmTime()
{
  return alarm.getTime();
}

void Clock::adjustSetting(int setting, int part)
{
  switch (setting)
  {
  case 0: // Time
  {
    Time currentTime = rtc->getTime();
    switch (part)
    {
    case 0: // Hour
      currentTime.hour = (currentTime.hour + 1) % 24;
      if (rtc)
        rtc->setTime(currentTime);
      break;
    case 1: // Minute
      currentTime.minute = (currentTime.minute + 1) % 60;
      if (rtc)
        rtc->setTime(currentTime);
      break;
    case 2: // Second
      currentTime.second = (currentTime.second + 1) % 60;
      if (rtc)
        rtc->setTime(currentTime);
      break;
    }
  }
  break;

  case 1: // Date
  {
    Date currentDate = rtc->getDate();
    switch (part)
    {
    case 0: // Day
      currentDate.day = (currentDate.day % 31) + 1;
      if (rtc)
        rtc->setDate(currentDate);
      break;
    case 1: // Month
      currentDate.month = (currentDate.month % 12) + 1;
      if (rtc)
        rtc->setDate(currentDate);
      break;
    case 2: // Year
      currentDate.year = (currentDate.year % 100) + 2024;
      if (rtc)
        rtc->setDate(currentDate);
      break;
    }
  }
  break;

  case 2: // Alarm
    switch (part)
    {
    case 0: // Hour
      alarm.adjustHour();
      break;
    case 1: // Minute
      alarm.adjustMinute();
      break;
    case 2: // Enable/Disable
      if (alarm.isEnabled())
        alarm.disable();
      else
        alarm.enable();
      break;
    }
    break;
  case 3: // Timer
    switch (part)
    {
    case 0: // Hour
      timer.adjustHour();
      break;
    case 1: // Minute
      timer.adjustMinute();
      break;
    case 2: // Second
      timer.adjustSecond();
      break;
    }
    break;
  }
}

void Clock::loadSettings()
{
  EEPROMStorage eeprom;
  if (eeprom.hasValidSettings())
  {
    Time currentTime = rtc->getTime();
    Date currentDate = rtc->getDate();
    AlarmData alarmData;
    eeprom.loadSettings(currentTime, currentDate, alarmData);
    alarm.setTime(alarmData.hour, alarmData.minute);
    if (alarmData.enabled)
    {
      alarm.enable();
    }
    else
    {
      alarm.disable();
    }
  }
}

void Clock::saveSettings()
{
  EEPROMStorage eeprom;
  AlarmData alarmData = alarm.getTime();
  eeprom.saveSettings(rtc->getTime(), rtc->getDate(), alarmData);
}

bool Clock::isAlarmTriggered() const
{
  return alarm.isTriggered();
}

void Clock::stopAlarm()
{
  alarm.stop();
}

void Clock::startTimer()
{
  timer.start();
}

void Clock::stopTimer()
{
  timer.stop();
}

void Clock::resetTimer()
{
  timer.reset();
}

void Clock::setAlarmTime(uint8_t hour, uint8_t minute)
{
  alarm.setTime(hour, minute);
}

void Clock::enableAlarm()
{
  alarm.enable();
}

void Clock::disableAlarm()
{
  alarm.disable();
}

void Clock::setAlarmData(const AlarmData &alarmData)
{
  alarm.setTime(alarmData.hour, alarmData.minute);
  if (alarmData.enabled)
  {
    alarm.enable();
  }
  else
  {
    alarm.disable();
  }
}

void Clock::setTimerTime(uint8_t hour, uint8_t minute, uint8_t second)
{
  timer.setTime(hour, minute, second);
}

void Clock::setTime(const Time &time)
{
  rtc->setTime(time);
}

void Clock::setDate(const Date &date)
{
  rtc->setDate(date);
}